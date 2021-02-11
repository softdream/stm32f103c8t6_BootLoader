本实验将实现如下功能:开机的时候先显示提示信息，然后等待串口输入接收APP程序（无校验，一次性接收），在串口接收到APP程序之后，即可执行IAP。如果是SRAM APP，通过按下KEY0即可执行这个收到的SRAM APP程序。如果是FLASH APP，则需要先按下WK_UP按键，将串口接收到的APP程序存放到STM32的FLASH，之后再按KEY2既可以执行这个FLASH APP程序。通过KEY1按键，可以手动清除串口接收到的APP程序。


注意：SRAM APP代码的起始地址必须是：0X20001000，FLASH APP代码的起始地址必须是：0X08005000。

               	正点原子@ALIENTEK
               	2012-9-24
		广州市星翼电子科技有限公司
                联系电话（传真）：020-38271790
	       	购买：http://shop62103354.taobao.com
                      http://shop62057469.taobao.com
               	技术支持论坛：www.openedv.com