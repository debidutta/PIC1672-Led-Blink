#include<htc.h>
#include<pic.h>
#define _XTAL_FREQ 8000000

//Configure i/o port connection
#define SEVENSEG_TRIS TRISB
#define SEVENSEG_PORT PORTB
//cinfiguration for display selection
#define SEVENSEG_SEL_TRIS TRISC
#define SEVENSEG_SEL_PORT PORTC

int digits[4]={0,0,0,0};

void wait(unsigned int delay)
{
for (;delay;delay--)
___delay_us(100);
}
void SevenSegment(int num)
{
switch(num)
{
case 0 :
SEVENSEG_PORT=0x3F;
break;
case 1 :
SEVENSEG_PORT=0x06;
break;
case 2 :
SEVENSEG_PORT=0B01011011;
break;
case 3 :
SEVENSEG_PORT=0B01001111;
break;
case 4 :
SEVENSEG_PORT=0B01100110;
break;
case 5 :
SEVENSEG_PORT=0B01101101;
break;
case 6 :
SEVENSEG_PORT=0b0011111;
break;
case 7 :
SEVENSEG_PORT=0B00000111;
break;
case 8 :
SEVENSEG_PORT=0b0011111;
break;
case 9 :
SEVENSEG_PORT=0b0011111;
break;
}
}
void SevenSegInit()
{
//initalize output port
SEVENSEG_TRIS=0x00;
SEVENSEG_SEL_TRIS=0x00;
SEVENSEG_SEL_PORT=0x01;

//Setting up Timer0
//Prescaler is divide by 256
PS0=1;
PS1=1;
PS2=1;
//Prescaler is assigned to Timer 0
PSA=0;
//Prescaler Clock Selection
T0CS=0;
//clear timer0 overflow flag
TMR0IF=0;
//Enabling Timer Overflow Interrupt
TMR0IE=1;
//Enable Peripherial Interrupt
PEIE=1;
//Enable Global Interrupt
GIE=1;
}
void SevenSegPrint(int num)
{
   /* 

   
   This function breaks apart a given integer into separete digits
   and writes them to the display array i.e. digits[]
   
   */
   int i=0;
   int j;
   if(num>9999) return;
   while(num)
   {
      digits[i]=num%10;
      i++;

      num=num/10;
   }
   for(j=i;j<4;j++) digits[j]=0;
}
void SevenSegISR()
{
   /*

   This interrupt service routine (ISR)
   Updates the displays

   */

   TMR0=150;

   static int i;

   if(i==3)
   {
      //If on last display then come
      //back to first.
      i=0;

   }
   else

   {
      //Goto Next display
      i++;
   }



   //Activate a display according to i
   SEVENSEG_SEL_PORT=((SEVENSEG_SEL_PORT & 0xf0)|(1<<i));

   //Write the digit[i] in the ith display.

   SevenSegment(digits[i]);
}
void interrupt ISR()
{
   //Check if it is TMR0 Overflow ISR
   if(TMR0IE && TMR0IF)
   {
      //Call the SevenSegISR
      SevenSegISR();

      //Clear Flag

      TMR0IF=0;
   }
}
void  main()
{
SevenSegInit();
SevenSegPrint(1234);
 while(1);
}