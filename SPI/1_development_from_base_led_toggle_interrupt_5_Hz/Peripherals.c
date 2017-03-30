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
	
	U1STAbits.URXISEL = 0; // interrupt when a character is received at RX
	
	//IPC2bits.U1RXIP = priority; // Set UART_1 Receive Interrupt Priority Level
	//not used as DMA is used
	IFS0bits.U1RXIF = 0; // Clear UART1_RX Interrupt Flag
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

	U2STAbits.URXISEL = 0; // interrupt when a character is received at RX
	
	//IPC7bits.U2RXIP = priority; // Set UART2_RX Receive Interrupt Priority Level
	//not used as DMA is used
	IFS1bits.U2RXIF = 0; // Clear UART2_RX Interrupt Flag
	//IEC1bits.U2RXIE = 0; // Disable UART2_RX interrupt
	
}
