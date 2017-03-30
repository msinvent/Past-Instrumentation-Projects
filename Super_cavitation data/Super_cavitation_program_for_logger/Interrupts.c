#if defined(__dsPIC33F__)
#include "p33Fxxxx.h"
#endif

#ifndef __MICRODAQV2_H__
#include "MicroDAQv2.h"
#endif

#ifndef __INTERRUPTS_H__
#include "interrupts.h"
#endif

#ifndef __ADXL_H__
#include "Adxl.h"
#endif

#ifndef __PERIPHERALS_H__
#include "Peripherals.h"
#endif



// Interrupts declaration
void __attribute__ ((__interrupt__,__no_auto_psv__)) _T1Interrupt(void);
void __attribute__ ((__interrupt__,__no_auto_psv__)) _U1RXInterrupt(void);
void __attribute__ ((__interrupt__,__no_auto_psv__)) _U2RXInterrupt(void);
void __attribute__((interrupt, no_auto_psv)) _DMA0Interrupt(void);
void __attribute__((interrupt, no_auto_psv)) _DMA1Interrupt(void);
void __attribute__((interrupt, no_auto_psv)) _DMA2Interrupt(void);


void __attribute__ ((__interrupt__,__no_auto_psv__)) _T1Interrupt(void)
{
	// Interrupt Service Routine code goes here
	
	LED_1 = ~LED_1;
	LED_3 = ~LED_1;
	//CS = ~CS;
	//writeRegister(DATA_FORMAT,0x02);

	U2TXREG = 'S';
	while (!U2STAbits.TRMT);//wait while the previous transmission is in progress
	
	U1TXREG = 'M';
	while (!U1STAbits.TRMT);//wait while the previous transmission is in progress
	
	//LED_1 = ~LED_1;	
	
	//cfgAdxl();
	//getdata(DATAY1);
	//SPI1BUF = 0xAE;
	//int i;
	//for(i = 0;i<500;i++);
	//SPI1BUF = 0x94;

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
unsigned int Rx3DmaBuffer = 0;

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

void __attribute__((interrupt, no_auto_psv)) _DMA2Interrupt(void)
{
	if(Rx3DmaBuffer == 0)
	{
		//ProcessSpiRxSamples(&Spi1RxBuffA[0]);
		U1TXREG = Spi1RxBuffB[0];
		U1TXREG = Spi1RxBuffB[1];
		U1TXREG = Spi1RxBuffB[2];
		U1TXREG = Spi1RxBuffB[3];
		while (!U1STAbits.TRMT);//wait while the previous transmission is in progress
		U1TXREG = Spi1RxBuffB[4];
		U1TXREG = Spi1RxBuffB[5];
		U1TXREG = Spi1RxBuffB[6];
		U1TXREG = Spi1RxBuffB[7];
		while (!U1STAbits.TRMT);//wait while the previous transmission is in progress
		U1TXREG = Spi1RxBuffB[8];
		U1TXREG = Spi1RxBuffB[9];
		U1TXREG = Spi1RxBuffB[10];
		U1TXREG = Spi1RxBuffB[11];
		while (!U1STAbits.TRMT);//wait while the previous transmission is in progress
		U1TXREG = Spi1RxBuffB[12];
		U1TXREG = Spi1RxBuffB[13];
		U1TXREG = Spi1RxBuffB[14];
		U1TXREG = Spi1RxBuffB[15];
		while (!U1STAbits.TRMT);//wait while the previous transmission is in progress


	}
	else
	{
		//ProcessSpiRxSamples(&Spi1RxBuffB[0]);
		U1TXREG = Spi1RxBuffA[0];
		U1TXREG = Spi1RxBuffA[1];
		U1TXREG = Spi1RxBuffA[2];
		U1TXREG = Spi1RxBuffA[3];
		while (!U1STAbits.TRMT);//wait while the previous transmission is in progress
		U1TXREG = Spi1RxBuffA[4];
		U1TXREG = Spi1RxBuffA[5];
		U1TXREG = Spi1RxBuffA[6];
		U1TXREG = Spi1RxBuffA[7];
		while (!U1STAbits.TRMT);//wait while the previous transmission is in progress
		U1TXREG = Spi1RxBuffA[8];
		U1TXREG = Spi1RxBuffA[9];
		U1TXREG = Spi1RxBuffA[10];
		U1TXREG = Spi1RxBuffA[11];
		while (!U1STAbits.TRMT);//wait while the previous transmission is in progress
		U1TXREG = Spi1RxBuffA[12];
		U1TXREG = Spi1RxBuffA[13];
		U1TXREG = Spi1RxBuffA[14];
		U1TXREG = Spi1RxBuffA[15];
		while (!U1STAbits.TRMT);//wait while the previous transmission is in progress
	}

	Rx3DmaBuffer ^= 1;
	LED_3 = ~LED_3;
	LED_1 = ~LED_3;

	//tglPin();					// Toggle RA6	
    IFS1bits.DMA2IF = 0;		// Clear the DMA0 Interrupt Flag
}
