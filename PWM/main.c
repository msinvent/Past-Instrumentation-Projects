// PID control main function
// this program is giving some satisfactory performance

#include<p33FJ128MC804.h>

//Peripheral declaration...
	
#define TIMER_1 1    //1 to enable anyother value to disable
#define UART_1 0    //1 to enable anyother value to disable
#define UART_2 0    //1 to enable anyother value to disable
#define DMA_0 0    //1 to enable anyother value to disable

#define LED_1 LATAbits.LATA2
#define LED_3 LATAbits.LATA4



int configure_clock(void);
int configure_PPS(void);

int configure_TIMER_T1(unsigned int interrupt_priority);
int configure_uart1(unsigned char);
int configure_uart2(unsigned char);
int configure_DMA_0(void);
int configure_PWM(void);

int configure_ADC(void);


int delay_ms(int);


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
	/*
	U2TXREG = '2';
	while (!U2STAbits.TRMT);//wait while the previous transmission is in progress
	*/
	U1TXREG = '1';
	while (!U1STAbits.TRMT);//wait while the previous transmission is in progress
	
	//P1DC1=P1DC1-10;	//50%
	//P1DC2=P1DC2-10;	//50%	
	//P1DC3=P1DC3-10;	//50%
	
	// Interrupt Service Routine code ends here
	IFS0bits.T1IF = 0; // Clear Timer1 Interrupt Flag
	return;
}

/*
void __attribute__ ((__interrupt__,__no_auto_psv__)) _U1RXInterrupt(void)
{
	//When a character is received
	
	char data;
	while(U1STAbits.URXDA) //their is something to receive
	{
		data = U1RXREG;
		U1TXREG = 'a';
		//LED_3 = ~LED_3;
	}
	
	IFS0bits.U1RXIF = 0; // Clear RX1 Interrupt Flag
	return;
}
void __attribute__ ((__interrupt__,__no_auto_psv__)) _U2RXInterrupt(void)
{
	//When a character is received
	char data;
	while(U2STAbits.URXDA) // something to receive
	{
		data = U2RXREG;
		U2TXREG = 'z';
		LED_3 = ~LED_3;
	}

	IFS1bits.U2RXIF = 0; // Clear RX1 Interrupt Flag
	return;
}
*/

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

//this function is not for modification..
int configure_BOARD()
{
	//dont remove these configuration functions
	configure_clock();
	configure_PPS();
	//dont remove these configuration functions
	
	//configure GPIO here
	TRISAbits.TRISA2 = 0;
	TRISAbits.TRISA4 = 0;

	//FWDTbits.FWDTEN = 0;
	//I was trying to disable watchdog timer...

	delay_ms(20);
	LED_1 = ~LED_1;
	LED_3 = ~LED_3;
	delay_ms(20);
	LED_1 = ~LED_1;
	LED_3 = ~LED_3;
	
	unsigned int PR1,BAUDRATE1,BAUDRATE2;

	disableInterrupts();

	if(DMA_0 ==1)
	{
		configure_DMA_0();
	}
	//DMA_1(); disabled.... by me cant be enabled bcoz even the function is disabled
	
	//configure INTERRUPT here
	if(TIMER_1==1)			//If timer_1 is enabled 
	{
	PR1=configure_TIMER_T1(4); //priority = 4
	}
	
	//configure UART here
	if(UART_1==1)			//if UART_1 is enabled
	{	
	BAUDRATE1 = configure_uart1(1);//priority = 4
	}
	if(UART_2==1)			//if UART_1 is enabled
	{	
	BAUDRATE2 = configure_uart2(1);//priotiry = 4
	}
	//configure PWM
	configure_PWM();

	//configure ADC here
	//configure_ADC();
	//configure anyother pheripheral if used here...

	enableInterrupts();
	return 0;
}


int main()
{	
	configure_BOARD();

	while(1)
	{
	int i = 3;
	i=i;
	//P2DC1=P1DC2;
	}

	return 0;
}
