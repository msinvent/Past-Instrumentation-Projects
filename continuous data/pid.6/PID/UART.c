#include<p33FJ128MC804.h>

#define FCY 40000000
//#define BAUDRATE 115200
#define BAUDRATE 9600 
//38400
#define BRGVAL ((FCY/BAUDRATE)/16)-1
unsigned int i;

int configure_uart(void)
{
/*
	//UART-1
	U1MODEbits.STSEL = 0; // 1 Stop bit
	U1MODEbits.PDSEL = 0; // No Parity, 8 data bits
	U1MODEbits.ABAUD = 0; // Auto-Baud Disabled
	U1MODEbits.BRGH = 0; // ...Low Speed mode not working on high speed mode I dont know why
	U1BRG = BRGVAL; // BAUD Rate Setting
	//U1STAbits.UTXISEL0 = 0; // Interrupt after one TX Character is transmitted
	//U1STAbits.UTXISEL1 = 0;
	U1MODEbits.UARTEN = 1; // Enable UART
	U1STAbits.UTXEN = 1; // Enable UART TX
	U1STAbits.URXISEL = 0; // interrupt when a character is received at RX
	
	IPC2bits.U1RXIP = 7; // Set UART_1 Receive Interrupt Priority Level
	IFS0bits.U1RXIF = 0; // Clear Timer1 Interrupt Flag
	IEC0bits.U1RXIE = 1; // Enable Timer1 interrupt
	
*/	
	//UART-2
	U2MODEbits.STSEL = 0; // 1 Stop bit
	U2MODEbits.PDSEL = 0; // No Parity, 8 data bits
	U2MODEbits.ABAUD = 0; // Auto-Baud Disabled
	U2MODEbits.BRGH = 0; // Low Speed mode
	U2BRG = BRGVAL; // BAUD Rate Setting
	//U2STAbits.UTXISEL0 = 0; // Interrupt after one TX Character is transmitted
	//U2STAbits.UTXISEL1 = 0;
	U2MODEbits.UARTEN = 1; // Enable UART
	U2STAbits.UTXEN = 1; // Enable UART TX

	//IPC7bits.U2RXIP = 5; // Set UART_1 Receive Interrupt Priority Level
	//IFS1bits.U2RXIF = 0; // Clear Timer1 Interrupt Flag
	//IEC1bits.U2RXIE = 1; // Enable Timer1 interrupt
		
	
	return 0;
}
//Configure the PPS Crossbar for UART pins mapping
void configure_PPS(void)
{
	
	__builtin_write_OSCCONL(OSCCON & ~(1<<6));	//Unlock Registers

	// Configure Input Functions
	//	RPINR18bits.U1RXR = 11;		//Assign U1Rx To Pin RP11
	//	RPINR19bits.U2RXR = 13;		//Assign U2Rx To Pin RP13

	// Configure Output Functions
	//	RPOR6bits.RP12R = 3;		//Assign U1Tx To Pin RP12
		RPOR7bits.RP14R = 5;		//Assign U2Tx To Pin RP14

	__builtin_write_OSCCONL(OSCCON | (1<<6));	// Lock Registers
	
}
//End PPS Configuration
