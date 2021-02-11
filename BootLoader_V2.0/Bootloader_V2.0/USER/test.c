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
		/* ����һЩ�ֲ����� */
	u32 appAddr = FLASH_APP1_ADDR;
	unsigned char startTransmit = 0;
	unsigned int fileLength = 0;
	unsigned char pageCount = 0;
	unsigned char headFlag = 0;
	
 	Stm32_Clock_Init(9);	//ϵͳʱ������
	uart_init(72,9600);	//���ڳ�ʼ��Ϊ9600��72 ��ʱ��Ƶ��
	delay_init(72);	   	 	//��ʱ��ʼ�� 
	LED_Init();		  		//��ʼ����LED���ӵ�Ӳ���ӿ�
	
	LED0 = 0;
	delay_ms( 5000 );
	delay_ms( 5000 );
	
	while(1) {
		if(((*(vu32*)(FLASH_APP1_ADDR+4))&0xFF000000)==0x08000000) { //�ж��Ƿ�Ϊ0X08XXXXXX.
			iap_load_app(FLASH_APP1_ADDR);//ִ��FLASH APP����
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
						if(((*(vu32*)(FLASH_APP1_ADDR+4))&0xFF000000)==0x08000000) {//�ж��Ƿ�Ϊ0X08XXXXXX.
							iap_load_app(FLASH_APP1_ADDR);//ִ��FLASH APP����				
						}
						else {
							printf("��FLASHӦ�ó���,�޷�ִ��!\r\n");
							printf("Illegal FLASH APP!");	 			
						}
					}
 				}
				else if( startTransmit ) {
					if( USART_RX_CNT == 4 && headFlag == 0 ) { // ׼������ͷ
						memcpy( &fileLength, USART_RX_BUF, sizeof( fileLength ) );
						printf( "the file length = %d", fileLength );
						USART_RX_CNT = 0;// ����
						headFlag = 1; // ��ʾ���յ���ͷ
						continue; // ��������ѭ��
					}		
					if( USART_RX_CNT == 1024 && headFlag == 1 ) {
						pageCount ++; 
						iap_write_appbin(appAddr,USART_RX_BUF,1024);//����FLASH����   
						appAddr += 1024; // flash д���ַƫ��
						USART_RX_CNT = 0;// ����
						continue;
					}
					if( ( pageCount +1 == (int)(fileLength / 1024) + 1) && USART_RX_CNT > 0 && headFlag == 1 ){
						iap_write_appbin(appAddr,USART_RX_BUF,USART_RX_CNT);//����FLASH���һ�δ��� 
						USART_RX_CNT = 0;// ����
						pageCount = 0;
						printf( "Have written the bin file to the falsh now, please restart the MCU or send 'E' command ... " );
						startTransmit = 0;
						appAddr = FLASH_APP1_ADDR;
						headFlag = 0;
						delay_ms( 100 );// ��ʱһ��֮��ʼ��ת��Ӧ�ó���
						iap_load_app( FLASH_APP1_ADDR );
					}
				}
			}
		}
	}
	
	return 0;
}



