#include <stdlib.h>
#include <avr/io.h> 
#include <avr/interrupt.h>

#define F_CPU 12000000

#define DESIRED_FREQUENCY F_CPU

#define DESIRED_DELAY 0.05

/******** Prescale ********************************************/
#define PRESCALE0		1
#define PRESCALE8		8
#define PRESCALE64 		64
#define PRESCALE256 		256
#define PRESCALE1024 		1024

#define PRESCALE0_BIT		(1<<CS10)
#define PRESCALE8_BIT		(1<<CS11)
#define PRESCALE64_BIT 		((1<<CS11)|(1<<CS10))
#define PRESCALE256_BIT 	(1<<CS12)
#define PRESCALE1024_BIT 	((1<<CS12)|(1<<CS10))
int TIMER_COUNT;
/**************************************************************/


/** Predifined Function : ISR
 \n Description : Function defined under "avr/interrupt.h" is used to call the Interrupt service routine.
 \n
 \n Note : this method is used to check Timer Interrupt, and on interrupt data on PORTB1 is modified which caused the PinInterrupt and on PinInterrupt data is sent.
 */
ISR(TIMER1_OVF_vect)
{
	PORTB ^= _BV(1);
	TCNT1=TIMER_COUNT;
}


/** Predifined Function : ISR
 \n Description : Function defined under "avr/interrupt.h" is used to call the Interrupt service routine.
 \n
 \n Note : this method is used as Pin Interrupt on PortB, and on interrupt, "Send()" function is called.
 */
ISR(PCINT0_vect)
{
	Send();
}


/** Predifined Function : ISR
 \n Description : Function defined under "avr/interrupt.h" is used to call the Interrupt service routine.
 \n
 \n Note : this method is used ad Pin Interrupt on PortD, and on interrupt, "Recieve()" function is called.
 */
ISR(PCINT2_vect) //Rx
{
	Recieve();
}


/** Function : CalculatePrescalar
 \n Description : This function is used to calculate and return the prescalar based on the Desired Frequency and Delay.
 \n
 \n Input - None
 \n
 \n Output - Returns calculated prescalar value
 */
int CalculatePrescalar()
{
	int iPrescalar= 0;

	if((DESIRED_DELAY * DESIRED_FREQUENCY  / PRESCALE0) <= 65535)
		iPrescalar=PRESCALE0;
	else if((DESIRED_DELAY * DESIRED_FREQUENCY  / PRESCALE8) <= 65535)
		iPrescalar=PRESCALE8;
	else if((DESIRED_DELAY * DESIRED_FREQUENCY / PRESCALE64) <= 65535)
		iPrescalar=PRESCALE64;
	else if((DESIRED_DELAY * DESIRED_FREQUENCY / PRESCALE256) <= 65535)
		iPrescalar=PRESCALE256;
	else 
		iPrescalar=PRESCALE1024;
	
	return iPrescalar;
}


/** Function : InitTimer1Interrupt
 \n Description : This function is used to initialize Timer 1.
 \n Values based on the calculated prescalar from "CalculatePrescalar()" will be set on respective Registers.
 \n
 \n Input - None
 \n
 \n Output - None
 */
void InitTimer1Interrupt()
{
	int prescaler=CalculatePrescalar();

	TIMER_COUNT =	(65536-((int)(DESIRED_DELAY * DESIRED_FREQUENCY / prescaler)-1));

	if(prescaler == PRESCALE0)
		TCCR1B |=	PRESCALE0_BIT;		//Setting the prescaler
	else if(prescaler == PRESCALE8)
		TCCR1B |=	PRESCALE8_BIT;
	else if(prescaler == PRESCALE64)
		TCCR1B |=	PRESCALE64_BIT;
	else if(prescaler == PRESCALE256)
		TCCR1B |=	PRESCALE256_BIT;
	else //if(prescaler == PRESCALE1024)
		TCCR1B |=	PRESCALE1024_BIT;
	
	//Initialize Counter
	TCNT1=TIMER_COUNT;			

	//Enable Overflow interrupt
	TIMSK1 |= (1<< TOIE1);

	//Enable global interrupt
	sei();
}

/** Function : InitPinInterrupt
 \n Description : This function is used to initialize PinInterrupts.
 \n Based on PORTs/PINs respective registers are set.
 \n
 \n Input - None
 \n
 \n Output - None
 */
void InitPinInterrupt()
{
	PCICR |= (1<< PCIE0);			//Using mask 0, Since PB1 is in MSK0
	PCMSK0 |= (1<< PCINT1);

	PCICR |= (1<< PCIE2);			//Using mask 2, Since PD3 is in MSK2
	PCMSK2 |= (1<< PCINT19);
}

/** Function : InitTimer_Pin_Interrupts
 \n Description : This function is used for Function call of "InitPinInterrupt()" and "InitTimer1Interrupt()" from other Files
 \n Input - None
 \n
 \n Output - None
 */
void initTimer()
{
	//Initialize the Timer1 Interrupts
	InitTimer1Interrupt();
	//Initialize the Pin Interrupts
	InitPinInterrupt();
}


