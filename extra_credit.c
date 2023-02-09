#define F_CPU 16000000UL

#define MAX_LEN 25
#define LED_PIN DDB5
#define DASH 2
#define DOT 1
#define DELAY_DASH 700
#define DELAY_DOT 200

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <ctype.h>

void str_to_morse(char *str);
void char_to_morse(char c);
void blink(int symbol);

int main(void)
{
	DDRB |= (1 << LED_PIN);
	DDRB &= ~(1 << DDB0);
	PORTB |= (1 << PORTB0);	
	
	char message[MAX_LEN];
	sprintf(message, "ESE 350");
	
	while (PINB & (1 << PINB0));

	str_to_morse(message);

	while (1);
	return 0;
}

void str_to_morse(char *str) {
	while (*str) {
		char_to_morse(toupper((unsigned char) *str));
		str++;
	}
}

void char_to_morse(char c) {
	switch (c) {
		case 'A':
		blink(DOT);
		blink(DASH);
		break;
		case 'B':
		blink(DASH);
		blink(DOT);
		blink(DOT);
		blink(DOT);
		break;
		case 'C':
		blink(DASH);
		blink(DOT);
		blink(DASH);
		blink(DOT);
		break;
		case 'D':
		blink(DASH);
		blink(DOT);
		blink(DOT);
		break;
		case 'E':
		blink(DOT);
		break;
		case 'F':
		blink(DOT);
		blink(DOT);
		blink(DASH);
		blink(DOT);
		break;
		case 'G':
		blink(DASH);
		blink(DASH);
		blink(DOT);
		break;
		case 'H':
		blink(DOT);
		blink(DOT);
		blink(DOT);
		blink(DOT);
		break;
		case 'I':
		blink(DOT);
		blink(DOT);
		break;
		case 'J':
		blink(DOT);
		blink(DASH);
		blink(DASH);
		blink(DASH);
		break;
		case 'K':
		blink(DASH);
		blink(DOT);
		blink(DASH);
		break;
		case 'L':
		blink(DOT);
		blink(DASH);
		blink(DOT);
		blink(DOT);
		break;
		case 'M':
		blink(DASH);
		blink(DASH);
		break;
		case 'N':
		blink(DASH);
		blink(DOT);
		break;
		case 'O':
		blink(DASH);
		blink(DASH);
		blink(DASH);
		break;
		case 'P':
		blink(DOT);
		blink(DASH);
		blink(DASH);
		blink(DOT);
		break;
		case 'Q':
		blink(DASH);
		blink(DASH);
		blink(DOT);
		blink(DASH);
		break;
		case 'R':
		blink(DOT);
		blink(DASH);
		blink(DOT);
		break;
		case 'S':
		blink(DOT);
		blink(DOT);
		blink(DOT);
		break;
		case 'T':
		blink(DASH);
		break;
		case 'U':
		blink(DOT);
		blink(DOT);
		blink(DASH);
		break;
		case 'V':
		blink(DOT);
		blink(DOT);
		blink(DOT);
		blink(DASH);
		break;
		case 'W':
		blink(DOT);
		blink(DASH);
		blink(DASH);
		break;
		case 'X':
		blink(DASH);
		blink(DOT);
		blink(DOT);
		blink(DASH);
		break;
		case 'Y':
		blink(DASH);
		blink(DOT);
		blink(DASH);
		blink(DASH);
		break;
		case 'Z':
		blink(DASH);
		blink(DASH);
		blink(DOT);
		blink(DOT);
		break;
		case '1':
		blink(DOT);
		blink(DASH);
		blink(DASH);
		blink(DASH);
		blink(DASH);
		break;
		case '2':
		blink(DOT);
		blink(DOT);
		blink(DASH);
		blink(DASH);
		blink(DASH);
		break;
		case '3':
		blink(DOT);
		blink(DOT);
		blink(DOT);
		blink(DASH);
		blink(DASH);
		break;
		case '4':
		blink(DOT);
		blink(DOT);
		blink(DOT);
		blink(DOT);
		blink(DASH);
		break;
		case '5':
		blink(DOT);
		blink(DOT);
		blink(DOT);
		blink(DOT);
		blink(DOT);
		break;
		case '6':
		blink(DASH);
		blink(DOT);
		blink(DOT);
		blink(DOT);
		blink(DOT);
		break;
		case '7':
		blink(DASH);
		blink(DASH);
		blink(DOT);
		blink(DOT);
		blink(DOT);
		break;
		case '8':
		blink(DASH);
		blink(DASH);
		blink(DASH);
		blink(DOT);
		blink(DOT);
		break;
		case '9':
		blink(DASH);
		blink(DASH);
		blink(DASH);
		blink(DASH);
		blink(DOT);
		break;
		case '0':
		blink(DASH);
		blink(DASH);
		blink(DASH);
		blink(DASH);
		blink(DASH);
		break;
}
_delay_ms(DELAY_DASH);
}

void blink(int symbol) {
	PORTB |= (1 << LED_PIN);
	if (symbol == DASH) _delay_ms(DELAY_DASH);
	else if (symbol == DOT) _delay_ms(DELAY_DOT);
	PORTB &= ~(1 << LED_PIN);
	 _delay_ms(DELAY_DOT);
}