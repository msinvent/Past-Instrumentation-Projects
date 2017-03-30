//Programmer : Manish Sharma
//Date : 25/02/2012
//Place : IIT-KANPUR
//Things left : give some delay in the beginning and attach buzzer

#if defined(__dsPIC33F__)
#include "p33Fxxxx.h"
#endif

// have definitions of clock settings and Peripheral Pin mapping
#include "MicroDAQv2.h"
// have declarations and definitions of Peripherals used
#include "Adxl.h"
// have declarations and definitions of Peripherals used
#include "Peripherals.h"


//Enable ALL User Interrupts
void enableInterrupts(void)
{
	/* Set CPU IPL to 0, enable level 1-7 interrupts */
	/* No restoring of previous CPU IPL state performed here */
	INTCON1bits.NSTDIS = 1;		//Disable Interrupt Nesting
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

int main()
{

	unsigned long i,j;

	cfgClock();
	cfgPps();
	
	//disableInterrupts();

	cfgUart1(4);

	//cfgTimerT1(4);
	//cfgUart2(4);
	//cfgDma0Uart1Rx();
	//cfgDma1Uart2Rx();
	//cfgDma2Spi1Rx();	

	cfgSpi1();
	cfgAdxl();

	//enableInterrupts();
	
	int count1=2600;//2600 sample points will be take
	//initial delay
/* UART Test
	while(1)
	{
	
		LED_1 = ~LED_1;
		LED_3 = ~LED_1;
		U1TXREG = 0xAA;
		U1TXREG = 0xAA;
		U1TXREG = 0xAA;
		U1TXREG = 0xAA;

		while (!U1STAbits.TRMT);//wait while the previous transmission is in progress
	}
	//Uart Test End
*/ 
	
	//logging loop
	while(count1) //background loop
	{
		for(i=0;i<3450;i++);//800 Hz calibrated approximate +-10 Hz
		getdatacheck(0x00);
		//getdata(0x00);
		count1--;
		//data1=getdatacheck(0x32);
	}

	//waiting loop
	while(TRANSFER)
	{
		LED_1 = 0;
		LED_3 = 0;
		for(i=0;i<60000;i++);
		for(i=0;i<60000;i++);
		LED_1 = 1;
		LED_3 = 1;
		for(i=0;i<60000;i++);
		for(i=0;i<60000;i++);
	}	

	//transfer loop	
	for(i=0;i<2600;i++)
	{
		U1TXREG = 0x00;
		U1TXREG = 0x00;
		while (!U1STAbits.TRMT);//wait while the previous transmission is in progress
		U1TXREG = IMU_data[i*6+0];
		U1TXREG = IMU_data[i*6+1];
		while (!U1STAbits.TRMT);//wait while the previous transmission is in progress
		U1TXREG = IMU_data[i*6+2];
		U1TXREG = IMU_data[i*6+3];
		while (!U1STAbits.TRMT);//wait while the previous transmission is in progress
		U1TXREG = IMU_data[i*6+4];
		U1TXREG = IMU_data[i*6+5];
		while (!U1STAbits.TRMT);//wait while the previous transmission is in progress
		
	}
	
	//transfer complete loop
	while(1)
	{
		LED_1 = ~LED_1;
		LED_3 = ~LED_1;
		for(i=0;i<60000;i++);
		for(i=0;i<60000;i++);
		for(i=0;i<60000;i++);
		for(i=0;i<60000;i++);
		LED_1 = ~LED_1;
		LED_3 = ~LED_1;
		for(i=0;i<60000;i++);
		for(i=0;i<60000;i++);
		for(i=0;i<60000;i++);
		for(i=0;i<60000;i++);
	}	

	return 0;
}
