#if defined(__dsPIC33F__)
#include "p33Fxxxx.h"
#endif

void cfgClock(void);
void cfgPps(void);

// Select Internal FRC at POR
_FOSCSEL(FNOSC_FRC);
// Enable Clock Switching and Configure
_FOSC(FCKSM_CSECMD & OSCIOFNC_OFF);
// Watchdog Timer Enabled/disabled by user software
_FWDT(FWDTEN_OFF);


void cfgClock(void)
{
// This function make the internal clock to operate at 40 Mhz
// Configure PLL prescaler, PLL postscaler, PLL divisor
PLLFBD = 41; // M = 43
CLKDIVbits.PLLPOST=0; // N2 = 2
CLKDIVbits.PLLPRE=0; // N1 = 2

// Disable Watch Dog Timer
	RCONbits.SWDTEN=0;

// Initiate Clock Switch to Internal FRC with PLL (NOSC = 0b001)
__builtin_write_OSCCONH(0x01);
__builtin_write_OSCCONL(0x01);
// Wait for Clock switch to occur
while (OSCCONbits.COSC != 0b001);
// Wait for PLL to lock
while(OSCCONbits.LOCK != 1) {};

}

//Configure the PPS Crossbar for UART pins mapping
void cfgPps(void)
{
	
	__builtin_write_OSCCONL(OSCCON & ~(1<<6));	//Unlock Registers

	
	/* configuration for MicroDAQv2	

		// Configure Input Functions
		RPINR18bits.U1RXR = 5;	//Assign U1Rx To Pin RP5(41)
		RPINR19bits.U2RXR = 4;	//Assign U2Rx To Pin RP4(33)
		RPINR20bits.SDI1R = 24;	//Assign SD1I To Pin RP24(4)

		// Configure Output Functions
		RPOR3bits.RP6R = 3;		//Assign U1Tx To Pin RP6(42)
		RPOR4bits.RP8R = 5;		//Assign U2Tx To Pin RP8(44)
		RPOR4bits.RP9R = 7;		//Assign SD1O To Pin RP9(1)
		RPOR12bits.RP25R = 8;	//Assign CLK To Pin RP25(5)
		// Configuration for MicroDAQv2 end
	*/
	
	// Configuration for DataLOgger

		//Configure Input Functions
		RPINR18bits.U1RXR = 6;	//Assign U1Rx To Pin RP6(42)
		RPINR20bits.SDI1R = 25;	//Assign SD1I To Pin RP25(5)

		// Configure Output Functions
		RPOR9bits.RP19R = 3;	//Assign U1Tx To Pin RP19(36)
		RPOR11bits.RP23R = 7;		//Assign SD1O To Pin RP23(3)
		RPOR4bits.RP8R = 8;	//Assign CLK To Pin RP8(44)
		
	// Configuration for Datalogger end	

	
	__builtin_write_OSCCONL(OSCCON | (1<<6));	// Lock Registers

	//configure GPIO here
	TRISAbits.TRISA2 = 0; // declaring pin as o/p pin LED_1
	TRISAbits.TRISA4 = 0; // declaring pin as o/p pin LED_2
	TRISCbits.TRISC5 = 0; // declaring pin as o/p pin LED_3
	TRISCbits.TRISC6 = 0; // declaring pin as o/p pin CS
	TRISBbits.TRISB6 = 1; // declaring pin as i/p for data transfer INT1

	
}
//End PPS Configuration
