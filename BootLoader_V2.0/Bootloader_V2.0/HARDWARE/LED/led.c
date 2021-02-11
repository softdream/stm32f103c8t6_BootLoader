#include "led.h"   

//初始化PB5和PE5为输出口.并使能这两个口的时钟		    
//LED IO初始化
void LED_Init(void)
{
	RCC->APB2ENR|=1<<4;    //使能PORTC时钟	   	 
	   	 
	GPIOC->CRH&=0XFF0FFFFF; 
	GPIOC->CRH|=0X00300000;//PC.13 推挽输出
	
  GPIOC->ODR|=1<<13;      //PC.13 输出高
											  
}






