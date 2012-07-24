#include <avr/io.h>
#include <inttypes.h>
#include <avr/interrupt.h>
//#define F_CPU 16000000
#include <util/delay.h>
#include <math.h>


void InitADC()
{
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
	//DDRD = 0x00;
	DDRA = 0x00;

	InitADC();
	PORTD = 0;
	PORTB = (uint8_t)(~(0b00000001));

	// Interrupts programmieren
	//GICR	|= ( (1<<INT0) | (1<<INT1) | (1<<INT2) );	// INT0, 1, 2 zulassen
	// INT0 und INT1 fallende Flanke
	//MCUCR	|= ( (1<<ISC01) | (1<<ISC11) );	// INT0, INT1: ISC01 und ISC11 setzen
	//MCUCR	&= ~( (1<<ISC00) | (1<<ISC10) );	// INT0, INT1: ISC00 und ISC10 löschen
	//MCUCSR	&= ~(1<<ISC2);	// INT2: Interrup Sense Control 2: fallende Flanke

	//sei();
	//ledOut = 0b00000000;

	int value = 0;

	while(1)
	{

		value = ReadADC();
		value = value - 820;
		value = abs(value);
		uint8_t vumeter = 0b00000000;
		
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
		}
		
		//value = ReadADC();
		//value -=850;
		//value = abs(value);
		//PORTB = (uint8_t)(~(value));
		//PORTB = (uint8_t)(~(ReadADC()));
		PORTB = (uint8_t)(~(vumeter));
		
		_delay_ms(100);

		//if (ReadADC() > 1000)
		//{
			//PORTB = (PORTB << 1);	
		//	_delay_ms(2000);
		//}
		//if (PIND)
		//{
			//PORTB = PIND;

			//selectedPIN = PIND;

		/*
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
		*/
		

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
