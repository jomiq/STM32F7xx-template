#include "blink.h"

static THD_WORKING_AREA(waThread1, 128);
static THD_FUNCTION(Thread1, arg) {

  (void)arg;
  chRegSetThreadName("blinker");
  while (true) {
    palSetLine(LINE_LED1);
    palSetLine(LINE_LED3);
    palSetLine(LINE_LED2);
    chThdSleepMilliseconds(500);
    palClearLine(LINE_LED1);
    palClearLine(LINE_LED2);
    palClearLine(LINE_LED3);
    chThdSleepMilliseconds(200);
    
  }
}