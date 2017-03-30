#include<p33FJ128MC804.h>
#include "ADXL.h"

#define TIMER_1 1    //1 to enable anyother value to disable
#define SPI 1		//1 to enable anyother value to disable
#define UART_1 1    //1 to enable anyother value to disable
#define UART_2 0    //1 to enable anyother value to disable
#define DMA_0 0   //1 to enable anyother value to disable

_FWDT(FWDTEN_OFF);		//Watchdog Timer Disabled

#define LED_1 LATAbits.LATA2
#define LED_3 LATAbits.LATA4


char data_accel[1000];
char dummy[6];

int x,y,z;

int configure_clock(void);
int configure_PPS(void);
int configure_TIMER_T1(unsigned int interrupt_priority);

//Enable ALL User Interrupts
void enableInterrupts(void)
{
	/* Set CPU IPL to 0, enable level 1-7 interrupts */
	/* No restoring of previous CPU IPL state performed here */
	INTCON1bits.NSTDIS = 0;		//Enable Interrupt Nesting
	SRbits.IPL = 0;
	return;
}
//End enableInterrupts

//Disable ALL User Interrupts
void disableInterrupts(void)
{
	/* Set CPU IPL to 7, disable level 1-7 interrupts */
	/* No saving of current CPU IPL setting performed here */
	SRbits.IPL = 7;
	return;
}
//End disableInterrupts

void __attribute__ ((__interrupt__,__no_auto_psv__)) _T1Interrupt(void)
{
	// Interrupt Service Routine code goes here
	
	LED_1 = ~LED_1;

	//U2TXREG = '2';
	//while (!U2STAbits.TRMT);//wait while the previous transmission is in progress
	
	//U1TXREG = 'm';
	//while (!U1STAbits.TRMT);//wait while the previous transmission is in progress
	
	// Interrupt Service Routine code ends here
	IFS0bits.T1IF = 0; // Clear Timer1 Interrupt Flag
	return;
}

int delay_ms(int time)
{
	int i,j,k;
	for(i=0;i<1000;i++)
	{
		for(j=0;j<1000;j++)
		{
			for(k=0;k<time;k++);
		}
	}
	return 0;
}

void writeRegister(char registerAddress, char value)
{
	//Set Chip Select pin low to signal the beginning of an SPI packet.
	int a;
	CS = LOW;
	SPI1BUF = registerAddress;
	for(a=0;a<30;a++); // delay is required 
	SPI1BUF = value;
	CS = HIGH;
}

void readRegister()
{
	char registerAddress = DATAX0;
	//Since we're performing a read operation, the most significant bit of the register address should be set.
	char address = 0x80 | registerAddress;
	//If we're doing a multi-byte read, bit 6 needs to be set as well.
	address = address | 0x40;
	
	int a;
	CS = LOW;
	SPI1BUF = address;
	for(a=0;a<6;a++)
	{
		//for(a=0;a<30;a++);
		//if(SPI1STATbits.SPIRBF == 1)
		{
			//for(a=0;a<30;a++); // delay is required 
			dummy[a] = SPI1BUF;
			LED_3 = ~LED_3;
		}
	}
	CS = HIGH;
}

//don't modify this function
int configure_BOARD()
{
	//dont remove these configuration functions
	configure_clock();
	configure_PPS();
	//dont remove these configuration functions
	
	//configure GPIO here

	TRISAbits.TRISA2 = 0; // declaring pin as o/p pin
	TRISAbits.TRISA4 = 0; // declaring pin as o/p pin

	delay_ms(20);
	LED_1 = ~LED_1;
	LED_3 = ~LED_3;
	delay_ms(20);
	LED_1 = ~LED_1;
	LED_3 = ~LED_3;
	
	unsigned int PR1,BAUDRATE1,BAUDRATE2;

	disableInterrupts();
	
	//configure DMA here
	if(DMA_0)
	{
		configure_DMA_0();
	}
	//configure INTERRUPT here
	if(TIMER_1)			//If timer_1 is enabled 
	{
		PR1=configure_TIMER_T1(4); //priority = 4
	}
	//configure UART here
	if(UART_1)			//if UART_1 is enabled
	{	
		BAUDRATE1 = configure_UART1(1);//priority = 4
	}
	if(UART_2)			//if UART_1 is enabled
	{	
		BAUDRATE2 = configure_UART2(1);//priotiry = 4
	}
	if(SPI)			//if UART_1 is enabled
	{	
		configure_SPI();;//used in polling mode
	}
	//configure ADC here
	//configure_ADC();
	//configure anyother pheripheral if used here...

	enableInterrupts();
	return 0;
}


int main()
{	
	configure_BOARD();

	//state 1 waiting for SPI to start
/*	while(1)
	{
		
	
	}*/
	//receiving data from SPI
	
  
  	
  	//Put the ADXL345 into +/- 8G range by writing the value 0x01 to the DATA_FORMAT register.
  	writeRegister(DATA_FORMAT, 0x02);
	 //setting data rate to 100 Hz
	writeRegister(BW_RATE, 0x0A); 
  	//Put the ADXL345 into Measurement Mode by writing 0x08 to the POWER_CTL register.
  	writeRegister(POWER_CTL, 0x08);  //Measurement mode  
	
	while(1)
	{

		  //writeRegister(DATA_FORMAT, 0x02);
		  //Reading 6 bytes of data starting at register DATAX0 will retrieve the x,y and z acceleration values from the ADXL345.
		  //The results of the read operation will get stored to the values[] buffer.
		  readRegister();
		
		  //The ADXL345 gives 10-bit acceleration values, but they are stored as bytes (8-bits). To get the full value, two bytes must be combined for each axis.
		  //The X value is stored in values[0] and values[1].
		  x = ((int)dummy[1]<<8)|(int)dummy[0];
		  //The Y value is stored in values[2] and values[3].
		  y = ((int)dummy[3]<<8)|(int)dummy[2];
		  //The Z value is stored in values[4] and values[5].
		  z = ((int)dummy[5]<<8)|(int)dummy[4];
		  
		  //Print the results to the terminal.
		  U1TXREG = dummy[0];
		  U1TXREG = dummy[1];
		  U1TXREG = dummy[2];
		  U1TXREG = dummy[3]; 
		  while (!U1STAbits.TRMT);//wait while the previous transmission is in progress
		  U1TXREG = dummy[4];
		  U1TXREG = dummy[5];
		  while (!U1STAbits.TRMT);//wait while the previous transmission is in progress
	
	}
	
	//state 2 data receive complete... waiting for a signal 
	// to transmit the received data to UART
	while(1)
	{

	}

	//state 3 transmitting data through UART to computer
	while(1)
	{
	}

	return 0;
}
