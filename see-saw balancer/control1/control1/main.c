//****************************************************
// Programmer : Manish Sharma
// Date : 9/13/2011
// Program to synchronize IMU with dsPIC
//****************************************************

//****************************************************
//inclusion of header files
#include<p33FJ128MC804.h>
//****************************************************

int configure_clock(void);
int configure_timer_T1(void);
int configure_uart(void);
void configure_PPS(void);

#define STRING_SIZE 23
#define IMU_CONSTANT 0.005493

#define LED_1 LATAbits.LATA2
#define LED_2 LATAbits.LATA4
//#define LED_3 

volatile unsigned int STRING[STRING_SIZE];

//used in IMU data recovery tracking
volatile unsigned int counter;
volatile unsigned char flag;

//will be used to calculate the physical values of IMU data
volatile unsigned int var1=0,var2=0,var3=0; //variables to combine 16 bits
volatile double roll=0.0,pitch=0.0,yaw=0.0;


void __attribute__ ((__interrupt__,__no_auto_psv__)) _T1Interrupt(void)
{
	// Interrupt Service Routine code goes here
	
		LED_1 = ~LED_1;
		LED_2 = ~LED_2;
		U1TXREG = '1' ; // Transmit request to send data
	
	// Interrupt Service Routine code ends here

	IFS0bits.T1IF = 0; // Clear Timer1 Interrupt Flag
	return;
}

/*
void __attribute__ ((__interrupt__,__no_auto_psv__)) _U1RXInterrupt(void)
{
	//When a character is received
	if(flag) //and counter-3 %4 ==0 for error detection
	{
		while(U1STAbits.URXDA) //their is something to receive
		{
			STRING[STRING_SIZE-counter--] = U1RXREG;
			STRING[STRING_SIZE-counter--] = U1RXREG;
			STRING[STRING_SIZE-counter--] = U1RXREG;
			STRING[STRING_SIZE-counter--] = U1RXREG;
			flag-=1;
		}
	}
	if(flag == 0 && counter ==3) //for error detection
	{	
		//U1STAbits.URXISEL = 2; // interrupt when 3 characters is received at RX
		while(counter)
		{
			if(U1STAbits.URXDA = 1)
			{
				STRING[STRING_SIZE-counter--] = U1RXREG;
			}
		}
		counter = 0;
	}

	IFS0bits.U1RXIF = 0; // Clear RX1 Interrupt Flag
	return;
}
*/

int main()
{
	int a = 10;
	//************************************************
	configure_clock(); //40 Mhz from internal FRC PLL
	//************************************************
	
	//************************************************
	//congifure digital I/O ports
	TRISAbits.TRISA2 = 0;
	TRISAbits.TRISA4 = 0;
	
	//************************************************
			
	configure_PPS();
	configure_uart(); //UART1 enabled uart 1 receive interrupt priority 6
	configure_timer_T1(); // Priority 5 at 50 Hz
	
	while(1);

	return 0;
}
