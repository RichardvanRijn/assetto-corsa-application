#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include <stdbool.h>

char d = 0;
bool x = false;
uint8_t count = 0;
uint16_t waarde = 0;
uint16_t waardebuffer = 0;
uint16_t speed;
uint16_t RPM;
uint8_t gear;

char buffer[30];

void instellen232(void)
{
	UCSRA = 0x00;
    UCSRB = (1 << RXEN) | (1 << TXEN);
	UCSRB |= (1 << RXCIE) | (1 << RXEN) | (1 << TXEN);
    UCSRC = (1 << UCSZ0) | (1 << URSEL) | (1 << UPM1) | (1 << UCSZ1);
    UBRRH = 0;
    UBRRL = 26; //clock = 4MHZ	clock = 8MHZ
				//25			71
				//BAUD = 9600
}

ISR(USART_RXC_vect) { // ontvanger
	if (UCSRA & (1<<FE | 1<<DOR | 1<<PE)) {
        d = UDR;
        d = '?';
    }
	else if (x == true){
		if (count == 0){
			waarde = UDR;
			count = 1;
		}
		else {
			waardebuffer = UDR;
			count = 0;
			waardebuffer = (waardebuffer << 8);
			waarde += waardebuffer;
			x = false;
		}
	}
    else {
        d = UDR;
    }
}

void zendentekst(void){
	volatile static uint8_t i;
		for (i = 0; buffer[i] != '\0'; i++){
			while (~UCSRA & 1 << UDRE);
			UDR = buffer[i];
		}
}

int main(void)
{
	
	DDRA = 0xff;
	DDRB = 0x00;
	instellen232();
	sei();
	while(true){
		while (d == 0){
			if (~PINB & (1<<0))
			{
				snprintf(buffer, sizeof buffer, "A");
				zendentekst();
				while (~PINB & (1<<0)){
				}
			}
			else if (~PINB & (1<<7))
			{
				snprintf(buffer, sizeof buffer, "B");
				zendentekst();
				while (~PINB & (1<<7)){
				}
			}
		}
			cli();
			if (d == 'X')
			{
				snprintf(buffer, sizeof buffer, "Z");
				zendentekst();
				d = 0;
			}
			else if (d == 'A')
			{
				waarde = 0;
				x = true;
				sei();
				while (x == true);
				cli();
				RPM = waarde;
				waarde = 0;
				d = 0;
				PORTA = RPM;
			}
			else if (d == 'B')
			{
				waarde = 0;
				x = true;
				sei();
				while (x == true);
				cli();
				speed = waarde;
				waarde = 0;
				d = 0;
				PORTA = speed;
			}
		sei();
	}
}
