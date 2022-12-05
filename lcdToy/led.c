#include <msp430.h>
#include "led.h"

void ledInit()
{
  P1DIR |= LEDS;
  P1OUT &= ~LEDS;
}

void toggleGreen(unsigned char flag)
{
  if (flag) {
    P1OUT |= LED_GREEN;
  } else {
    P1OUT &= ~LED_GREEN;
  }
}

void toggleRed(unsigned char flag)
{
  if (flag) {
    P1OUT |= LED_RED;
  } else {
    P1OUT &= ~LED_RED;
  }
}
