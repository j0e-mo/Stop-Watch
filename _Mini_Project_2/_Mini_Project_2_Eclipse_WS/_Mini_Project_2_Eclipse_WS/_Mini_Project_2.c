/*
 * _Mini_Project_2.c
 *
 *  Created on: May 12, 2024
 *      Author: Joe Metwally
 */

/****************_HEADER FILE INCLUSION SECTION_************************/
#include "_Mini_Project_2.h"


/****************_GLOBAL VARIABLE INITIALIZATION SECTION_************************/
unsigned char seconds = 0;
unsigned char minutes = 0;
unsigned char hours = 0;
/*INITIALIZE A COUNTER FOR EACH DIGIT*/


/****************_MODULE INITIALIZATION SECTION_************************/
void Enable_Extern_Interrupts(void) {
	DDRD &= ~(1 << PD2);
	/*CONFIGURE INT0 PIN AS INPUT PIN*/
	PORTD |= (1 << PD2);
	/*TURN ON INTERNAL PULL UP RESISTANCE FOR INT0*/
	DDRD &= ~(1 << PD3);
	/*CONFIGURE INT1 PIN AS INPUT PIN*/
	DDRD &= ~(1 << PB2);
	/*CONFIGURE INT2 PIN AS INPUT PIN*/
	PORTB |= (1 << PB2);
	/*TURN ON INTERNAL PULL UP RESISTANCE FOR INT2*/

	MCUCR = (1 << ISC11) | (1 << ISC10) | (1 << ISC01);
	/*CONFIGURE INT1 SENSE CONTROL TRIGGER AT RISING EDGE*/
	/*CONFIGURE INT0 SENSE CONTROL TRIGGER AT FALLING EDGE*/
	MCUCSR = (1 << ISC2);
	/*CONFIGURE INT2 SENSE CONTROL TRIGGER AT FALLING EDGE*/
	GICR = (1 << INT1) | (1 << INT0) | (1 << INT2);
	/*ENABLE INTERRUPT REQUEST FOR IN0, INT1, AND INT2*/
	SREG |= (1 << 7);
	/*GLOBAL INTERRUPT ENABLE*/
}

void Timer1_CTC_Init(void) {
	TCCR1A = (1 << FOC1A) | (1 << FOC1B);
	/*CONFIGURE TIMER1 CTC MODE AS TARGET SPECIFICATION SHEET SPECIFIES*/
	TCCR1B = (1 << WGM12) | (1 << CS10) | (1 << CS12);
	/*SET THE CTC MODE AND SELECT 1024 PRE-SCALER*/
	TIMSK |= (1 << OCIE1A);
	/*TRIGGER TIMER 1 INTERRUPTS WHEN OCR1A COMPARE MATCH OCCURS*/
	OCR1A = 977;
	/*SET THE COMPARE VALUE TO ACCURATELY REPRESENT 1 SECOND*/
	TCNT1 = 0;
	/*START THE TIMER AT 0*/
}

void Seven_Segment_Init(void) {
	DDRC |= 0x0F;
	/*CONFIGURE OUTPUT FROM THE MC TO THE 7-SEGMENT DECODER*/
	PORTC &= (~0x0F);
	/*INITIALIZE OUTPUT AS 00:00:00*/
	DDRA |= 0x3F;
	/*CONFIGURE MULTIPLEX MODE FOR 7-SEGMENT*/
	PORTA |= 0x3F;
	/*SELECT ALL 7-SEGMENTS TO DISPLAY 00:00:00*/
}


/****************_ISR SECTION_************************/
ISR(INT0_vect) {
	hours = 0;
	minutes = 0;
	seconds = 0;
	/*IF INT0 IS TRIGGERED, RESET THE STOPWATCH*/
}

ISR(INT1_vect) {
	TCCR1B &= ~(1 << CS10);
	TCCR1B &= ~(1 << CS12);
	/*IF INT1 IS TRIGGERED, STOPWATCH IS PAUSED*/
}

ISR(INT2_vect) {
	TCCR1B |= (1 << CS10);
	TCCR1B |= (1 << CS12);
	/*IF INT2 IS TRIGGERED, STOPWATCH IS STARTED*/
}

ISR(TIMER1_COMPA_vect) {
		seconds++;
		/*INCREMENT SECONDS COUNTER AT EACH TIMER1 COMPARE MATCH*/
		if (seconds == 60) {
			seconds = 0;
			minutes++;
			/*IF WE COUNTED 60 SECONDS, WE SHALL COUNT A MINUTE AND RESET THE SECONDS COUNTER*/
		}
		if (minutes == 60) {
			seconds = 0;
			minutes = 0;
			hours++;
			/*IF WE COUNTED 60 MINUTES, WE SHALL COUNT AN HOUR AND RESET THE MINUTES COUNTER*/
		}
		if (hours == 24) {
			seconds = 0;
			minutes = 0;
			hours = 0;
			/*IF WE COUNTED 24 HOURS, WE SHALL RESTART THE COUNTER*/
		}
}


/****************_MAIN APPLICATION SECTION_************************/
int main(void) {
	Enable_Extern_Interrupts();
	Timer1_CTC_Init();
	Seven_Segment_Init();
	/*WE CALL ALL MODULE INITIALIZATIONS*/

	for (;;) {
		PORTA = (PORTA & 0xC0) | 0x01;
		PORTC = (PORTC & 0xF0) | ((seconds % 10) & 0x0F);
		_delay_ms(3);
		PORTA = (PORTA & 0xC0) | 0x02;
		PORTC = (PORTC & 0xF0) | ((seconds / 10) & 0x0F);
		_delay_ms(3);
		PORTA = (PORTA & 0xC0) | 0x04;
		PORTC = (PORTC & 0xF0) | ((minutes % 10) & 0x0F);
		_delay_ms(3);
		PORTA = (PORTA & 0xC0) | 0x08;
		PORTC = (PORTC & 0xF0) | ((minutes / 10) & 0x0F);
		_delay_ms(3);
		PORTA = (PORTA & 0xC0) | 0x10;
		PORTC = (PORTC & 0xF0) | ((hours % 10) & 0x0F);
		_delay_ms(3);
		PORTA = (PORTA & 0xC0) | 0x20;
		PORTC = (PORTC & 0xF0) | ((hours / 10) & 0x0F);
		_delay_ms(3);
		/*WE USE THE MULTIPLEX METHOD TO ROUND ROBIN THE DISPLAY OF OUR COUNTERS TO ACCURATELY DISPLAY TIME*/
	}
}
