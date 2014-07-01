#include<htc.h>
#include<pic.h>
#define _XTAL_FREQ 4000000
void ADC_int()
{
	ADCON0=0x80;
	ADCON1=0x00;//All pins are analog input
				//with reference voltages VDD
}
unsigned int ADC_Read(unsigned char channel)
{
	if (channel>4)//if invalid channel is selected
	return 0;//Return 0
	ADCON0 &=0X00;//Clearing the Channel selection bits
	ADCON0=channel<<2;//setting the required bits
	ADON=1;
	__delay_ms(10);//Acquisition time to charge hold capacitor
	GODONE = 1;//intilize A/D conversion
	while(GODONE);//wait for A/D Conversion to complet
	ADON=0; 
	return ADRES;//Return result
}

void main()
{
	unsigned int a;
	TRISB=0x00;//PORTB as output
	TRISC=0x00;//PORTC as output
	TRISA=0xFF;//PORTA as input
	ADC_int();//Initializes ADC Module
do
{
	a=ADC_Read(0);//Reading Analog channel 0
	PORTB = a;//Lower 8 bits to PORTB
	
	__delay_ms(10);
}while(1);//infinite loop
}