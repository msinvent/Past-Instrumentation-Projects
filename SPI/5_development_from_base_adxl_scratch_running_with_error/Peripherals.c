//Peripherals used and functions attached

#if defined(__dsPIC33F__)
#include "p33Fxxxx.h"
#endif


#define TIMER_FREQ 5
//5,10,15,20,25,30 50,55,60,70,100.

#define FCY 40000000
#define BAUDRATE1 38400 
#define BAUDRATE2 38400 

#define BRGVAL1 ((FCY/BAUDRATE1)/16)-1
#define BRGVAL2 ((FCY/BAUDRATE2)/16)-1

void cfgTimerT1(unsigned int priority);
void cfgUart1(unsigned int priority);
void cfgUart2(unsigned int priority);
void cfgSpi1(void);
void cfgDma0Uart1Rx(void);
void cfgDma1Uart2Rx(void);
void cfgDma2Spi1Rx(void);

void ProcessSpiRxSamples(unsigned int * SpiRxBuffer);
//void cfgDma0SpiTx(void);
//void cfgDma1SpiRx(void);
//void cfgSPI1Master(void);



void cfgTimerT1(unsigned int priority)
{
	T1CONbits.TON = 0;// Disable Timer
	T1CONbits.TCS = 0;// Select internal instruction cycle clock
	T1CONbits.TGATE = 0;// Disable Gated Timer mode
	T1CONbits.TCKPS = 0b11;// Select 1:256 Prescaler
	TMR1 = 0x0000;// Clear timer register
		if(TIMER_FREQ == 50)
		PR1 = 0x0C35;//0xC35 for 50 Hz 0x138 for 5 Hz
		else if(TIMER_FREQ == 100)
		PR1 = 0x061A;
		else if(TIMER_FREQ == 5)
		PR1 = 0x7A12;
		else if(TIMER_FREQ == 10)
		PR1 = 0x3D09;
		else if(TIMER_FREQ == 15)
		PR1 = 0x28B0;
		else if(TIMER_FREQ == 20)
		PR1 = 0x1E84;
		else if(TIMER_FREQ == 25)
		PR1 = 0x186A;
		else if(TIMER_FREQ == 30)
		PR1 = 0x1458;
		else if(TIMER_FREQ == 55)
		PR1 = 0x072E;
		else if(TIMER_FREQ == 60)
		PR1 = 0x06C8;
		else if(TIMER_FREQ == 70)
		PR1 = 0x061A;
		else
		PR1 = 0x7A12;
		
	IPC0bits.T1IP = priority; // Set Timer1 Interrupt Priority Level
	IFS0bits.T1IF = 0; // Clear Timer1 Interrupt Flag
	IEC0bits.T1IE = 1; // Enable Timer1 interrupt
	T1CONbits.TON = 1; // Start Timer
	
}

void cfgUart1(unsigned int priority)
{

	//UART-1
	U1MODEbits.STSEL = 0; // 1 Stop bit
	U1MODEbits.PDSEL = 0; // No Parity, 8 data bits
	U1MODEbits.ABAUD = 0; // Auto-Baud Disabled
	U1MODEbits.BRGH = 0; // low speed mode selected
	U1BRG = BRGVAL1; // BAUD Rate Setting
	//U1STAbits.UTXISEL0 = 0; // Interrupt after one TX Character is transmitted
	//U1STAbits.UTXISEL1 = 0;
	U1MODEbits.UARTEN = 1; // Enable UART
	U1STAbits.UTXEN = 1; // Enable UART TX
	
	//U1STAbits.URXISEL = 0; // interrupt when a character is received at RX
	
	//IPC2bits.U1RXIP = priority; // Set UART_1 Receive Interrupt Priority Level
	//IFS0bits.U1RXIF = 0; // Clear UART1_RX Interrupt Flag
	//IEC0bits.U1RXIE = 1; // Enable UART1_RX interrupt
	
}

void cfgUart2(unsigned int priority)
{

	//UART-2
	U2MODEbits.STSEL = 0; // 1 Stop bit
	U2MODEbits.PDSEL = 0; // No Parity, 8 data bits
	U2MODEbits.ABAUD = 0; // Auto-Baud Disabled
	U2MODEbits.BRGH = 0; // Low Speed mode
	U2BRG = BRGVAL2; // BAUD Rate Setting
	//U2STAbits.UTXISEL0 = 0; // Interrupt after one TX Character is transmitted
	//U2STAbits.UTXISEL1 = 0;
	U2MODEbits.UARTEN = 1; // Enable UART
	U2STAbits.UTXEN = 1; // Enable UART TX

	//U2STAbits.URXISEL = 0; // interrupt when a character is received at RX
	
	//IPC7bits.U2RXIP = priority; // Set UART2_RX Receive Interrupt Priority Level
	//IFS1bits.U2RXIF = 0; // Clear UART2_RX Interrupt Flag
	//IEC1bits.U2RXIE = 1; // Enable UART2_RX interrupt
}


