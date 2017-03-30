#if defined(__dsPIC33F__)
#include "p33Fxxxx.h"
#endif

#ifndef __MICRODAQV2_H__
#include "MicroDAQv2.h"
#endif

#ifndef __ADXL_H__
#include "Adxl.h"
#endif

char SPI_transfer(char data);
void cfgAdxl(void);
extern char getdata(char);
char getdata(char);

//This is a list of some of the registers available on the ADXL345.
//To learn more about these and the rest of the registers on the ADXL345, read the datasheet!
char POWER_CTL = 0x2D;	//Power Control Register
char DATA_FORMAT = 0x31;
char DATAX0 = 0x32;	//X-Axis Data 0
char DATAX1 = 0x33;	//X-Axis Data 1
char DATAY0 = 0x34;	//Y-Axis Data 0
char DATAY1 = 0x35;	//Y-Axis Data 1
char DATAZ0 = 0x36;	//Z-Axis Data 0
char DATAZ1 = 0x37;	//Z-Axis Data 1 

int a = 0xABCD;

char adxl_data[11];
	
long x,y,z;

char Spi_transfer(char data)
{
	SPI1BUF = data; //DATA_FORMAT register
	while(SPI1STATbits.SPITBF);//transfer complete
	while(!SPI1STATbits.SPIRBF);
	data = SPI1BUF;
	return data;
}

void cfgAdxl(void)
{
	unsigned long i=0;
	for(i=0;i<60000;i++);//DELAY

	CS = LOW;
	adxl_data[0] = Spi_transfer(0x31);
	adxl_data[1] = Spi_transfer(0x0B);//+-16g 13 bit
	CS = HIGH;

	for(i=0;i<60000;i++); //delay
	
	CS = LOW;
	adxl_data[2] = Spi_transfer(0x2C);//BW_RATE
	adxl_data[3] = Spi_transfer(0x0D);//800 Hz
	CS = HIGH;
	
	for(i=0;i<60000;i++); //delay
	
	CS = LOW;
	adxl_data[2] = Spi_transfer(0x2E);//interrupt
	adxl_data[3] = Spi_transfer(0x80);//data ready
	CS = HIGH;
	
	for(i=0;i<60000;i++); //delay
	
	CS = LOW;
	adxl_data[2] = Spi_transfer(0x2D);//POWER_CTL
	adxl_data[3] = Spi_transfer(0x08);//Measurement Mode
	CS = HIGH;

}

char getdata(char register_address)
{
	long i;
	
	CS = LOW;
	char address = 0x32 | 0x80 | 0x40;
	for(i=0;i<20;i++);
	//while(INT1);
	adxl_data[4]=Spi_transfer(address);
	adxl_data[5]=Spi_transfer(0x00);
	adxl_data[6]=Spi_transfer(0x00);
	adxl_data[7]=Spi_transfer(0x00);
	adxl_data[8]=Spi_transfer(0x00);
	adxl_data[9]=Spi_transfer(0x00);
	adxl_data[10]=Spi_transfer(0x00);

	CS = HIGH;
	
	LED_1= ~LED_1;
	LED_3= ~LED_1;
	x = ((int)adxl_data[6]<<8)|(int)adxl_data[5];
  	//The Y value is stored in values[2] and values[3].
  	y = ((int)adxl_data[8]<<8)|(int)adxl_data[7];
  	//The Z value is stored in values[4] and values[5].
  	z = ((int)adxl_data[10]<<8)|(int)adxl_data[9];
	
	U1TXREG = 0;
	U1TXREG = 0;
	
	//x = 0xABCD;
	x = (int)(x*0.39);
	U1TXREG = x;
	//U1TXREG = 0;
	U1TXREG = ((x>>8));
	//U1TXREG = 0;
	while (!U1STAbits.TRMT);//wait while the previous transmission is in progress
	
	y = (int)(y*0.39);
	U1TXREG = y;
	//U1TXREG = 0;	
	U1TXREG = ((y>>8));
	//U1TXREG = 0;
	z = (int)(z*0.39);
	U1TXREG = z;
	//U1TXREG = 0;
	U1TXREG = ((z>>8));
	//U1TXREG = 0;
	while (!U1STAbits.TRMT);//wait while the previous transmission is in progress
	
	return adxl_data[5];
}

char getdatacheck(char register_address)
{
	long i;
	char address = 0x00 | 0x80;
	CS = LOW;
	
	for(i=0;i<260;i++);

	adxl_data[4] = Spi_transfer(address);

	adxl_data[5] = Spi_transfer(0x00);

	CS = HIGH;
	
	LED_1= ~LED_1;
	LED_3= ~LED_1;
	
	return adxl_data[5];
}
//manish sharma 24 feb