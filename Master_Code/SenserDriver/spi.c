#include "spi.h"
#include "ssi.h"

void spi_init(void)
{
  ROM_SysCtlPeripheralEnable( SYSCTL_PERIPH_SSI0 );
  ROM_SysCtlPeripheralEnable(SPI0_SYSCTL);
  /*����IO��*/
  ROM_GPIOPinTypeSSI(SPI0_PROT, SPI0_CLK_PIN | SPI0_RX_PIN | SPI0_TX_PIN);
  ROM_GPIOPinConfigure(SPI0_CLK);
  ROM_GPIOPinConfigure(SPI0_RX);
  ROM_GPIOPinConfigure(SPI0_TX);
  /*����SPIʱ��Ϊ1Mhz*/
  ROM_SSIConfigSetExpClk(SPI0_BASE, ROM_SysCtlClockGet(), SSI_FRF_MOTO_MODE_3,  SSI_MODE_MASTER, 10000000,  8);
  /*����SSI0*/
  ROM_SSIEnable(SPI0_BASE);

  ROM_SysCtlPeripheralEnable(FLASH_CSPIN_SYSCTL);
  ROM_GPIOPinTypeGPIOOutput(FLASH_CS_PORT, FLASH_CS_PIN);
  ROM_GPIOPinWrite(FLASH_CS_PORT, FLASH_CS_PIN, FLASH_CS_PIN);
	
	
  icm20602_pin_init();
}

/* SPI��д���� */
uint8_t spi_single_send(uint8_t SendData)
{
  uint32_t ui_TempData;
  uint8_t uc_ReceiveData;
  /* ��SSI FIFOд������ */
  ROM_SSIDataPut(SPI0_BASE, SendData);

  /* �ȴ�SSI��æ */
  while(ROM_SSIBusy(SPI0_BASE));

  /* ��FIFO��ȡ���� */
  ROM_SSIDataGet(SPI0_BASE, &ui_TempData);
  /* ��ȡ���ݵĵͰ�λ */
  uc_ReceiveData = ui_TempData & 0xff;
  return uc_ReceiveData;
}

/* ����д������ */
void spi_transmit(uint8_t *data, uint16_t len)
{
  for(uint16_t i = 0; i < len; i++) {
    spi_single_send(data[i]);
  }
}

/* ������ȡ���� */
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


