#include <string.h>
#include "sys.h"
#include "usart.h"		
#include "delay.h"	
#include "led.h" 	 
//#include "key.h"	 	 	 	 	 
#include "stmflash.h"
#include "iap.h"


int main(void)
{		 
		/* 定义一些局部变量 */
	u32 appAddr = FLASH_APP1_ADDR;
	unsigned char startTransmit = 0;
	unsigned int fileLength = 0;
	unsigned char pageCount = 0;
	unsigned char headFlag = 0;
	
 	Stm32_Clock_Init(9);	//系统时钟设置
	uart_init(72,9600);	//串口初始化为9600，72 是时钟频率
	delay_init(72);	   	 	//延时初始化 
	LED_Init();		  		//初始化与LED连接的硬件接口
	
	LED0 = 0;
	delay_ms( 5000 );
	delay_ms( 5000 );
	
	while(1) {
		if(((*(vu32*)(FLASH_APP1_ADDR+4))&0xFF000000)==0x08000000) { //判断是否为0X08XXXXXX.
			iap_load_app(FLASH_APP1_ADDR);//执行FLASH APP代码
		}
		if((((*(vu32*)FLASH_APP1_ADDR)&0x2FFE0000)!=0x20000000)) {
			printf( "\r stm32f103c8t6 Upgrade Online \r\n" );
			printf( "\r Press key 1 to send the bin file \r\n" );
			printf( "\r Press key 2 to enter the application program \r\n" );
			
			while(1) {
				if( !startTransmit ) {
					if( USART_RX_BUF[0] == 'A' ) {
						printf( "Start transmit the app bin file ...\n" );
						USART_RX_CNT = 0;
						startTransmit = 1;
						continue;
					}
					if( USART_RX_BUF[0] == 'E' ) {
						printf( "Start the app application ...\n" );
						USART_RX_CNT = 0;
						if(((*(vu32*)(FLASH_APP1_ADDR+4))&0xFF000000)==0x08000000) {//判断是否为0X08XXXXXX.
							iap_load_app(FLASH_APP1_ADDR);//执行FLASH APP代码				
						}
						else {
							printf("非FLASH应用程序,无法执行!\r\n");
							printf("Illegal FLASH APP!");	 			
						}
					}
 				}
				else if( startTransmit ) {
					if( USART_RX_CNT == 4 && headFlag == 0 ) { // 准备接收头
						memcpy( &fileLength, USART_RX_BUF, sizeof( fileLength ) );
						printf( "the file length = %d", fileLength );
						USART_RX_CNT = 0;// 归零
						headFlag = 1; // 表示接收到了头
						continue; // 结束本次循环
					}		
					if( USART_RX_CNT == 1024 && headFlag == 1 ) {
						pageCount ++; 
						iap_write_appbin(appAddr,USART_RX_BUF,1024);//更新FLASH代码   
						appAddr += 1024; // flash 写入地址偏移
						USART_RX_CNT = 0;// 归零
						continue;
					}
					if( ( pageCount +1 == (int)(fileLength / 1024) + 1) && USART_RX_CNT > 0 && headFlag == 1 ){
						iap_write_appbin(appAddr,USART_RX_BUF,USART_RX_CNT);//更新FLASH最后一段代码 
						USART_RX_CNT = 0;// 归零
						pageCount = 0;
						printf( "Have written the bin file to the falsh now, please restart the MCU or send 'E' command ... " );
						startTransmit = 0;
						appAddr = FLASH_APP1_ADDR;
						headFlag = 0;
						delay_ms( 100 );// 延时一会之后开始跳转到应用程序
						iap_load_app( FLASH_APP1_ADDR );
					}
				}
			}
		}
	}
	
	return 0;
}



