#include "exti.h"
#include "led.h"
#include "beep.h"
#include "key.h"
#include "delay.h"
#include "usart.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//�ⲿ�ж� ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/3
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////   
//�ⲿ�ж�0�������
void EXTI0_IRQHandler(void)
{
	delay_ms(10);//����
	if(KEY3==1)	 //WK_UP����
	{				 
		BEEP=!BEEP;	
	}		 
	EXTI->PR=1<<0;  //���LINE0�ϵ��жϱ�־λ  
}
//�ⲿ�ж�2�������
void EXTI2_IRQHandler(void)
{
	delay_ms(10);//����
	if(KEY2==0)	 //����KEY2
	{
		LED0=!LED0;
	}		 
	EXTI->PR=1<<2;  //���LINE2�ϵ��жϱ�־λ  
}
//�ⲿ�ж�3�������
void EXTI3_IRQHandler(void)
{
	delay_ms(10);//����
	if(KEY1==0)	 //����KEY1
	{				 
		LED1=!LED1;
	}		 
	EXTI->PR=1<<3;  //���LINE3�ϵ��жϱ�־λ  
}
//�ⲿ�ж�4�������
void EXTI4_IRQHandler(void)
{
	delay_ms(10);//����
	if(KEY0==0)	 //����KEY0
	{
		LED0=!LED0;
		LED1=!LED1;
	}		 
	EXTI->PR=1<<4;  //���LINE4�ϵ��жϱ�־λ  
}		   
//�ⲿ�жϳ�ʼ������
//��ʼ��PA0/PE2/PE3/PE4Ϊ�ж�����.
void EXTIX_Init(void)
{
	KEY_Init();

	Ex_NVIC_Config(GPIO_A,0,RTIR); 			//�����ش���
	Ex_NVIC_Config(GPIO_E,2,FTIR); 			//�½��ش���
	Ex_NVIC_Config(GPIO_E,3,FTIR); 			//�½��ش���
 	Ex_NVIC_Config(GPIO_E,4,FTIR); 			//�½��ش���

	MY_NVIC_Init(2,3,EXTI0_IRQChannel,2);	//��ռ2�������ȼ�3����2
	MY_NVIC_Init(2,2,EXTI2_IRQChannel,2);	//��ռ2�������ȼ�2����2	   
	MY_NVIC_Init(2,1,EXTI3_IRQChannel,2);	//��ռ2�������ȼ�1����2	   
	MY_NVIC_Init(2,0,EXTI4_IRQChannel,2);	//��ռ2�������ȼ�0����2	   
}












