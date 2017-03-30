#include<p33FJ128MC804.h>

#define LED_1 LATAbits.LATA2
#define LED_3 LATAbits.LATA4

unsigned int BufferA[23] __attribute__((space(dma)));
unsigned int BufferB[23] __attribute__((space(dma)));

unsigned int BufferC[23] __attribute__((space(dma)));
unsigned int BufferD[23] __attribute__((space(dma)));

volatile int duty_cycle_roll = 2800;
volatile int duty_cycle_pitch = 2800;
volatile int duty_cycle_yaw = 2800;
//range : 1600 --->4000

float kp_const_roll = 0.2000;
float ki_const_roll = 0.0000;//0.1000;
float kd_const_roll = 0.0105;//0.0050;

int set_point_roll = 0;
int Pr_error_roll = 0;
int i_term_100_roll = 0;

float kp_const_pitch = 0.2000;
float ki_const_pitch = 0.0000;//0.1000;
float kd_const_pitch = 0.0105;//0.0050;

int set_point_pitch = 0;
int Pr_error_pitch = 0;
int i_term_100_pitch = 0;

float kp_const_yaw = 0.2000;
float ki_const_yaw = 0.0000;//0.1000;
float kd_const_yaw = 0.0105;//0.0050;

int set_point_yaw = 0;
int Pr_error_yaw = 0;
int i_term_100_yaw = 0;


int IMU_data[24];

volatile signed long roll;
volatile signed long pitch;
volatile signed long yaw;
volatile signed long accel_x;
volatile signed long accel_y;
volatile signed long accel_z;
volatile signed long anglerate_x;
volatile signed long anglerate_y;
volatile signed long anglerate_z;




