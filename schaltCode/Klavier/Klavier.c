#include <avr/io.h>
#include <inttypes.h>
#include <avr/interrupt.h>
//#define F_CPU 16000000
#include <util/delay.h>
#include <math.h>


void InitADC()
{
	//PORTA = 0;
	PORTA = 0;
	// MUX0-4 setzen (Datenhandbuch Seite 219)
	// ADC1 und ADC 0 (diese haben wir jetzt genommen)
	//ADMUX |= (1<<REFS0) | (1<<ADLAR) | (1<<MUX3) | (1<<MUX1) | (1<<MUX0);
	// 10x Verstaerkung
	//ADMUX |= (1<<REFS0) | (1<<MUX3) | (1<<MUX0);
	// ohne Verstaerkung
	ADMUX = (1<<REFS0);
	// ADC3 und ADC 2
	//ADMUX |= (1<<REFS0) | (1<<ADLAR) | (1<<MUX3) | (1<<MUX2) | (1<<MUX1) | (1<<MUX10);
	ADCSRA = (1 << ADEN) | (1 << ADPS1) | (1 << ADPS0) | ( 0 << ADPS2);
}

uint16_t ReadADC()
{
	ADCSRA |= (1 << ADSC);
	while (!(ADCSRA & (1 << ADIF)));

	ADCSRA |= (1 << ADIF);

	return(ADC);
}

int main(void)
{
	//uint8_t ledOut = 0b00000000;
	DDRB = 0xff;
	DDRD = 0x00;
	DDRA = 0x00;

	InitADC();
	PORTB = (uint8_t)(~(0b00000000));
	//PORTB = (uint8_t)(~(0b00000001));

	// Interrupts programmieren
	//GICR	|= ( (1<<INT0) | (1<<INT1) | (1<<INT2) );	// INT0, 1, 2 zulassen
	// INT0 und INT1 fallende Flanke
	//MCUCR	|= ( (1<<ISC01) | (1<<ISC11) );	// INT0, INT1: ISC01 und ISC11 setzen
	//MCUCR	&= ~( (1<<ISC00) | (1<<ISC10) );	// INT0, INT1: ISC00 und ISC10 löschen
	//MCUCSR	&= ~(1<<ISC2);	// INT2: Interrup Sense Control 2: fallende Flanke

	//sei();
	//ledOut = 0b00000000;

	int value = 0;
	//int pinD = PIND;
	int programm = 0;
	int maxValue = 0;
	uint8_t maxVumeter = 0b00000000;

	while(1)
	{
		value = ReadADC();
		value = value - 820;
		value = abs(value);
		uint8_t vumeter = 0b00000000;
		
		// BA Config
		/*
		if (value > 50) {
			vumeter |= 0b00000001;
		}
		if (value > 55) {
			vumeter |= 0b00000010;
		}
		if (value > 60) {
			vumeter |= 0b00000100;
		}
		if (value > 65) {
			vumeter |= 0b00001000;
		}
		if (value > 70) {
			vumeter |= 0b00010000;
		}
		if (value > 75) {
			vumeter |= 0b00100000;
		}
		if (value > 85) {
			vumeter |= 0b01000000;
		}
		if (value > 100) {
			vumeter |= 0b10000000;
		}*/
		
		if (PIND == 0b11111110)
		{
			PORTB = PIND;
			programm = 1;
		}
		else if (PIND == 0b11111101)
		{
			PORTB = PIND;
			programm = 2;
		}
		else if (PIND == 0b01111111)
		{
			// loeschen der Werte	
			PORTB = PIND;
			maxVumeter = 0b00000000;
			maxValue = 0;
			programm = 0;
		}

		if (programm == 1)
		{
			// home work
			// eifach reinpusten und sehen wie hoch es steigt
			if (value > 200) {
					vumeter |= 0b00000001;
				}
				if (value > 275) {
					vumeter |= 0b00000010;
				}
				if (value > 305) {
					vumeter |= 0b00000100;
				}
				if (value > 335) {
					vumeter |= 0b00001000;
				}
				if (value > 365) {
					vumeter |= 0b00010000;
				}
				if (value > 395) {
					vumeter |= 0b00100000;
				}
				if (value > 425) {
					vumeter |= 0b01000000;
				}
				if (value > 555) {
					vumeter |= 0b10000000;
				}
		
			//value = ReadADC();
			//value -=850;
			//value = abs(value);
			//PORTB = (uint8_t)(~(value));
			//PORTB = (uint8_t)(~(ReadADC()));
			PORTB = (uint8_t)(~(vumeter));
		}
		else if (programm == 2)
		{
			// maximalen pust wert speichern
			// wenn maximal Pust wert groesser als der letzte ist
			if (value > maxValue)
			{
				/*
				if (value > 40) {
					vumeter |= 0b00000001;
				}
				if (value > 55) {
					vumeter |= 0b00000010;
				}
				if (value > 85) {
					vumeter |= 0b00000100;
				}
				if (value > 100) {
					vumeter |= 0b00001000;
				}
				if (value > 135) {
					vumeter |= 0b00010000;
				}
				if (value > 170) {
					vumeter |= 0b00100000;
				}
				if (value > 190) {
					vumeter |= 0b01000000;
				}
				if (value > 200) {
					vumeter |= 0b10000000;
				}*/

				if (value > 200) {
					//vumeter |= 0b00000001;
					maxVumeter |= 0b00000001;
				}
				if (value > 215) {
					//vumeter |= 0b00000010;
					maxVumeter |= 0b00000010;
				}
				if (value > 355) {
					//vumeter |= 0b00000100;
					maxVumeter |= 0b00000100;
				}
				if (value > 425) {
					//vumeter |= 0b00001000;
					maxVumeter |= 0b00001000;
				}
				if (value > 565) {
					//vumeter |= 0b00010000;
					maxVumeter |= 0b00010000;
				}
				if (value > 665) {
					//vumeter |= 0b00100000;
					maxVumeter |= 0b00100000;
				}
				if (value > 765) {
					//vumeter |= 0b01000000;
					maxVumeter |= 0b01000000;
				}
				if (value > 845) {
					//vumeter |= 0b10000000;
					maxVumeter |= 0b10000000;
				}

				maxValue = value;
			}

			PORTB = (uint8_t)(~(maxVumeter));
		}

		_delay_ms(200);
		
	}

	return 0;
}
