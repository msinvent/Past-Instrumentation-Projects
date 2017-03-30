#if defined(__dsPIC33F__)
#include "p33Fxxxx.h"
#endif

#ifndef __MICRODAQV2_H__
#include "microDAQv2.h"
#endif

#ifndef __INTERRUPTS_H__
#include "interrupts.h"
#endif

#ifndef __ADIS16334_H__
#include "adis16334.h"
#endif

#ifndef __PERIPHERALS_H__
#include "peripherals.h"
#endif

#include<math.h>


#define PI 3.1416
volatile double manish;

volatile double dummy_0,dummy_1,dummy_2,x1,x2,psi,theta,phi;

volatile int freq = 50;

// Interrupts declaration
void __attribute__ ((__interrupt__,__no_auto_psv__)) _T1Interrupt(void);
void __attribute__ ((__interrupt__,__no_auto_psv__)) _U1RXInterrupt(void);
void __attribute__ ((__interrupt__,__no_auto_psv__)) _U2RXInterrupt(void);


void __attribute__ ((__interrupt__,__no_auto_psv__)) _T1Interrupt(void)
{
	// Interrupt Service Routine code goes here
	
	
	LED_1 = ~LED_1;
	LED_3 = ~LED_1;
	
	int imu_raw[8];//only first 7 var are usable
	double Kacc[3];
	double w[3];
	//p,q,r,ax,ay,az,T
	
	//	this interrupt is running at 50 Hz
	get_adis_data(imu_raw);

	U1TXREG = imu_raw[0];
	U1TXREG = imu_raw[0]>>8;
	
	U1TXREG = imu_raw[1];
	U1TXREG = imu_raw[1]>>8;
	
	while(!U1STAbits.TRMT);

	U1TXREG = imu_raw[2];
	U1TXREG = imu_raw[2]>>8;
	
	
	U1TXREG = imu_raw[3];
	U1TXREG = imu_raw[3]>>8;
	
	while(!U1STAbits.TRMT);
	
	U1TXREG = imu_raw[4];
	U1TXREG = imu_raw[4]>>8;

	while(!U1STAbits.TRMT);
	U1TXREG = imu_raw[5];	
	U1TXREG = imu_raw[5]>>8;
	
	U1TXREG = '\n';
	//U1TXREG = 0;
	//U1TXREG = 0;
	while(!U1STAbits.TRMT);

//	U1TXREG = 0x41;
//	while(!U1STAbits.TRMT);
/*
	measurement[0] = 0.05  * imu_raw[0] * 0.01745;//p in radians
	measurement[1] = 0.05  * imu_raw[1] * 0.01745;//q in radians
	measurement[2] = 0.05  * imu_raw[2] * 0.01745;//r in radians
	measurement[3] = 0.001 * imu_raw[3];//Axb
	measurement[4] = 0.001 * imu_raw[4];//Ayb
	measurement[5] = 0.001 * imu_raw[5];//Azb

	
	int man= 0;man= 10;//for debugging purpose
	manish = sin(PI/2);
	
	//INS mechanics here

		psi = (measurement[0])/freq; theta=(measurement[1])/freq; phi = (measurement[1])/freq;
		dummy_0=roll;
		dummy_1=pitch;
		dummy_2=yaw;
		
		x1=sin(phi)*sin(theta);
		x2=cos(phi)*sin(theta);
		
		roll = (((cos(theta)*cos(psi))*dummy_0) + ((x1*cos(psi)-cos(phi)*sin(psi))*dummy_1) + ((x2*cos(psi) + sin(phi)*sin(psi))*dummy_2));
		pitch = (((cos(theta)*sin(psi))*dummy_0) + ((x1*sin(psi)+cos(phi)*cos(psi))*dummy_1) + ((x2*sin(psi) - sin(phi)*cos(psi))*dummy_2));
		yaw = (((sin(theta))		 *dummy_0) - ((sin(phi)*cos(theta))			 *dummy_1) - ((cos(phi)*cos(theta))			   *dummy_2));	
*/
	// Interrupt Service Routine code ends here
	IFS0bits.T1IF = 0; // Clear Timer1 Interrupt Flag
	return;
}