//configured for UART1_RX
int configure_DMA_0()
{	
	//#1 Select DMA operating modes
	DMA0CON = 0x0002; // Continuous, Ping-Pong, Post-Inc., Periph-RAM
	//#2 Set DMA transfer count
	DMA0CNT = 22; // 8 DMA requests
	//#3 Select Peripheral
	DMA0REQ = 0x000B; // Select UART1_RX
	
	DMA0PAD = (volatile unsigned int) &U1RXREG;
	//DSPRAM memory allocation
	DMA0STA = __builtin_dmaoffset(BufferA);
	DMA0STB = __builtin_dmaoffset(BufferB);
	
	IFS0bits.DMA0IF = 0; // Clear DMA interrupt
	IEC0bits.DMA0IE = 1; // Enable DMA interrupt
	DMA0CONbits.CHEN = 1; // Enable DMA Channel
	
	return 0;
}	
/*
//configured for UART2_RX
int configure_DMA_1()
{	
	//#1 Select DMA operating modes
	DMA1CON = 0x0002; // Continuous, Ping-Pong, Post-Inc., Periph-RAM
	//#2 Set DMA transfer count
	DMA1CNT = 7; // 8 DMA requests
	//#3 Select Peripheral
	DMA1REQ = 0x001E; // Select UART2_RX
	//#Associating DMA1 with UART2_RX
	DMA1PAD = (volatile unsigned int) &U2RXREG;
	//DSPRAM memory allocation
	DMA1STA = __builtin_dmaoffset(BufferC);
	DMA1STB = __builtin_dmaoffset(BufferD);
	
	IFS0bits.DMA1IF = 0; // Clear DMA interrupt
	IEC0bits.DMA1IE = 1; // Enable DMA interrupt
	DMA1CONbits.CHEN = 1; // Enable DMA Channel
	
	return 0;
}	
*/
void __attribute__((__interrupt__)) _DMA0Interrupt()
{
	//interrupt routine here	
	static unsigned int BufferCount = 0 ; //keep record of which buffer contains RX data
	if(BufferCount == 0)
	{//BufferA full, Do whatever you want to do after the buffer A is full
	
		IMU_data[0] =BufferA[0];
		IMU_data[1] =BufferA[1];
		IMU_data[2] =BufferA[2];
		IMU_data[3] =BufferA[3];
		IMU_data[4] =BufferA[4];
		IMU_data[5] =BufferA[5];
		IMU_data[6] =BufferA[6];
		IMU_data[7] =BufferA[7];
		IMU_data[8] =BufferA[8];
		IMU_data[9] =BufferA[9];
		IMU_data[10]=BufferA[10];
		IMU_data[11]=BufferA[11];
		IMU_data[12]=BufferA[12];
		IMU_data[13]=BufferA[13];
		IMU_data[14]=BufferA[14];
		IMU_data[15]=BufferA[15];
		IMU_data[16]=BufferA[16];
		IMU_data[17]=BufferA[17];
		IMU_data[18]=BufferA[18];
		IMU_data[19]=BufferA[19];
		IMU_data[20]=BufferA[20];
		IMU_data[21]=BufferA[21];
		IMU_data[22]=BufferA[22];

		LED_3 = 1;
	}
	else
	{//BufferB full, Do whatever you want to do after the buffer B is full
	
		IMU_data[0] =BufferB[0];
		IMU_data[1] =BufferB[1];
		IMU_data[2] =BufferB[2];
		IMU_data[3] =BufferB[3];
		IMU_data[4] =BufferB[4];
		IMU_data[5] =BufferB[5];
		IMU_data[6] =BufferB[6];
		IMU_data[7] =BufferB[7];
		IMU_data[8] =BufferB[8];
		IMU_data[9] =BufferB[9];
		IMU_data[10]=BufferB[10];
		IMU_data[11]=BufferB[11];
		IMU_data[12]=BufferB[12];
		IMU_data[13]=BufferB[13];
		IMU_data[14]=BufferB[14];
		IMU_data[15]=BufferB[15];
		IMU_data[16]=BufferB[16];
		IMU_data[17]=BufferB[17];
		IMU_data[18]=BufferB[18];
		IMU_data[19]=BufferB[19];
		IMU_data[20]=BufferB[20];
		IMU_data[21]=BufferB[21];
		IMU_data[22]=BufferB[22];

		LED_3 = 0;
	}
	
	{//calculating IMU_data
		//to 1 degree of accuracy .1
		roll = ((IMU_data[1]<<8) | IMU_data[2]);
		roll*=3600;
		roll=(roll>>16);
		pitch =((IMU_data[3]<<8) | IMU_data[4]);
		pitch*=3600;
		pitch =(pitch>>16);
		yaw =((IMU_data[5]<<8)|IMU_data[6]);
		yaw*=3600;
		yaw = (yaw>>16);
		//to 3 degree of accuracy .001
		accel_x =((IMU_data[7]<<8) | IMU_data[8]);
		accel_x*=85000;
		accel_x=(accel_x>>15);
		accel_y =((IMU_data[9]<<8) | IMU_data[10]);
		accel_y*=85000;
		accel_y=(accel_y>>15);
		accel_z =((IMU_data[11]<<8) | IMU_data[12]);
		accel_z*=85000;
		accel_z=(accel_z>>15);
		//to 3 degree of accuracy .001
		anglerate_x =((IMU_data[13]<<8) | IMU_data[14]);
		anglerate_x*=10000;
		anglerate_x=(anglerate_x>>15);
		anglerate_y =((IMU_data[15]<<8) | IMU_data[16]);
		anglerate_y*=10000;
		anglerate_y=(anglerate_y>>15);
		anglerate_z =((IMU_data[17]<<8) | IMU_data[18]);
		anglerate_z*=10000;
		anglerate_z=(anglerate_z>>15);
		//calculation of IMU data ends
		}
	{//Autopilot code here... provide actuation depending on IMU data and desired values
	
		//to terminal
		//U2TXREG = (char)roll;
		//while (!U2STAbits.TRMT);//wait while the previous transmission is in progress
		

		 // This is an example of PID controller
		 // constant used kp_const and kd_const and ki_const
		 int error_roll = -roll;
		 int p_term_roll = (error_roll*kp_const_roll);
		 i_term_100_roll = i_term_100_roll*100 + (ki_const_roll * error_roll);
		 int d_term_roll = kd_const_roll *(error_roll - Pr_error_roll)*100;
		 		// this negative sign is required to be checked thoroughly
 		int correction_factor_roll = p_term_roll + d_term_roll + i_term_100_roll/100;


 		duty_cycle_roll= duty_cycle_roll + correction_factor_roll;

 		if(duty_cycle_roll < 1600)
 		duty_cycle_roll =1600;
 		else if(duty_cycle_roll > 4000)
 		duty_cycle_roll =4000;
 		else // do nothing
 		duty_cycle_roll = duty_cycle_roll;
 
 		Pr_error_roll = error_roll;

		
		U2TXREG = 0x80; // start byte
		U2TXREG = 0x01; // Device ID which is 0x01 for 8 servo controller
		U2TXREG = 0x04; // configuration for forward movement
		U2TXREG = 0x01; // Servo 0 selected
		while(!U2STAbits.TRMT);
		//highest 7 bits
		U2TXREG = duty_cycle_roll>>7;
		//lowest 7 bits
		U2TXREG = duty_cycle_roll&127;
		while(!U2STAbits.TRMT);		
		//if(roll<0)
		//duty_cycle = duty_cycle+5;
		//else
		//duty_cycle = duty_cycle-5;
		
		//for pitch control
	
		int error_pitch = pitch;
		 int p_term_pitch = (error_pitch*kp_const_pitch);
		 i_term_100_pitch = i_term_100_pitch*100 + (ki_const_pitch * error_pitch);
		 int d_term_pitch = kd_const_pitch *(error_pitch - Pr_error_pitch)*100;
		 		// this negative sign is required to be checked thoroughly
 		int correction_factor_pitch = p_term_pitch + d_term_pitch + i_term_100_pitch/100;


 		duty_cycle_pitch= duty_cycle_pitch + correction_factor_pitch;

 		if(duty_cycle_pitch < 1600)
 		duty_cycle_pitch =1600;
 		else if(duty_cycle_pitch > 4000)
 		duty_cycle_pitch =4000;
 		else // do nothing
 		duty_cycle_pitch = duty_cycle_pitch;
 
 		Pr_error_pitch = error_pitch;
		
		U2TXREG = 0x80; // start byte
		U2TXREG = 0x01; // Device ID which is 0x01 for 8 servo controller
		while(!U2STAbits.TRMT);
		U2TXREG = 0x04; // configuration for forward movement
		U2TXREG = 0x02; // Servo 1 selected
		
		//highest 7 bits
		U2TXREG = duty_cycle_pitch>>7;
		//lowest 7 bits
		U2TXREG = duty_cycle_pitch&127;
		

		while (!U2STAbits.TRMT);//wait while the previous transmission is in progress
/*	
		//for yaw control
		int error_yaw = -yaw;
		 int p_term_yaw = (error_yaw*kp_const_yaw);
		 i_term_100_yaw = i_term_100_yaw*100 + (ki_const_yaw * error_yaw);
		 int d_term_yaw = kd_const_yaw *(error_yaw - Pr_error_yaw)*100;
		 		// this negative sign is required to be checked thoroughly
 		int correction_factor_yaw = p_term_yaw + d_term_yaw + i_term_100_yaw/100;


 		duty_cycle_yaw= duty_cycle_yaw + correction_factor_yaw;

 		if(duty_cycle_yaw < 1600)
 		duty_cycle_yaw =1600;
 		else if(duty_cycle_yaw > 4000)
 		duty_cycle_yaw =4000;
 		else // do nothing
 		duty_cycle_yaw = duty_cycle_yaw;
 
 		Pr_error_yaw = error_yaw;
		
		U2TXREG = 0x80; // start byte
		U2TXREG = 0x02; // Device ID which is 0x01 for 8 servo controller
		U2TXREG = 0x04; // configuration for forward movement
		U2TXREG = 0x03; // Servo 2 selected
		while(!U2STAbits.TRMT);
		//highest 7 bits
		U2TXREG = duty_cycle_yaw>>7;
		//lowest 7 bits
		U2TXREG = duty_cycle_yaw&127;
		

		while (!U2STAbits.TRMT);//wait while the previous transmission is in progress
*/	
	//Autopilot code ends..
	}

	
	
	BufferCount ^= 1; //exclusive OR operator used to toggle Buffer count
	//interrupt routine ends
	IFS0bits.DMA0IF = 0;
	
}

