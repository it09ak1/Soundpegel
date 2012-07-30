#include <avr/io.h>
#include <inttypes.h>
#include <avr/interrupt.h>
//#define F_CPU 16000000
#include <util/delay.h>
#include <math.h>
#include <avr/eeprom.h>

volatile uint8_t programm = 0;
volatile uint8_t maxVumeter = 0b00000000;
volatile uint8_t vumeterCounter = 0;
// Variable im EEPROM
uint8_t eeProgramm EEMEM;
uint8_t eeMaxVumeter EEMEM;
uint8_t eeVumeterCounter EEMEM;

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
	ADMUX = (1<<REFS0) | (1<<REFS1);
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

uint16_t MittelwertADC()
{
	uint16_t sum;
	uint16_t mittelwert;

	for (int i=0; i<=29;i++)
	{
		uint16_t singleValue = ReadADC();

		sum += singleValue;
	}

	mittelwert = sum/30;

	return mittelwert;
}

int main(void)
{
	//uint8_t ledOut = 0b00000000;
	DDRB = 0xff;
	DDRD = 0x00;
	DDRA = 0x00;
	programm = eeprom_read_byte(&eeProgramm);				// Programm aus EEPROM lesen
	maxVumeter = eeprom_read_byte(&eeMaxVumeter);			// Maximalen Wert aus EEPROM lesen
	vumeterCounter = eeprom_read_byte(&eeVumeterCounter);	// Maximalen Wert aus EEPROM lesen

	InitADC();
	PORTB = (uint8_t)(~(0b00000000));

	// warum uint16_t und nicht unsigned int:
	// uint16_t erhoeht die portabilitaet des Codes
	// wurede man nur unsigned int waere der Datentype
	// auf einem x86 = 32bit und auf x64=64bit

	uint16_t value = 0;
	//int pinD = PIND;
	uint16_t maxValue = 0;
	uint16_t abzugswert = 0;
	

	abzugswert = MittelwertADC();

	while(1)
	{
		value = ReadADC();
		value = value - abzugswert;
		value = abs(value) * 5;
		uint8_t vumeter = 0b00000000;
		
		// programm Auswahl
		if (PIND == 0b11111110)
		{
			PORTB = PIND;
			programm = 1;
			eeprom_write_byte(&eeProgramm, programm);	// im EEPROM sichern
		}
		else if (PIND == 0b11111101)
		{
			PORTB = PIND;
			programm = 2;
			eeprom_write_byte(&eeProgramm, programm);	// im EEPROM sichern
		}
		else if (PIND == 0b11111011)
		{
			PORTB = PIND;
			programm = 3;
			eeprom_write_byte(&eeProgramm, programm);	// im EEPROM sichern
		}
		else if (PIND == 0b01111111)
		{
			// loeschen der Werte	
			PORTB = PIND;
			maxVumeter = 0b00000000;
			maxValue = 0;
			vumeterCounter = 0;
			//programm = 0;
			abzugswert = MittelwertADC();
			//PORTB = (uint8_t)(~(0b00000000));
		}

		if (programm == 1)
		{
			
				if (value > 50) {
					vumeter |= 0b00000001;
				}
				if (value > 228) {
					vumeter |= 0b00000010;
				}
				if (value > 406) {
					vumeter |= 0b00000100;
				}
				if (value > 584) {
					vumeter |= 0b00001000;
				}
				if (value > 762) {
					vumeter |= 0b00010000;
				}
				if (value > 940) {
					vumeter |= 0b00100000;
				}
				if (value > 1118) {
					vumeter |= 0b01000000;
				}
				if (value > 1300) {
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
				if (value > 50) {
					maxVumeter |= 0b00000001;
					eeprom_write_byte(&eeMaxVumeter, maxVumeter);	// im EEPROM sichern
				}
				if (value > 228) {
					maxVumeter |= 0b00000010;
					eeprom_write_byte(&eeMaxVumeter, maxVumeter);	// im EEPROM sichern
				}
				if (value > 406) {
					maxVumeter |= 0b00000100;
					eeprom_write_byte(&eeMaxVumeter, maxVumeter);	// im EEPROM sichern
				}
				if (value > 584) {
					maxVumeter |= 0b00001000;
					eeprom_write_byte(&eeMaxVumeter, maxVumeter);	// im EEPROM sichern
				}
				if (value > 762) {
					maxVumeter |= 0b00010000;
					eeprom_write_byte(&eeMaxVumeter, maxVumeter);	// im EEPROM sichern
				}
				if (value > 940) {
					maxVumeter |= 0b00100000;
					eeprom_write_byte(&eeMaxVumeter, maxVumeter);	// im EEPROM sichern
				}
				if (value > 1118) {
					maxVumeter |= 0b01000000;
					eeprom_write_byte(&eeMaxVumeter, maxVumeter);	// im EEPROM sichern
				}
				if (value > 1300) {
					maxVumeter |= 0b10000000;
					eeprom_write_byte(&eeMaxVumeter, maxVumeter);	// im EEPROM sichern
				}

				maxValue = value;
			}
			
			PORTB = (uint8_t)(~(maxVumeter));

		}
		else if (programm == 3)
		{
			if (value > 300)
			{
				vumeterCounter++;
				eeprom_write_byte(&eeVumeterCounter, vumeterCounter);	// im EEPROM sichern				
				PORTB = (uint8_t)(~(vumeterCounter));
				_delay_ms(200);
			}

			PORTB = (uint8_t)(~(vumeterCounter));
		}

		_delay_ms(20);
		
	}

	return 0;
}
