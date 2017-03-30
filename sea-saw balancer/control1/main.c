//****************************************************
// Programmer : Manish Sharma
// Date : 9/13/2011
// Program to synchronize IMU with dsPIC
//****************************************************

//****************************************************
//inclusion of header files
#include<p33FJ128MC804.h>
//****************************************************

#define ONN_TIME 1
#define OFF_TIME 0
#define STRING_SIZE 23
#define CONSTANT 0.005493

unsigned int dummy;
volatile unsigned int start;
volatile unsigned int timer; // in multiples of 20 ms
volatile unsigned int duty_cycle;
volatile unsigned int STRING[STRING_SIZE];
volatile unsigned char counter,counter2=10000;
volatile unsigned char flag;
volatile unsigned int var1=0,var2=0,var3=0; //variables to combine 16 bits
volatile double roll=0.0,pitch=0.0,yaw=0.0;



void __attribute__ ((__interrupt__,__no_auto_psv__)) _T1Interrupt(void)
{
	//TMR1 timer 1 expired
	/* Interrupt Service Routine code goes here */
	if(start == OFF_TIME)// this variable tell if the pulse is high or low
	{
		start = ONN_TIME;
		LATAbits.LATA2 = 1; // blink led this is signal to ESC
		LATBbits.LATB9 = 1; // blink led this is signal to ESC
		PR1 = duty_cycle; //625
	}
	
	if(start == ONN_TIME)
	{
		start = OFF_TIME;
		
		//starting delay.... before anything happens
		if(counter2)
		{
			counter2--;
			LATAbits.LATA2 = 0;	// signal to ESC
			LATBbits.LATB9 = 0;	// signal to ESC
			PR1 = 0x30D4 - duty_cycle; // total time pd will remain constant at 20 ms
			IFS0bits.T1IF = 0; // Clear Timer1 Interrupt Flag
			return;
		}
		//feedback operation starting
		else
		{
			LATAbits.LATA2 = 0;	// signal to ESC
			LATBbits.LATB9 = 0;	// signal to ESC
			PR1 = 0x30D4 - duty_cycle; // total time pd will remain constant at 20 ms
		
			//these 2 variables are used to keep track on received bytes
			counter = 23; // HEX 0x17
			flag = 5;
			//
		
			U1TXREG = '1' ; // Transmit request to send data
			while(counter); // while all bytes are not received
			LATAbits.LATA4 = ~LATAbits.LATA4; // just to indicate synchronization
			LATBbits.LATB9 = ~LATBbits.LATB9; // just to indicate synchronization
			
			//Processing of Data received from IMU
			var1 = (STRING[1]<<8) | STRING[2];
			//var2 = (STRING[3]<<8) | STRING[4];
			//var3 = (STRING[5]<<8) | STRING[6];
	
			// converting to standard outputs	
			roll = var1*CONSTANT;
			//pitch = var2*CONSTANT;
			//yaw = var3*CONSTANT;
			
			if(roll<90)
			{
				//duty_cycle = 0x271 + ((roll/90)*625); //625 initial duty cycle set for 1ms on time
                duty_cycle = 50 + ((roll/90)*625); //625 initial duty cycle set for 1ms on time
				duty_cycle = 1200;
			}
			else
			{
				duty_cycle = 0x250;
				//duty_cycle = 0x200;
			}
		}
		//process data apply control loop;
		//modify dutycycle 625---> 625*2
	}		

	IFS0bits.T1IF = 0; // Clear Timer1 Interrupt Flag
	return;
}

void __attribute__ ((__interrupt__,__no_auto_psv__)) _U1RXInterrupt(void)
{
	//When a character is received
	if(flag)
	{
		while(U1STAbits.URXDA) //their is something to receive
		{
		//dummy=U1RXREG;
		STRING[STRING_SIZE-counter--] = U1RXREG;
		STRING[STRING_SIZE-counter--] = U1RXREG;
		STRING[STRING_SIZE-counter--] = U1RXREG;
		STRING[STRING_SIZE-counter--] = U1RXREG;
		//counter-=4;
		flag-=1;
		//LATAbits.LATA4 = ~LATAbits.LATA4;
		}
	}
	if(flag == 0 && counter ==3)
	{	
		//U1STAbits.URXISEL = 2; // interrupt when 3 characters is received at RX
		while(counter)
		{
			if(U1STAbits.URXDA = 1)
			{
				STRING[STRING_SIZE-counter--] = U1RXREG;
			}
		}
	}

	IFS0bits.U1RXIF = 0; // Clear RX1 Interrupt Flag
	return;
}



int main()
{
	//************************************************
	configure_clock(); //40 Mhz from internal FRC PLL
	//************************************************
	
	//************************************************
	//congifure digital I/O ports
	TRISAbits.TRISA2 = 0;
	TRISAbits.TRISA4 = 0;
	TRISBbits.TRISB9 = 0;
	//************************************************
	
	duty_cycle = 0x271; //625 initial duty cycle set for 1ms on time
		
	configure_PPS();
	configure_uart(); //UART1 enabled uart 1 receive interrupt priority 6
	configure_timer_T1(); // Priority 5 at 50 Hz
	
	while(1);

	return 0;
}