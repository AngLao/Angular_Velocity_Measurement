#include "FreeRTOS.h"
#include "task.h"

#include "Drv_Bsp.h"
#include "mavlink.h"

mavlink_system_t mavlink_system = {
    2, // System ID (1-255)
    0x66  // Component ID (a MAV_COMPONENT value)
};


struct icm20602_dev icm20602_config = { .id = 0, \
																				.hal_wr = icm20602_write, \
																				.hal_rd = icm20602_read, \
																				.hal_sleep = vTaskDelay, \
																				.mutex_lock = NULL, \
																				.mutex_unlock = NULL, \
																				.use_accel = true, \
																				.accel_fifo = false, \
																				.accel_dlpf = ICM20602_ACCEL_DLPF_10_2_HZ, \
																				.accel_g = ICM20602_ACCEL_RANGE_4G, \
																				.use_gyro = true, \
																				.gyro_fifo = false, \
																				.gyro_dlpf = ICM20602_GYRO_DLPF_10_HZ, \
																				.gyro_dps = ICM20602_GYRO_RANGE_2000_DPS, \
																				.sample_rate_div = 0, \
																				.i2c_disable = true, };

/* 与上位机通讯进程 */
void send_data(void *pvParameters)
{
  TickType_t xLastWakeTime = xTaskGetTickCount(); //获取当前Tick次数,以赋给延时函数初值

	icm20602_init(&icm20602_config);
  while (1) {
		mavlink_attitude_t 	imu_data;
		mavlink_raw_imu_t 	imu_raw_data;

		mavlink_message_t 	imu_data_msg;
		mavlink_message_t 	imu_raw_data_msg;
    /*读取icm20602原始寄存器数据*/
		icm20602_read_data_raw(&icm20602_config, 
													 &imu_raw_data.xacc, &imu_raw_data.yacc, &imu_raw_data.zacc, 
													 &imu_raw_data.xgyro, &imu_raw_data.ygyro, &imu_raw_data.zgyro,
													 &imu_raw_data.temperature);
		
		mavlink_msg_raw_imu_encode_chan(mavlink_system.sysid, mavlink_system.compid,MAVLINK_COMM_0,
														   &imu_raw_data_msg,&imu_raw_data);
		
    /*读取icm20602角速度（rad/s）数据*/
		icm20602_read_gyro(&icm20602_config, 
													 &imu_data.pitchspeed, &imu_data.rollspeed, &imu_data.yawspeed);
    
		
		mavlink_msg_attitude_encode_chan(mavlink_system.sysid, mavlink_system.compid,MAVLINK_COMM_0,
														   &imu_data_msg,&imu_data);
		
    /*虚拟串口发送数据*/
		uint8_t buf[100] = {0};
		uint16_t len = mavlink_msg_to_send_buffer(buf, &imu_data_msg);
		AnoUsbCdcSend(buf, len);
		
    vTaskDelayUntil(&xLastWakeTime, configTICK_RATE_HZ / 100);
  }
}
 
void led_task(void *pvParameters)
{
  TickType_t xLastWakeTime = xTaskGetTickCount(); //获取当前Tick次数,以赋给延时函数初值

  while (1) {
		static uint8_t ledNum = 1;
		
		Drv_LedOnOff(ledNum, 1);
		
		if(ledNum < LED_B)
			ledNum <<= 1;
		else
			ledNum = 1;
		
		Drv_LedOnOff(ledNum, 0);
 
		
    vTaskDelayUntil(&xLastWakeTime, configTICK_RATE_HZ / 3);
  }
}

int main(void)
{ 
  /* 驱动初始化 */
	Drv_BspInit();
	
  /* 基本传感器数据准备进程 */
  xTaskCreate(send_data, "send_data", 400, NULL, 4, NULL);
	
  /* led_task */
  xTaskCreate(led_task, "led_task", 100, NULL, 3, NULL);

  /*--------------------------------------------------------
  									启用任务调度器
  --------------------------------------------------------*/
  vTaskStartScheduler();



  /*--------------------------------------------------------
  									溢出处理
  --------------------------------------------------------*/
  //printf("Free_Heap_Size = %d \n",xPortGetFreeHeapSize());
  //printf("MinimumEverFreeHeapSize = %d \n",xPortGetMinimumEverFreeHeapSize());
}
