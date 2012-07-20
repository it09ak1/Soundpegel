#include <avr/io.h>
#include <inttypes.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <math.h>

void updateLED()
{
	//PORTB = ~(def << i);
	PORTB = (uint8_t)(~(0b00000001));

}

ISR(INT0_vect)		// Routine für INT0
{
	//PORTB = (uint8_t)(~(0b00000001));
	//showLED = 0b00000100;
	//i++;
	updateLED();
	_delay_ms(2000);
	GIFR &= ~(1 << INTF0);	// weitere angemeldete Interrupts löschen (Entprellung)
}

int main(void)
{
	//uint8_t ledOut = 0b00000000;
	DDRB = 0xff;
	DDRD = 0x00;

	// Interrupts programmieren
	//GICR	|= ( (1<<INT0) | (1<<INT1) | (1<<INT2) );	// INT0, 1, 2 zulassen
	// INT0 und INT1 fallende Flanke
	//MCUCR	|= ( (1<<ISC01) | (1<<ISC11) );	// INT0, INT1: ISC01 und ISC11 setzen
	//MCUCR	&= ~( (1<<ISC00) | (1<<ISC10) );	// INT0, INT1: ISC00 und ISC10 löschen
	//MCUCSR	&= ~(1<<ISC2);	// INT2: Interrup Sense Control 2: fallende Flanke

	//sei();
	//ledOut = 0b00000000;

	while(1)
	{
		//if (PIND)
		//{
			//PORTB = PIND;

			//selectedPIN = PIND;

		if (PIND == 0b11111110)
		{
			PORTB = (uint8_t)(~(0b00000001));
		}
		else if (PIND == 0b11111101)
		{
			PORTB = (uint8_t)(~(0b00000011));
		}
		else if (PIND == 0b11111011)
		{
			PORTB = (uint8_t)(~(0b00000111));
		}
		else if (PIND == 0b11110111)
		{
			PORTB = (uint8_t)(~(0b00001111));
		}
		else if (PIND == 0b11101111)
		{
			PORTB = (uint8_t)(~(0b00011111));
		}
		else if (PIND == 0b11011111)
		{
			PORTB = (uint8_t)(~(0b00111111));
		}
		else if (PIND == 0b10111111)
		{
			PORTB = (uint8_t)(~(0b01111111));
		}
		else if (PIND == 0b01111111)
		{
			PORTB = (uint8_t)(~(0b11111111));
		}
		else
		{
			PORTB = (uint8_t)(~(0b00000000));
		}
		

		//}
		//else
		//{
			//PORTB = selectedPIN;
		//	PORTB = (uint8_t)(~(0b00000000));
		//}
		//ledOut = 0b00000000;		

		//for (int i = 0; i <= 7; i++)
		//{
		//	if (PIND > i)
		//	{
		//		ledOut = (0b00000001 << i);
		//	}
		//}
		
		//PORTB = (uint8_t)(~(ledOut));
	}

	return 0;
}
