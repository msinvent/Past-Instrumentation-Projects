    // PID control main function
// this program is giving some satisfactory performance

#include<p33FJ128MC804.h>

#define LED_1 LATAbits.LATA2
#define LED_2 LATAbits.LATA4

/***********************************************
PID CONSTANTS declaration
***********************************************/
#define Kp 2
#define Kd 0
#define Ki 0

volatile unsigned int adc_value_setpoint;
volatile unsigned int adc_value_present;
volatile int Pr_error;
volatile int i_term;
volatile short flag;
//volatile unsigned int PWM=0x40;

/* variable defined by me
	for controlling purpose
	: Manish Sharma
*/
volatile int duty_cycle=0x40;//this is a representation
// of duty cycle and not exact duty cycle

float kp_const = 0.0135;
float ki_const = 0.0200;
float kd_const = 0.0005;



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

int PID(unsigned int set_point,unsigned int present_value)
{	
	/*
	int delta_PWM;
	int error = set_point - present_value;
	int P_term = Kp*error;
	int D_term = Kd *(error - Pr_error)*100;
	// check this thing later
	I_term += Ki * error * 0.01;
	int PID_out = P_term + D_term + I_term;
	Pr_error = error;
	//int delta_PWM = (PID_out)/ (0x03E8);
	delta_PWM = PID_out;
	if(delta_PWM<-2) delta_PWM =-2;
	else if (delta_PWM>2) delta_PWM = 2;
	else delta_PWM = delta_PWM;
	PWM = PWM + delta_PWM;
	if(PWM<0) PWM =0;
	else if (PWM>0x80) PWM = 0x80;
	else PWM = PWM;
	return PWM;
	*/
}
int CONTROL_1(unsigned int set_point,unsigned int present_value)
{
	// the controller is trying to minimize error by approaching towards 
	// some specific value with constant rate
	//constant used 1
	int error = set_point - present_value;
	if(error<-15 && duty_cycle>0x00)
	duty_cycle= duty_cycle-1;
	else if(error>15 && duty_cycle<0x80)
	duty_cycle= duty_cycle+1;
	else	//do nothing
	duty_cycle+=0;
	
	return 0;
	
}
int CONTROL_2(unsigned int set_point,unsigned int present_value)
{
	// This is an example of proportional controller
	// constant used kp_const
	// checked on kp_const .0275
	int error = set_point - present_value;
	int	p_term = (error*kp_const);
	
	int correction_factor = p_term;

	if(correction_factor>10) correction_factor = 10;
	else if(correction_factor<-10) correction_factor = -10;
	else correction_factor = correction_factor;

	if(error<-15 || error >15) // just to remove noise
	duty_cycle= duty_cycle + correction_factor;

	else //do nothing
	duty_cycle+=0;

	//saturating out of bound dutycycle values
	if(duty_cycle < 0x00)
	duty_cycle =0;
	else if(duty_cycle > 0x80)
	duty_cycle =0x80;
	else	// do nothing
	duty_cycle = duty_cycle;

	return 0;
}

int CONTROL_3(unsigned int set_point,unsigned int present_value)
{
	// This is an example of proportional integral controller
	// constant used kp_const and ki const
	int error = set_point - present_value;
	int	p_term = (error*kp_const);
	i_term = i_term + (ki_const * error * 0.01);

	
	int correction_factor = p_term - i_term;

	// i tried to remove small errors by this but not working great	
	if(correction_factor == 0)
	{
		if(error>0) correction_factor += 1;
		else if(error<0) correction_factor -= 1;
	}
	else 
	{
	// do nothing
	}

	/*
	if(correction_factor>10) correction_factor = 10;
	else if(correction_factor<-10) correction_factor = -10;
	else correction_factor = correction_factor;
	*/
	// i have idented these lines to check if the errors can be removed this way
//	if(error<-15 || error >15) // just to remove noise
	duty_cycle= duty_cycle + correction_factor;

//	else //do nothing
//	duty_cycle+=0;

	//saturating out of bound dutycycle values
	if(duty_cycle < 0x00)
	duty_cycle =0;
	else if(duty_cycle > 0x80)
	duty_cycle =0x80;
	else	// do nothing
	duty_cycle = duty_cycle;

	return 0;
}

