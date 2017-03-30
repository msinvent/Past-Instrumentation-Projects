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


char data_accel[15000];


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

	writeRegister();
	//U2TXREG = '2';
	//while (!U2STAbits.TRMT);//wait while the previous transmission is in progress
	
	//U1TXREG = '1';
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

	//state 1 receiving data from SPI
	while(1)
	{
		
	
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
