// Programmer : Manish Sharma
// Date : 03/11/2011
// PID control main function 

// Hardware property :
// Motor resolution approx .05 degree
// ADC resolution 3.3/0x0FFF

#include<p33FJ128MC804.h>

#define LED_1 LATAbits.LATA2
#define LED_2 LATAbits.LATA4


volatile unsigned int adc_value_setpoint;
volatile unsigned int adc_value_present;
volatile int Pr_error;
volatile int i_term_100;
volatile short flag;
//volatile unsigned int PWM=0x40;

/* variable defined by me
	for controlling purpose
	: Manish Sharma
*/
volatile int duty_cycle=2500;// initial declaration of duty_cycle somewhere in the middle of range
//range : 1600 --- > 4000 
//duty_cycle is proportional variable to original duty_cycle

float kp_const = 0.2000;
float ki_const = 0.0000;//0.1000;
float kd_const = 0.0105;//0.0050;

//end of declaration


int configure_ADC(void);
int configure_clock(void);
int configure_uart(void);
int configure_PPS(void);
int configure_TIMER_T1(void);



unsigned int adc_sample(unsigned char);
int PID(unsigned int,unsigned int);// will return PID_out
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


int CONTROL_5(unsigned int set_point,unsigned int present_value)
{
	// This is an example of PID controller
	// constant used kp_const and kd_const and ki_const
	int error = set_point - present_value;
	int	p_term = (error*kp_const);
	i_term_100 = i_term_100*100 + (ki_const * error);
	int d_term = kd_const *(error - Pr_error)*100;

	// this negative sign is required to be checked thoroughly
	int correction_factor = p_term + d_term + i_term_100/100;


	duty_cycle= duty_cycle + correction_factor;

	if(duty_cycle < 1600)
	duty_cycle =1600;
	else if(duty_cycle > 4000)
	duty_cycle =4000;
	else	// do nothing
	duty_cycle = duty_cycle;
	
	Pr_error = error;
	return 0;
}

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

void __attribute__ ((__interrupt__,__no_auto_psv__)) _T1Interrupt(void)
{
	// Interrupt Service Routine code goes here
	
	LED_1 = ~LED_1;
	unsigned int i[3];
	unsigned int j[3];
	i[0]=adc_sample(5);
	j[0]=adc_sample(4);
	i[1]=adc_sample(5);
	j[1]=adc_sample(4);
	i[2]=adc_sample(5);
	j[2]=adc_sample(4);
	
	adc_value_setpoint= (i[0]+i[1]+i[3])/3;
/*
	if(adc_value_setpoint>3900)	adc_value_setpoint = 3900;
	else if(adc_value_setpoint<1620)	adc_value_setpoint=1620;
	else adc_value_setpoint = adc_value_setpoint;
*/
	adc_value_present = (j[0]+j[1])/2;
	int PWM; 
	//int PWM = PID(adc_value_setpoint, adc_value_present);

	CONTROL_5(adc_value_setpoint, adc_value_present); //PID controller
	//CONTROL_TEST();
	motor_control(PWM);

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