int CONTROL_3_2(unsigned int set_point,unsigned int present_value)
{
	// This is an example of proportional integral controller
	// constant used kp_const and ki const
	int error = set_point - present_value;
	int	p_term = (error*kp_const);
	i_term = i_term + (ki_const * error * 0.01);

// integral term added :)	
	int correction_factor = p_term + i_term;

	// i tried to remove small errors by this but not working great	
/*	if(correction_factor == 0)
	{
		if(error>0) correction_factor += 1;
		else if(error<0) correction_factor -= 1;
	}
	else 
	{
	// do nothing
	}
*/
	/*
	if(correction_factor>10) correction_factor = 10;
	else if(correction_factor<-10) correction_factor = -10;
	else correction_factor = correction_factor;
	*/
	// i have idented these lines to check if the errors can be removed this way
//	if(error<-15 || error >15) // just to remove noise
	duty_cycle= duty_cycle + correction_factor;

//	else //do nothing
//	duty_cycle+=0;

	//saturating out of bound dutycycle values
	if(duty_cycle < 0x00)
	duty_cycle =0;
	else if(duty_cycle > 0x80)
	duty_cycle =0x80;
	else	// do nothing
	duty_cycle = duty_cycle;

	return 0;
}

int CONTROL_4(unsigned int set_point,unsigned int present_value)
{
	// This is an example of proportional differential controller
	// constant used kp_const and k
	int error = set_point - present_value;
	int	p_term = (error*kp_const);
	
	int d_term = kd_const *(error - Pr_error)*100;

	// this negative sign is required to be checked thoroughly
	int correction_factor = p_term + d_term;

	/*
	if(correction_factor>10) correction_factor = 10;
	else if(correction_factor<-10) correction_factor = -10;
	else correction_factor = correction_factor;
	*/
	/*
	if(error<-15 || error >15) // just to remove noise
	duty_cycle= duty_cycle + correction_factor;

	else //do nothing
	duty_cycle+=0;
	*/
	//saturating out of bound dutycycle values

	duty_cycle= duty_cycle + correction_factor;

	if(duty_cycle < 0x00)
	duty_cycle =0;
	else if(duty_cycle > 0x80)
	duty_cycle =0x80;
	else	// do nothing
	duty_cycle = duty_cycle;
	
	Pr_error = error;
	return 0;
}

void motor_control(int a )
{
	a=10;
	// this PWM is actually PID_out and in my point of view need to be
	// normalized accordingly
	U2TXREG = 0x80; // start byte
	U2TXREG = 0x01; // Device ID which is 0x01 for 8 servo controller
	U2TXREG = 0x02; // configuration for forward movement 0x4F
	U2TXREG = 0x00; // Servo 0 selected
	
/*	if(PWM<0) PWM =0;
	else if (PWM>0x80) PWM = 0x80;
	else PWM = PWM;
*/
	//U2TXREG = PWM ;// value between 0 to 127 have to be fed... :)

	//U2TXREG = 0x55 ;// value between 0 to 127 have to be fed... :)
	U2TXREG = duty_cycle;
	while (!U2STAbits.TRMT);//wait while the previous transmission is in progress

	return;
	
}

void __attribute__ ((__interrupt__,__no_auto_psv__)) _T1Interrupt(void)
{
	// Interrupt Service Routine code goes here
	
	LED_1 = ~LED_1;

	adc_value_setpoint= adc_sample(5);
	adc_value_present = adc_sample(4);
	int PWM; 
	//int PWM = PID(adc_value_setpoint, adc_value_present);
	//CONTROL_1(adc_value_setpoint, adc_value_present);
	//CONTROL_2(adc_value_setpoint, adc_value_present);
	//CONTROL_3(adc_value_setpoint, adc_value_present);
	//CONTROL_3_2(adc_value_setpoint, adc_value_present);
	CONTROL_4(adc_value_setpoint, adc_value_present);
	//send PWM value instead of this DUMMY data
	motor_control(PWM);

	//flag = flag + 1;
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