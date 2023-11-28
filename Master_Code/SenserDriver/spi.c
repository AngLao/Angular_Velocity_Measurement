#include "spi.h"
#include "ssi.h"

void spi_init(void)
{
  ROM_SysCtlPeripheralEnable( SYSCTL_PERIPH_SSI0 );
  ROM_SysCtlPeripheralEnable(SPI0_SYSCTL);
  /*配置IO口*/
  ROM_GPIOPinTypeSSI(SPI0_PROT, SPI0_CLK_PIN | SPI0_RX_PIN | SPI0_TX_PIN);
  ROM_GPIOPinConfigure(SPI0_CLK);
  ROM_GPIOPinConfigure(SPI0_RX);
  ROM_GPIOPinConfigure(SPI0_TX);
  /*配置SPI时钟为1Mhz*/
  ROM_SSIConfigSetExpClk(SPI0_BASE, ROM_SysCtlClockGet(), SSI_FRF_MOTO_MODE_3,  SSI_MODE_MASTER, 10000000,  8);
  /*开启SSI0*/
  ROM_SSIEnable(SPI0_BASE);

  ROM_SysCtlPeripheralEnable(FLASH_CSPIN_SYSCTL);
  ROM_GPIOPinTypeGPIOOutput(FLASH_CS_PORT, FLASH_CS_PIN);
  ROM_GPIOPinWrite(FLASH_CS_PORT, FLASH_CS_PIN, FLASH_CS_PIN);
	
	
  icm20602_pin_init();
}

/* SPI读写函数 */
uint8_t spi_single_send(uint8_t SendData)
{
  uint32_t ui_TempData;
  uint8_t uc_ReceiveData;
  /* 向SSI FIFO写入数据 */
  ROM_SSIDataPut(SPI0_BASE, SendData);

  /* 等待SSI不忙 */
  while(ROM_SSIBusy(SPI0_BASE));

  /* 从FIFO读取数据 */
  ROM_SSIDataGet(SPI0_BASE, &ui_TempData);
  /* 截取数据的低八位 */
  uc_ReceiveData = ui_TempData & 0xff;
  return uc_ReceiveData;
}

/* 连续写入数据 */
void spi_transmit(uint8_t *data, uint16_t len)
{
  for(uint16_t i = 0; i < len; i++) {
    spi_single_send(data[i]);
  }
}

/* 连续读取数据 */
void spi_receive(uint8_t *data, uint16_t len)
{
  for(uint16_t i = 0; i < len; i++) {
    data[i] = spi_single_send(0xFF);
  }
}

#define ICM20602_ENABEL()    ROM_GPIOPinWrite(ICM20602_CS_PORT, ICM20602_CS_PIN, 0);
#define ICM20602_DISABEL()   ROM_GPIOPinWrite(ICM20602_CS_PORT, ICM20602_CS_PIN, ICM20602_CS_PIN);

void icm20602_pin_init(void)
{
  ROM_SysCtlPeripheralEnable(ICM_CSPIN_SYSCTL);
  ROM_GPIOPinTypeGPIOOutput(ICM20602_CS_PORT, ICM20602_CS_PIN);
  ROM_GPIOPinWrite(ICM20602_CS_PORT, ICM20602_CS_PIN, ICM20602_CS_PIN);
}

int8_t icm20602_read(uint8_t id, uint8_t reg, uint8_t * data, uint16_t len)
{
  ICM20602_ENABEL();
  spi_single_send(reg | 0x80);
  spi_receive(data, len);
  ICM20602_DISABEL();
	
  return 0;
}

int8_t icm20602_write(uint8_t id, uint8_t reg, uint8_t * data, uint16_t len)
{
  ICM20602_ENABEL();
  spi_single_send(reg);
  spi_transmit(data, len);
  ICM20602_DISABEL();
	
  return 0;
} 


