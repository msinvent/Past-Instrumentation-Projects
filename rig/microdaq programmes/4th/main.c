#include<p33FJ128MC804.h>
#include<stdio.h>

#define DATA_SIZE 23
#define CONST_1 0.005493

int set_clock_settings(void);
int uart_settings(void);
void configure_PPS(void);
void configure_ADC(void);
unsigned int adc_sample(unsigned char channel);
int configure_TIMER_T1(void);

// roll pitch and yaw => rpy
volatile unsigned char ReceivedChar[24];	// input from IMU are saved in these variables
volatile unsigned int counter;			// to keep track of received number of variables
volatile unsigned int flag_0; 			// request to send data from IMU is send
volatile unsigned int flag_3;			// rpy is in updating
volatile unsigned int adc_values[5];	// ADC values are updated in this
volatile unsigned int var1=0,var2=0,var3=0; //variables to combine 16 bits
volatile double roll=0.0,pitch=0.0,yaw=0.0;
volatile char roll_2[20];
volatile char pitch_2[20];
volatile char yaw_2[20];
volatile unsigned char error;



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
	//TMR1 timer 1 expired
	/* Interrupt Service Routine code goes here */
	TMR1 =0x00; // this is not required... but m not sure.
	while (!U1STAbits.TRMT);//wait while the previous transmission is in progress
	PORTAbits.RA4 = ~PORTAbits.RA4;
	U1TXREG = '1' ;
	U1TXREG = '2' ;
 
	//Sample all the ADC pins
	adc_values[0] = adc_sample(4);
	adc_values[1] = adc_sample(5);
	adc_values[2] = adc_sample(6);
	adc_values[3] = adc_sample(7);
	adc_values[4] = adc_sample(8);

	/*
	if(flag_0 == 0) // on slot of data is received...
	//put any processing to be done here before transmiting next
	//request
	{
		//processing code here
		// combining the data of 2 separate bytes
		var1 = (ReceivedChar[1]<<8) | ReceivedChar[2];
		var2 = (ReceivedChar[3]<<8) | ReceivedChar[4];
		var3 = (ReceivedChar[5]<<8) | ReceivedChar[6];
	
		// converting to standard outputs	
		roll = var1*CONST_1;
		pitch = var2*CONST_1;
		yaw = var3*CONST_1;
		//processing code here
		
		U1TXREG = '1' ; // Transmit request to send data		
		flag_0 =1;
		counter = DATA_SIZE;
	}
	*/
	IFS0bits.T1IF = 0; // Clear Timer1 Interrupt Flag
	return;

}
void __attribute__ ((__interrupt__,__no_auto_psv__)) _U1RXInterrupt(void)
{
	//When a character is received
	
	while(U1STAbits.URXDA) //their is something to receive
	{
		ReceivedChar[DATA_SIZE - counter]=U1RXREG;
		counter-=1;
		PORTAbits.RA2 = ~PORTAbits.RA2;
		if(counter==0) //all 23 bytes are received
		{	
			flag_0 =0; //unset flag to say that transfer complete
		}
	}
	
	IFS0bits.U1RXIF = 0; // Clear RX1 Interrupt Flag
	return;
}

void __attribute__ ((__interrupt__,__no_auto_psv__)) _U2RXInterrupt(void)
{
	//When a character is received
	char data;
	int i,j;
	while(U2STAbits.URXDA) // something to receive
	{
		data = U2RXREG;
		if(data == 'S' || data == 's')
		{
			/*
			sprintf(roll_2, "%f", roll);
			sprintf(pitch_2, "%f", pitch);
			sprintf(yaw_2, "%f", yaw);
			for(i=0;i<3;i++)
			{
				U2TXREG = ' ';
				for(j=0;j<6;j++)
				{
					while (!U2STAbits.TRMT);//wait for previous Tx to complete
					if(i==0)
					{
						U2TXREG = roll_2[j] ;
					}
					else if(i==1)
					{
						U2TXREG = pitch_2[j] ;
					}
					else
					{
						U2TXREG = yaw_2[j] ;
					}
				}
				
			}
			*/
			for(i=0;i<23;i++)
			{
				while (!U2STAbits.TRMT);//wait for previous Tx to complete
				U2TXREG = ReceivedChar[i];
			}
			for(j=0;j<5;j++)
			{
				while (!U2STAbits.TRMT);//wait for previous Tx to complete
				U2TXREG = (adc_values[j]>>8);
				while (!U2STAbits.TRMT);//wait for previous Tx to complete
				U2TXREG = adc_values[j];
			}
		//send back data
		}
	}

	IFS1bits.U2RXIF = 0; // Clear RX1 Interrupt Flag
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

int main()
{
	set_clock_settings();
	TRISA = 0;

	delay_ms(50);
	LATAbits.LATA2 = ~LATAbits.LATA2;
	LATAbits.LATA4 = ~LATAbits.LATA4;
	delay_ms(50);
	LATAbits.LATA2 = ~LATAbits.LATA2;
	LATAbits.LATA4 = ~LATAbits.LATA4;
	
	int a=10;
//	disableInterrupts();
	configure_PPS();	//Map the UART pins
	configure_ADC();
	uart_settings();
	configure_TIMER_T1();
	
//	enableInterrupts();

	while(1)
	{
		a++;	// These are dummy instructions
		a=a-1;	// of no use practically	
	}
	return 0;
}
