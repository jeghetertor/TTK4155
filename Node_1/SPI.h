#ifndef SPI_H_
#define SPI_H_
#endif
void SPI_init();
void SPI_send(uint8_t cData);
uint8_t SPI_read();
void ss_disable();
void ss_enable();
void test_SPI();