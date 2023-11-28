#include "Drv_Bsp.h"


static void SysTick_Init(void )
{
  ROM_SysTickPeriodSet(ROM_SysCtlClockGet() / 1000);
  ROM_SysTickIntEnable();
  ROM_SysTickEnable();
}

//����freeRTOS�δ�ص�
void xPortSysTickHandler( void );

void SysTick_Handler(void)
{
  if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED) 
		xPortSysTickHandler();
}
   

void Drv_BspInit(void)
{ 
	//�Ĵ���ֵ��Ĭ��ֵ�ͽ��������λ
  if(ROM_SysCtlClockGet() != 16000000 )
    ROM_SysCtlReset();
	
  /*����ϵͳ��ƵΪ80M*/
  ROM_SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

  /*�ж����ȼ��������*/
  NVIC_SetPriorityGrouping(0x03);
	
  /*�����������㵥Ԫ*/
  ROM_FPULazyStackingEnable();
  ROM_FPUEnable();
	
  //�ƹ��ʼ��
  Dvr_LedInit(); 

  //spiͨ�ų�ʼ��
  spi_init(); 
	
  //����USB���⴮�ڳ�ʼ��
  AnoUsbCdcInit();
	
  //�δ�ʱ�ӳ�ʼ��
  SysTick_Init(); 
	
	
}




