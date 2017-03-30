#include<p33FJ128MC804.h>
#include "ADXL.h"

//SPI
int configure_SPI()
{
	/* The following code shows the SPI register configuration for Master mode */
	//interrupt is not used by me
	//IFS0bits.SPI1IF = 0; // Clear the Interrupt Flag
	//IEC0bits.SPI1IE = 0;// Disable the Interrupt 

	// SPI1CON1 Register Settings
	SPI1CON1bits.DISSCK = 0;// Internal Serial Clock is Enabled
	SPI1CON1bits.DISSDO = 0;// SDOx pin is controlled by the module
	SPI1CON1bits.MODE16 = 0;// Communication is word-wide (8 bits)
	SPI1CON1bits.SMP = 0;// Input data is sampled at the middle of data output time
	SPI1CON1bits.CKE = 0;// Serial output data changes on transition 
						// from Idle clock state to active clock state
	SPI1CON1bits.CKP = 0;// Idle state for clock is a low level; 
						// active state is a high level
	SPI1CON1bits.MSTEN = 1;// Master mode Enabled
	SPI1CON1bits.SSEN = 0; // SS1 pin as GPIO
	SPI1CON1bits.PPRE = 1;// Primary prescaler 1:16
	SPI1CON1bits.SPRE = 7;// Primary prescaler 1:1
	TRISCbits.TRISC5 = 0; // CS configured as GP output

	SPI1STATbits.SPIEN = 1;// Enable SPI module

	//SPI1BUF = 0x0000;// Write data to be transmitted 
	
	//interrupt is not used by me
	// Interrupt Controller Settings
	//IFS0bits.SPI1IF = 0; // Clear the Interrupt Flag
	//IEC0bits.SPI1IE = 1;// Enable the Interrupt
	
	return 0;
}

//This function will write a value to a register on the ADXL345.
//Parameters:
//  char registerAddress - The register to write a value to
//  char value - The value to be written to the specified register.
void writeRegister(char registerAddress, char value)
{
	//Set Chip Select pin low to signal the beginning of an SPI packet.
//	CS = LOW;
	/*if(SPI1STATbits.SPIROV==1)
	{	
		SPI1STATbits.SPIROV = 0; //clear receive overflow error flag
		//LED_3 = ~LED_3;
		CS=~CS;
	}*/
	//SPI1BUF = 0x0F;
	int a;
	//SPI1BUF = 0b01010101;
	SPI1BUF = DATA_FORMAT;
	//SPI1BUF = 0b00110001;
	for(a=0;a<10;a++);
	//SPI1BUF = 0b01010101;
	SPI1BUF = value;
	
//	CS = HIGH;
	//while()
	//int a =10;
	//for(a=0;a<20;a++);
	//CS = HIGH;
	//digitalWrite(CS, LOW);
    //Transfer the register address over SPI.
    //SPI.transfer(registerAddress);
    //Transfer the desired register value over SPI.
    //SPI.transfer(value);
    //Set the Chip Select pin high to signal the end of an SPI packet.
    //digitalWrite(CS, HIGH);
}