/*=============================================================================
_DMA initializations and interrupts
=============================================================================*/

void ProcessSpiRxSamples(unsigned int * SpiRxBuffer);

unsigned int Uart1RxBuffA[16] __attribute__((space(dma)));
unsigned int Uart1RxBuffB[16] __attribute__((space(dma)));
unsigned int Uart2RxBuffA[16] __attribute__((space(dma)));
unsigned int Uart2RxBuffB[16] __attribute__((space(dma)));


void cfgDma0Uart1Rx(void)
{

	DMA0CON = 0x0002;					
	DMA0CNT = 15;						
	DMA0REQ = 0x00B;					

	DMA0PAD = (volatile unsigned int) &U1RXREG;
	//DMA0PAD = (volatile unsigned int) &SPI1BUF;

	DMA0STA= __builtin_dmaoffset(Uart1RxBuffA);
	DMA0STB= __builtin_dmaoffset(Uart1RxBuffB);
	
	
	IFS0bits.DMA0IF  = 0;			// Clear DMA interrupt
	IEC0bits.DMA0IE  = 1;			// Enable DMA interrupt
	DMA0CONbits.CHEN = 1;			// Enable DMA Channel	
	
}

// DMA1 configuration
// Direction: Read from SPI buffer and write to DMA RAM 
// AMODE: Register Indirect with Post-Increment mode
// MODE: Continuous, Ping-Pong Enabled
// IRQ: SPI
void cfgDma1Uart2Rx(void)
{
	DMA1CON = 0x0002;				
	DMA1CNT = 15;						
	DMA1REQ = 0x001E; // Select UART2_RX

	DMA1PAD = (volatile unsigned int) &U2RXREG;
	//DMA1PAD = (volatile unsigned int) &SPI1BUF;
	DMA1STA= __builtin_dmaoffset(Uart2RxBuffA);
	DMA1STB= __builtin_dmaoffset(Uart2RxBuffB);
	
	
	IFS0bits.DMA1IF  = 0;			// Clear DMA interrupt
	IEC0bits.DMA1IE  = 1;			// Enable DMA interrupt
	DMA1CONbits.CHEN = 1;			// Enable DMA Channel		
	
}

void ProcessSpiRxSamples(unsigned int * SpiRxBuffer)
{
	/* Do something with SPI Samples */
}

unsigned int Spi1RxBuffA[16] __attribute__((space(dma)));
unsigned int Spi1RxBuffB[16] __attribute__((space(dma)));


void cfgSpi1(void)
{
	/* The following code shows the SPI register configuration for Master mode */
	//interrupt is not used by me
	//IFS0bits.SPI1IF = 0; // Clear the Interrupt Flag
	//IEC0bits.SPI1IE = 0;// Disable the Interrupt 

	// SPI1CON1 Register Settings
	SPI1CON1bits.DISSCK = 0;// Internal Serial Clock is Enabled
	SPI1CON1bits.DISSDO = 0;// SDOx pin is controlled by the module
	SPI1CON1bits.MODE16 = 0;// Communication is word-wide (8 bits)
	SPI1CON1bits.SMP = 1;// Input data is sampled at the end of data output time
	SPI1CON1bits.CKE = 0;// Serial output data changes on transition 
						// from Idle clock state to active clock state
	//he bhagwan.... I love u !!
	SPI1CON1bits.CKP = 1;// Idle state for clock is a low level; 
						// active state is a high level
	SPI1CON1bits.MSTEN = 1;// Master mode Enabled
	SPI1CON1bits.SSEN = 0; // SS1 pin as GPIO
	SPI1CON1bits.PPRE = 0;// Primary prescaler 1:16
	SPI1CON1bits.SPRE = 0;// Primary prescaler 1:1
	TRISCbits.TRISC5 = 0; // CS configured as GP output
	LATCbits.LATC5 = 1; //declare as output

	SPI1STATbits.SPIEN = 1;// Enable SPI module

}

void cfgDma2Spi1Rx(void)
{
	DMA2CON = 0x0002;				
	DMA2CNT = 15;						
	DMA2REQ = 0x00A; // Select SPI1RX

	//DMA2PAD = (volatile unsigned int) &U2RXREG;
	DMA2PAD = (volatile unsigned int) &SPI1BUF;
	DMA2STA= __builtin_dmaoffset(Spi1RxBuffA);
	DMA2STB= __builtin_dmaoffset(Spi1RxBuffB);
	
	
	IFS1bits.DMA2IF  = 0;			// Clear DMA interrupt
	IEC1bits.DMA2IE  = 1;			// Enable DMA interrupt
	DMA2CONbits.CHEN = 1;			// Enable DMA Channel		
	
}
