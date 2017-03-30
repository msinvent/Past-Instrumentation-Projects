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

char IMU_data[16000];

int a = 0xABCD;

char adxl_data[11];

int x,y,z;

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
	adxl_data[3] = Spi_transfer(0x0C);//400 Hz
	CS = HIGH;
	
/*	for(i=0;i<60000;i++); //delay
	
	CS = LOW;
	adxl_data[2] = Spi_transfer(0x2E);//interrupt
	adxl_data[3] = Spi_transfer(0x80);//data ready
	CS = HIGH;
*/	
	for(i=0;i<60000;i++); //delay
	
	CS = LOW;
	adxl_data[2] = Spi_transfer(0x2D);//POWER_CTL
	adxl_data[3] = Spi_transfer(0x08);//Measurement Mode
	CS = HIGH;

}

char getdata(char register_address)
{
	long i;
	i=0;
	CS = LOW;
	char address = 0x32 | 0x80 | 0x40;
	static int count2 = 0;
	
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
	
	//x = (unsigned int)adxl_data[6]<<8 |(unsigned char)adxl_data[5];
	//y = (unsigned int)adxl_data[8]<<8 | (unsigned char)adxl_data[7];
	//z = (unsigned int)adxl_data[10]<<8 | (unsigned char)adxl_data[9];
  	
	IMU_data[count2*6+0]= adxl_data[5];
	IMU_data[count2*6+1]= adxl_data[6];
	IMU_data[count2*6+2]= adxl_data[7];
	IMU_data[count2*6+3]= adxl_data[8];
	IMU_data[count2*6+4]= adxl_data[9];
	IMU_data[count2*6+5]= adxl_data[10];
	
	if(count2 < 2600)
	count2 = count2+1;
	
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

char ff_detect()
{

	CS = LOW;
	char address = 0x32 | 0x80 | 0x40;
	
	adxl_data[4]=Spi_transfer(address);
	adxl_data[5]=Spi_transfer(0x00);
	adxl_data[6]=Spi_transfer(0x00);
	adxl_data[7]=Spi_transfer(0x00);
	adxl_data[8]=Spi_transfer(0x00);
	adxl_data[9]=Spi_transfer(0x00);
	adxl_data[10]=Spi_transfer(0x00);

	CS = HIGH;
	
	x = (unsigned int)adxl_data[6]<<8 |(unsigned char)adxl_data[5];
	y = (unsigned int)adxl_data[8]<<8 | (unsigned char)adxl_data[7];
	z = (unsigned int)adxl_data[10]<<8 | (unsigned char)adxl_data[9];
	
	if(((x<25)&&(x>-25)) && ((y<25)&&(y>-25)) && ((z<25)&&(z>-25)))
	{
		return 1;
	}
	
	return 0;
}
//manish sharma 25 feb
