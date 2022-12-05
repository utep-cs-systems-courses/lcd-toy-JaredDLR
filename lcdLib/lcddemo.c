/** \file lcddemo.c
 *  \brief A simple demo that draws a string and square
 */

#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"

int main()
{
  configureClocks();
  lcd_init();
  clearScreen(COLOR_BLUE);

  u_char fcol = screenWidth / 2, frow = screenHeight / 2;
}