/*
void __attribute__((__interrupt__)) _DMA1Interrupt()
{
	//interrupt routine here	
	static unsigned int BufferCount = 0 ; //keep record of which buffer contains RX data
	if(BufferCount == 0)
	{
		//BufferA full
		//Do whatever you want to do after the buffer A is full
		U1TXREG =BufferC[0];
		U1TXREG =BufferC[1];
		U1TXREG =BufferC[2];
		U1TXREG =BufferC[3];
		while (!U1STAbits.TRMT);//wait while the previous transmission is in progress
		U1TXREG =BufferC[4];
		U1TXREG =BufferC[5];
		U1TXREG =BufferC[6];
		U1TXREG =BufferC[7];
		while (!U1STAbits.TRMT);//wait while the previous transmission is in progress
		LED_3 = 1;
	}
	else
	{	
		//BufferB full
		//Do whatever you want to do after the buffer B is full
		U1TXREG =BufferD[0];
		U1TXREG =BufferD[1];
		U1TXREG =BufferD[2];
		U1TXREG =BufferD[3];
		while (!U1STAbits.TRMT);//wait while the previous transmission is in progress
		U1TXREG =BufferD[4];
		U1TXREG =BufferD[5];
		U1TXREG =BufferD[6];
		U1TXREG =BufferD[7];
		while (!U1STAbits.TRMT);//wait while the previous transmission is in progress
		LED_3 = 0;
	}
	
	BufferCount ^= 1; //exclusive OR operator used to toggle Buffer count
	//interrupt routine ends
	IFS0bits.DMA1IF = 0;
	
}
*/
