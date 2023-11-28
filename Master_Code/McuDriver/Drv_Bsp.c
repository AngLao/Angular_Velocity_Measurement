#include "Drv_Bsp.h"


static void SysTick_Init(void )
{
  ROM_SysTickPeriodSet(ROM_SysCtlClockGet() / 1000);
  ROM_SysTickIntEnable();
  ROM_SysTickEnable();
}

//声明freeRTOS滴答回调
void xPortSysTickHandler( void );

void SysTick_Handler(void)
{
  if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED) 
		xPortSysTickHandler();
}
   

void Drv_BspInit(void)
{ 
	//寄存器值非默认值就进行软件复位
  if(ROM_SysCtlClockGet() != 16000000 )
    ROM_SysCtlReset();
	
  /*设置系统主频为80M*/
  ROM_SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

  /*中断优先级组别设置*/
  NVIC_SetPriorityGrouping(0x03);
	
  /*开启浮点运算单元*/
  ROM_FPULazyStackingEnable();
  ROM_FPUEnable();
	
  //灯光初始化
  Dvr_LedInit(); 

  //spi通信初始化
  spi_init(); 
	
  //板载USB虚拟串口初始化
  AnoUsbCdcInit();
	
  //滴答时钟初始化
  SysTick_Init(); 
	
	
}




