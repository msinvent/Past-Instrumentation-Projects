#if defined(__dsPIC33F__)
#include "p33Fxxxx.h"
#endif

#ifndef __MICRODAQV2_H__
#include "MicroDAQv2.h"
#endif

#ifndef __INTERRUPTS__
#include "interrupts.h"
#endif

// Interrupts declaration
void __attribute__ ((__interrupt__,__no_auto_psv__)) _T1Interrupt(void);
void __attribute__ ((__interrupt__,__no_auto_psv__)) _U1RXInterrupt(void);
void __attribute__ ((__interrupt__,__no_auto_psv__)) _U2RXInterrupt(void);
void __attribute__((interrupt, no_auto_psv)) _DMA0Interrupt(void);
void __attribute__((interrupt, no_auto_psv)) _DMA1Interrupt(void);


void __attribute__ ((__interrupt__,__no_auto_psv__)) _T1Interrupt(void)
{
	// Interrupt Service Routine code goes here
	
	//LED_1 = ~LED_1;
	//LED_3 = ~LED_1;
	//writeRegister(DATA_FORMAT,0x02);

	U2TXREG = 'S';
	while (!U2STAbits.TRMT);//wait while the previous transmission is in progress
	
	U1TXREG = 'M';
	while (!U1STAbits.TRMT);//wait while the previous transmission is in progress
	
	SPI1BUF = '$';

	// Interrupt Service Routine code ends here
	IFS0bits.T1IF = 0; // Clear Timer1 Interrupt Flag
	return;
}

/*
//putting DMA in place so removing U1RX receive interrupt
void __attribute__ ((__interrupt__,__no_auto_psv__)) _U1RXInterrupt(void)
{
	//When a character is received

	while(U1STAbits.URXDA) //their is something to receive
	{
		char dummy=U1RXREG;
		
		if(dummy == 'S')
		{
			LED_1 = ~LED_1;
		}
			
	}
	
	IFS0bits.U1RXIF = 0; // Clear RX1 Interrupt Flag
	return;
}
*/
/*
//putting DMA in place so removing U2RX receive interrupt
void __attribute__ ((__interrupt__,__no_auto_psv__)) _U2RXInterrupt(void)
{
	//When a character is received
	
	
	while(U2STAbits.URXDA) //their is something to receive
	{
		char dummy=U2RXREG;
		
		if(dummy == 'M')
		{
			LED_3 = ~LED_3;
		}
		
	}
	
	IFS1bits.U2RXIF = 0; // Clear RX1 Interrupt Flag
	return;
}
*/

/*=============================================================================
Interrupt Service Routines.
=============================================================================*/
unsigned int Rx1DmaBuffer = 0;
unsigned int Rx2DmaBuffer = 0;


void __attribute__((interrupt, no_auto_psv)) _DMA0Interrupt(void)
{
	if(Rx1DmaBuffer == 0)
	{
		//ProcessSpiRxSamples(&Spi1RxBuffA[0]);

	}
	else
	{
		//ProcessSpiRxSamples(&Spi1RxBuffB[0]);
	}

	Rx1DmaBuffer ^= 1;
	LED_1 = ~LED_1;


      IFS0bits.DMA0IF = 0;		// Clear the DMA0 Interrupt Flag;

}



void __attribute__((interrupt, no_auto_psv)) _DMA1Interrupt(void)
{
	if(Rx2DmaBuffer == 0)
	{
		//ProcessSpiRxSamples(&Spi1RxBuffA[0]);

	}
	else
	{
		//ProcessSpiRxSamples(&Spi1RxBuffB[0]);
	}

	Rx2DmaBuffer ^= 1;
	LED_3 = ~LED_3;

	//tglPin();					// Toggle RA6	
    IFS0bits.DMA1IF = 0;		// Clear the DMA0 Interrupt Flag
}
