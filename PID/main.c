// PID control main function
// this program is giving some satisfactory performance

#include<p33FJ128MC804.h>


#define LED_1 LATAbits.LATA2
#define LED_2 LATAbits.LATA4

/***********************************************
variable declarations
***********************************************/
volatile int duty_cycle=0x40;//this is a representation
// of duty cycle and not exact duty cycle

//end of declaration



int configure_clock(void);
int configure_uart(void);
int configure_PPS(void);
int configure_TIMER_T1(void);

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

/*
int CONTROL_TEST()
{
	//offyougo=int(5000*angle/180)+500
	duty_cycle=duty_cycle+4;
	//duty_cycle=duty_cycle-4;
	if(duty_cycle < 1600)
	duty_cycle =1600;
	else if(duty_cycle > 4000)
	duty_cycle =4000;
	else	// do nothing--
	return 0;
}
*/
/*
void motor_control(int a )
{
	a=10;
	// this PWM is actually PID_out and in my point of view need to be
	// normalized accordingly
	U2TXREG = 0x80; // start byte
	U2TXREG = 0x01; // Device ID which is 0x01 for 8 servo controller
	U2TXREG = 0x04; // configuration for forward movement
	U2TXREG = 0x00; // Servo 0 selected
	while (!U2STAbits.TRMT);
	//Get the highest 7 bits
  	U2TXREG=duty_cycle>>7;
  	//get the lowest 7 bits
	U2TXREG=duty_cycle&127;
  	
	while (!U2STAbits.TRMT);//wait while the previous transmission is in progress

	return;
}
*/ 
void __attribute__ ((__interrupt__,__no_auto_psv__)) _T1Interrupt(void)
{

	//Frequency of this interrupt being called is set by registerb PR1 in
	//configure_TIMER_T1()
	//currently it is set to 0x0C35*2 which gives 25 Hz frequency to the Autopilot
	LED_1 = ~LED_1;
	// Interrupt Service Routine code goes here
		
	//everything sud happen in this time span of 1/25 seconds
	//when I said everything i mean data ac  

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


int configure_BOARD()
{
	//configure PPI
	configure_PPS();

	//configure GPIO here
	TRISAbits.TRISA2 = 0;
	TRISAbits.TRISA4 = 0;
	delay_ms(20);
	LED_1 = ~LED_1;
	LED_2 = ~LED_2;
	delay_ms(20);
	LED_1 = ~LED_1;
	//LED_2 = ~LED_2;
	
	disableInterrupts();

	//configure ADC here
	configure_ADC();

	//configure INTERRUPT here
	configure_TIMER_T1();
	
	//configure UART here
	configure_uart();

	enableInterrupts();
	return 0;
}


int main()
{	

	configure_clock();
	configure_BOARD();

	while(1)
	{
	}

	return 0;
}