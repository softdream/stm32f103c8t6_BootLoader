#include "led.h"   

//��ʼ��PB5��PE5Ϊ�����.��ʹ���������ڵ�ʱ��		    
//LED IO��ʼ��
void LED_Init(void)
{
	RCC->APB2ENR|=1<<4;    //ʹ��PORTCʱ��	   	 
	   	 
	GPIOC->CRH&=0XFF0FFFFF; 
	GPIOC->CRH|=0X00300000;//PC.13 �������
	
  GPIOC->ODR|=1<<13;      //PC.13 �����
											  
}






