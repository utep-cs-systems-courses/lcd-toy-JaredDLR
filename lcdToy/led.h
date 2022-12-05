#ifndef led_included
#define led_included

#define LED_GREEN BIT0
#define LED_RED BIT6
#define LEDS (BIT0 | BIT6)

void ledInit();
void toggleGreen(unsigned char flag);
void toggleRed(unsigned char flag);

#endif
