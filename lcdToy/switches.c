#include <msp430.h>
#include "switches.h"
#include "main.h"

static char switchUpdateInterruptSense()
{
  P2IES |= (P2IN & SWITCHES);
  P2IES &= (P2IN | ~SWITCHES);
  return P2IN;
}

void switchInit()
{
  P2REN |= SWITCHES;
  P2IE |= SWITCHES;
  P2OUT |= SWITCHES;
  P2DIR &= ~SWITCHES;

  switchUpdateInterruptSense();
}

void __interrupt_vec(PORT2_VECTOR) Port_2() {
  if (P2IFG & SWITCHES) {
    P2IFG &= ~SWITCHES;
    switches = ~switchUpdateInterruptSense() & SWITCHES;
  }
}
