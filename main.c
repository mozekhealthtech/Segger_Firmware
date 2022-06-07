
#include "nrf_drv_spi.h"
#include "app_util_platform.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"
#include "boards.h"
#include "app_error.h"
#include <string.h>
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"


#define ADD_REG_WHO_AM_I            0x90
#define UC_WHO_AM_I_DEFAULT_VALUE   0X00


#define SPI_BUFSIZE  8
#define SPI_INSTANCE 0

#define SET_READ_SINGLE_CMD(x)  (x | 0x4B)

uint8_t spi_tx_buf[SPI_BUFSIZE];
uint8_t spi_rx_buf[SPI_BUFSIZE];


static volatile bool spi_xfer_done;


static const nrf_drv_spi_t m_spi = NRF_DRV_SPI_INSTANCE(SPI_INSTANCE);



// this is the event handler

void spi_event_handler(nrf_drv_spi_evt_t const * p_event, void * p_context)
{
    spi_xfer_done = true;
}


void spi_init(void)
{
  nrf_drv_spi_config_t spi_config = NRF_DRV_SPI_DEFAULT_CONFIG;

  spi_config.ss_pin     = 27;
  spi_config.miso_pin   = 26;
  spi_config.mosi_pin   = 25;
  spi_config.sck_pin    = 24;
  spi_config.frequency  = NRF_DRV_SPI_FREQ_4M;

  APP_ERROR_CHECK(nrf_drv_spi_init(&m_spi, &spi_config, spi_event_handler, NULL));

}



int LIS3DSH_read_reg(int reg)
{

    spi_tx_buf[0] = SET_READ_SINGLE_CMD(reg);
    spi_xfer_done = false;

    APP_ERROR_CHECK(nrf_drv_spi_transfer(&m_spi, spi_tx_buf, 2, spi_rx_buf, 2));

    while(spi_xfer_done == false){};

    return spi_rx_buf[1];
}





int main(void)
{


    int intRegValue;

    APP_ERROR_CHECK(NRF_LOG_INIT(NULL));
    NRF_LOG_DEFAULT_BACKENDS_INIT();

    spi_init();

    NRF_LOG_INFO("SPI Application started");

    intRegValue = LIS3DSH_read_reg(ADD_REG_WHO_AM_I);

    if(intRegValue == UC_WHO_AM_I_DEFAULT_VALUE)
    {
      NRF_LOG_INFO("The WHO_AM_I value is correct for lis3dsh...");
    }
   
    while (1)
    {
       
    }
}