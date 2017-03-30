#include<p33FJ128MC804.h>
/*
int configure_PWM(void)
{
//PxTCON register
	{
		//PWM Time Base is On
		PxTCONbits.PTE1 = 1;	
		//Time Base O/P Postscale <3:0>
		//1111->1:16 ..... 0000->1:1
		PxTCONbits.PTOPS  =;
		//PWM Time Base Input Clock Prescale<1:0>
		//11->1:64 ..... 00->1:1
		PxTCONbits.PTCKPS =;
		//PWM mode select
		// 11->PWM up/down with double interrupt
		// 10->PWM continuous up/down
		// 01->Single Event mode
		// 00->Free running mode
		PxTCONbits.PTMOD =;		
	}	
//PxTMR register
	{
		//PWM TIMER BASE Register Count Value Bits <14:0>
		PxTMRbits.PTMR;
	}
//PxTPER register
	{
		//PWM Time Base Period Value bits<14:0>
		PxTPERbits.PTPER=;
	}
//PWMxCON1 (write protected, to write PWMLOCK = 0, FOSCSELbits.PWMLOCK = 0)
	{
		//Removing write protection
		FOSCSELbits.PWMLOCK = 0; 
		
		//Output mode select 1->independent o/p mode , 0->complementary o/p mode
		PWMxCON1bits.PMOD1=1;
		PWMxCON1bits.PMOD1=1;
		PWMxCON1bits.PMOD1=1;
		PWMxCON1bits.PMOD1=1;
		//Pins enable for PWM 1->PWM , 0->GPIO
		PWMxCON1bits.PEN3H=1;
		PWMxCON1bits.PEN3L=1;
		PWMxCON1bits.PEN2H=1;
		PWMxCON1bits.PEN2L=1;
		PWMxCON1bits.PEN1H=1;
		PWMxCON1bits.PEN1L=1;
	}
//PxDCn register 
	{
		//PWM DutyCycle value bits<15:0>
		P1DC1=;
		P1DC2=;	
		P1DC3=;	
		P2DC1=;
	}
}
*/


int configure_PWM(void)
{
//PxTCON register
	{
		//Time Base O/P Postscale <3:0>
		//1111->1:16 ..... 0000->1:1
		P1TCONbits.PTOPS  =0; //useful when the PWM duty cycles need not be updated every PWM cycle
		//PWM modele #2
		P2TCONbits.PTOPS  =0; //useful when the PWM duty cycles need not be updated every PWM cycle
		//PWM Time Base Input Clock Prescale<1:0>
		//11->1:64 ..... 00->1:1
		P1TCONbits.PTCKPS =3;
		//PWM modele #2
		P2TCONbits.PTCKPS =3;
		//PWM mode select
		// 11->PWM up/down with double interrupt
		// 10->PWM continuous up/down
		// 01->Single Event mode
		// 00->Free running mode
		P1TCONbits.PTMOD = 0;
		//PWM modele #2
		P1TCONbits.PTMOD = 0;
	}	
//PxTMR register
	{
		//PWM TIMER BASE Register Count Value Bits <14:0> // this is the counter register..
		//so sud be initialized wid 0 mostely 
		P1TMRbits.PTMR=0x00;
		//PWM module #2
		P1TMRbits.PTMR=0x00;
	}
//P1TPER register
	{
		//PWM Time Base Period Value bits<14:0>
		P1TPERbits.PTPER=0x30D3; // for 20 ms time period
		//PWM module #2
		P2TPERbits.PTPER=0x30D3; // for 20 ms time period
	}
//P1DCn and P2DC1 register 
	{
		//PWM DutyCycle value bits<15:0>
		P1DC1=0x1FFF;	//50%
		P1DC2=0x2FFF;	//50%	
		P1DC3=0x3FFF;	//50%
		P2DC1=0x4FFF;	//50%
	}
//PWM I/O pin controlled by PWM generator
	{
		P1OVDCONbits.POVD3H = 1;
		P1OVDCONbits.POVD2H = 1;
		P1OVDCONbits.POVD1H = 1;
		P1OVDCONbits.POVD3L = 1;
		P1OVDCONbits.POVD2L = 1;
		P1OVDCONbits.POVD1L = 1;
		//PWM module #2
		P2OVDCONbits.POVD1H = 1;
		P2OVDCONbits.POVD1L = 1;
		

	}
//PWM1CON1 (write protected, to write PWMLOCK = 0, FOSCSELbits.PWMLOCK = 0)
	{
		//Removing write protection
		//_FOSCSEL.PWMLOCK = 0; this is not how it is done.. read the manual carefully b4 implementing
		
		//Output mode select 1->independent o/p mode , 0->complementary o/p mode
		PWM1CON1bits.PMOD1=1;
		PWM1CON1bits.PMOD2=1;
		PWM1CON1bits.PMOD3=1;
		//PWM module #2
		PWM2CON1bits.PMOD1=1;		

		//Pins enable for PWM 1->PWM , 0->GPIO
		PWM1CON1bits.PEN3H=1;
		PWM1CON1bits.PEN3L=1;
		PWM1CON1bits.PEN2H=1;
		PWM1CON1bits.PEN2L=1;
		PWM1CON1bits.PEN1H=1;
		PWM1CON1bits.PEN1L=1;
		//PWM module #2
		PWM2CON1bits.PEN1H=1;
		PWM2CON1bits.PEN1L=1;

	}
	//PWM Time Base is On
	P1TCONbits.PTEN = 1;		
	P2TCONbits.PTEN = 1;		

}