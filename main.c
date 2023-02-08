#define F_CPU 16000000UL
#define BAUD_RATE 9600
#define BAUD_PRESCALER (((F_CPU / (BAUD_RATE * 16UL))) - 1)

#define DOT_THRESH 400
#define SPACE_THRESH 400

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>

#include "uart.h"

char overflowed;
int lastEdge;
char out[25];
char sequence[5];
int len = 0;

void Initialize();
void print_char();
char get_char();

int main(void) {
	Initialize();
	while(1);
	return 0;
}

/*
	Function to initialize pins and registers to setup the interrupts
*/
void Initialize() {
	// Initialize serial communication to print to console
	UART_init(BAUD_PRESCALER);
	
	cli();
	
	// Set input and outputs
	DDRB |= (1 << DDB5);
	DDRB |= (1 << DDB4);
	
	DDRB &= ~(1 << DDB0);
	PORTB |= (1 << PORTB0);
	
	// Timer 1 Setup
	// Scale down clock by 1024
	TCCR1B |= (1 << CS10);
	TCCR1B &= ~(1 << CS11);
	TCCR1B |= (1 << CS12);
	
	// Timer 1 to normal mode
	TCCR1A &= ~(1 << WGM10);
	TCCR1A &= ~(1 << WGM11);
	TCCR1B &= ~(1 << WGM12);
	
	// Enable noise filter
	TCCR1B |= (1 << ICNC1);
	
	// Falling edge detection
	TCCR1B &= ~(1 << ICES1);
	
	// Enable interrupts for timer and overflow
	TIMSK1 |= (1 << ICIE1);
	TIMSK1 |= (1 << TOIE1);
	
	sei();
}

/* 
	Function to handle input capture.
	It basically finds the interval since the lastEdge detected and,
	depending on the button state, determines if we got a dot, dash, or space.
*/
ISR(TIMER1_CAPT_vect) {
	
	// Converts the 'tick' interval to milliseconds
	int interval = (int) (0.064 * (double)(ICR1));

	// Checks if it's been more than 5ms since the last input (for noise/debouncing)
	if (interval - lastEdge > 5 || interval - lastEdge < -5 || overflowed) {
		_delay_ms(10);

		if (PINB & (1 << PINB0)) {
			// Button has been unpressed
			if (!overflowed && interval < DOT_THRESH) {
				// Dot: Turn on red led
				PORTB |= (1 << PORTB4);
				PORTB &= ~(1 << PORTB5);
				sequence[len++] = '.';
			}
			else {
				// Dash: Turn on white led
				PORTB |= (1 << PORTB5);
				PORTB &= ~(1 << PORTB4);
				sequence[len++] = '_';
			}	
			
			// No encoding has length more than 5, so if we get to 5 we can safely print it
			// Also ensures we won't go past the size of the array
			if (len >= 5) print_char();
		} else {
			// Button has been pressed

			// Turn off both leds
			PORTB &= ~(1 << PORTB4);
			PORTB &= ~(1 << PORTB5);
			
			// If it's been more than SPACE_THRESH milliseconds or the timer has overflowed (and there is something in the sequence),
			// then we have reached a space, so print out the character
			if ((overflowed && len) || interval > SPACE_THRESH) print_char();
		}
		
		// Reset the counter, overflow flag, and change the edge detection
		TCNT1 = 0;
		overflowed = 0;
		TCCR1B ^= (1 << ICES1);
	}
	

	// Update the last edge time
	lastEdge = interval;
}

/* 
	Function to handle timer overflow.
	It sets the overflow variable to true and 
	prints the character, if any, to the console if the button is not pressed, 
	as that would mean that it's been unpressed for ~4s so it can be printed
*/
ISR (TIMER1_OVF_vect)
{
	overflowed = 1;
	
	if ((PINB & (1 << PINB0)) && len) print_char();
}


/* Function to print the character in the sequence to the terminal */
void print_char() {
	out[0] = get_char();
	out[1] = '\0';
	UART_putstring(out);
	len = 0;
}


/* 
	Function to get the corresponding character from the sequence of dots/dashes given the length
	It simply follows the Morse code tree to decode the sequence
*/
char get_char() {
	if (!len) return '\n';
	
	if (len == 1) {
		if (sequence[0] == '.') return 'E';
		return 'T';
	}
	if (len == 2) {
		if (sequence[0] == '.') {
			if (sequence[1] == '.') return 'I';
			return 'A';
		}

		if (sequence[1] == '.') return 'N';
		return 'M';
	}
	if (len == 3) {
		if (sequence[0] == '.') {
			if (sequence[1] == '.') {
				if (sequence[2] == '.') return 'S';
				return 'U';
			}
			if (sequence[2] == '.') return 'R';
			return 'W';
		}

		if (sequence[1] == '.') {
			if (sequence[2] == '.') return 'D';
			return 'K';
		}
		if (sequence[2] == '.') return 'G';
		return 'O';
	}
	if (len == 4) {
		if (sequence[0] == '.') {
			if (sequence[1] == '.') {
				if (sequence[2] == '.') {
					if (sequence[3] == '.') return 'H';
					return 'V';
				}
				if (sequence[3] == '.') return 'F';
				return '?';
			}

			if (sequence[2] == '.') {
				if (sequence[3] == '.') return 'L';
				return '?';
			}
			if (sequence[3] == '.') return 'P';
			return 'J';
		}
		if (sequence[1] == '.') {
			if (sequence[2] == '.') {
				if (sequence[3] == '.') return 'B';
				return 'X';
			}
			if (sequence[3] == '.') return 'C';
			return 'Y';
		}

		if (sequence[2] == '.') {
			if (sequence[3] == '.') return 'Z';
			return 'Q';
		}
		if (sequence[3] == '.') return '?';
		return '?';
		} if (len == 5) {
		if (sequence[0] == '.') {
			if (sequence[1] == '.') {
				if (sequence[2] == '.') {
					if (sequence[3] == '.') {
						if (sequence[4] == '.') {
							return '5';
						}
						return '4';
					}
					if (sequence[4] != '.') return '3';
				}
				if (sequence[3] != '.' && sequence[4] != '.') return '2';
			}
			if (sequence[2] != '.' &&sequence[3] != '.' && sequence[4] != '.') return '1';
		}
		if (sequence[1] != '.') {
			if (sequence[2] != '.') {
				if (sequence[3] != '.') {
					if (sequence[4] != '.') {
						return '0';
					}
					return '9';
				}
				if (sequence[4] == '.') return '8';
			}
			if (sequence[3] == '.' && sequence[4] == '.') return '7';
		}
		if (sequence[2] == '.' &&sequence[3] == '.' && sequence[4] == '.') return '6';
	}
	return '?';
}