#if defined(__dsPIC33F__)
#include "p33Fxxxx.h"
#endif

#ifndef __MICRODAQV2_H__
#include "MicroDAQv2.h"
#endif

#ifndef __ADIS16334_H__
#include "adis16334.h"
#endif

//functions defined in this file
int spi_transfer(int data);
char get_adis_data(int *adis_data);

//This is a list of some of the registers available on the ADXL345.
//To learn more about these and the rest of the registers on the ADXL345, read the datasheet!

//int adis_data[8];

int spi_transfer(int data)
{
	SPI1BUF = data; //DATA_FORMAT register
	while(SPI1STATbits.SPITBF);//transfer complete
	while(!SPI1STATbits.SPIRBF);
	data = SPI1BUF;
	return data;
}

char get_adis_data(int *adis_data)
{

	int dummy[7];
	
	CS = 0;
	adis_data[7] = spi_transfer(0x3E00);
	adis_data[0] = spi_transfer(0x0000);	//p
	adis_data[1] = spi_transfer(0x0000);	//q
	adis_data[2] = spi_transfer(0x0000);	//r
	adis_data[3] = spi_transfer(0x0000);	//ax
	adis_data[4] = spi_transfer(0x0000);	//ay
	adis_data[5] = spi_transfer(0x0000);	//az
	adis_data[6] = spi_transfer(0x0000);	//T
	CS = 1;
	
	//14 bit data to 16 bit data conversions
	dummy[0] =((adis_data[0] & 0b0010000000000000)<<2);	//p
	if(dummy[0])adis_data[0] = adis_data[0] | 0b1110000000000000;
	else		adis_data[0] = adis_data[0] & 0b0001111111111111;

	dummy[1] =((adis_data[1] & 0b0010000000000000)<<2);	//q
	if(dummy[1])adis_data[1] = adis_data[1] | 0b1110000000000000;
	else		adis_data[1] = adis_data[1] & 0b0001111111111111;
	
	dummy[2] =((adis_data[2] & 0b0010000000000000)<<2);	//r
	if(dummy[2])adis_data[2] = adis_data[2] | 0b1110000000000000; 
	else		adis_data[2] = adis_data[2] & 0b0001111111111111;

	dummy[3] =((adis_data[3] & 0b0010000000000000)<<2);	//ax
	if(dummy[3])adis_data[3] = adis_data[3] | 0b1110000000000000; 
	else		adis_data[3] = adis_data[3] & 0b0001111111111111;

	dummy[4] =((adis_data[4] & 0b0010000000000000)<<2);	//ay
	if(dummy[4])adis_data[4] = adis_data[4] | 0b1110000000000000;
	else		adis_data[4] = adis_data[4] & 0b0001111111111111;
	
	dummy[5] =((adis_data[5] & 0b0010000000000000)<<2);	//az
	if(dummy[5])adis_data[5] = adis_data[5] | 0b1110000000000000;//p 
	else		adis_data[5] = adis_data[5] & 0b0001111111111111;
	
	return 0;

}
