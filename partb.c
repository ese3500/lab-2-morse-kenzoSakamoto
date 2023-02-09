#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>


void Initialize() {
	cli();
	
	// Set input (pull-up) and output
	DDRB |= (1 << DDB5);
	DDRB &= ~(1 << DDB0);
	PORTB |= (1 << PORTB0);
	
	// Timer 1 Setup
	// Scale down clock
	TCCR1B &= ~(1 << CS10);
	TCCR1B |= (1 << CS11);
	TCCR1B &= ~(1 << CS12);
	
	// Timer 1 to normal
	TCCR1A &= ~(1 << WGM10);
	TCCR1A &= ~(1 << WGM11);
	TCCR1B &= ~(1 << WGM12);
	TCCR1B &= ~(1 << WGM13);
	
	// Rising edge detection
	TCCR1B &= ~(1 << ICES1);
	
	// Enable interrupt and flag
	TIFR1 |= (1 << ICF1);	
	TIMSK1 |= (1 << ICIE1);
	
	// Turn off led
	PORTB &= ~(1 << PORTB5);
	
	sei();
}

ISR(TIMER1_CAPT_vect) {	
	if (TCCR1B & (1 << ICES1)) {
		// Rising edge was detected
		PORTB &= ~(1 << PORTB5);
	} else {
		// Falling edge was detected
		PORTB |= (1 << PORTB5);
	}
	_delay_ms(250);
	
	// Toggle edge detection
	TCCR1B ^= (1 << ICES1);
}

int main(void) {
	Initialize();
	while(1);
}