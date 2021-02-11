#include "WZSerialPort.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <Windows.h>
#include <conio.h>
#include "string.h"

unsigned char sendBuff[1024*20]; // 开辟 20k 的内存用来存储 bin 文件中内容
unsigned char recvBuff[1024];// 接收缓存
unsigned char sendHead[4]; // 发送头信息，包括整个 bin 文件的长度 

WZSerialPort uart;

/* 串口接收线程 */
DWORD WINAPI recvInformation(LPVOID lpParameter)
{
	while (1) {
		memset(recvBuff, 0, sizeof(recvBuff));
		if (uart.receive(recvBuff, sizeof(recvBuff))) {
			std::string recvString;
			recvString = (char*)recvBuff;
			std::cout << "-------------------------------------------------" << std::endl << std::endl;;
			std::cout << "收到的命令：" << recvBuff << std::endl << std::endl;;
			std::cout << " ------------------------------------------------" << std::endl;

		}
	}
}

int main()
{
	unsigned char command = 0;
	/* 读取bin文件*/
	std::ifstream binFile;
	binFile.open( "Template.bin", std::ifstream::binary);
	if( !binFile ){
		std::cerr << "Open binary file failed ..." << std::endl;
		return -1;
	}
	binFile.seekg( 0, std::ios::end );
	unsigned int length = binFile.tellg();                         //获取当前指针位置
	std::cout << "the length of the file is " << length << " " << "byte" << std::endl;
	unsigned char* data = new unsigned char[length](); //读取文件数组
	binFile.seekg(0, std::ios::beg);                            //将输入指针指向文件开头
	binFile.get();
	if (!binFile.eof()) {
		std::cout << "target reading..." << std::endl;
		binFile.read(reinterpret_cast<char*>(data), sizeof(char)* length); //读入文件流
		std::cout << "finished reading" << std::endl;
	}

	binFile.close();  //关闭文件

	/* 拷贝文件长度 */
	memcpy( sendHead, &length, sizeof( length ) );

	/* 打开串口 */
	if( uart.open( "COM3 ", 9600, 0, 8, 1, 1 ) == true){
		std::cout<<"Serial Port open Success !"<< std::endl ;
	}else{
		std::cout<<"Srial Port open Failed !" << std::endl ;
	}

	/* 创建一个接收线程 */
	CreateThread(NULL, 0, &recvInformation, NULL, 0, NULL);
	
	int a = 0;
	/* 主线程 */
	while (1) {
		if (_kbhit()) {//如果有按键按下，则_kbhit()函数返回真
			a = _getch();//使用_getch()函数获取按下的键值
			std::cout <<"按键是："<< a <<std::endl ;
			if (a == 27) { break; }//当按下ESC时循环，ESC键的键值时27.
			if (a == 114) { //  R 键
				command = 'R';
				uart.send(&command, 1);
			}
			if (a == 97) { // A 键
				command = 'A';
				uart.send(&command, 1);
				std::cout << "Starting transmitting the bin file ..." << std::endl;

				Sleep(50); //延时一下
				uart.send(sendHead, sizeof(length)); // 先发送一个文件长度
				Sleep(50); // 延时 50 ms
				int i = 0;
				for (i = 0; i < length - 1024; i += 1024) {
					uart.send(&sendBuff[i], 1024);
					Sleep(1000); // 每发送 1k 的数据延时 100 ms，等待单片机将数据写入 flash
				}
				Sleep(1000);
				uart.send(&sendBuff[i], length - i); // 发送最后一段不足 1k 的数据
				std::cout << "send " << length - i << "bytes" << std::endl;
				std::cout << "Transmit bin file completed ..." << std::endl;
			}
			if (a == 101) { // E 键
				command = 'E';
				uart.send(&command, 1);
			}
		}
	}
	return 0 ;
}