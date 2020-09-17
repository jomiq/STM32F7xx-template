/*
    ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#include "ch.h"
#include "hal.h"

#include "project.h"
#include "blink.h"
#include "usbcfg.h"
#include "usb.h"

static const uint8_t txbuf[] =
    "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
    "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
    "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
    "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
    "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
    "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
    "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
    "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
    "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
    "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
    "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
    "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
    "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
    "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
    "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
    "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef";

static uint8_t rxbuf[1024 + 1];

/*
 * USB writer. This thread writes data to the USB at maximum rate.
 * Can be measured using:
 *   dd if=/dev/xxxx of=/dev/null bs=512 count=10000
 */
static THD_WORKING_AREA(waWriter, 128);
static THD_FUNCTION(Writer, arg) {

  (void)arg;
  chRegSetThreadName("writer");
  while (true) {
    msg_t msg = usbTransmit(&USBD2, USBD2_DATA_REQUEST_EP,
                            txbuf, sizeof (txbuf));
    if (msg == MSG_RESET)
      chThdSleepMilliseconds(500);
  }
}

/*
 * USB reader. This thread reads data from the USB at maximum rate.
 * Can be measured using:
 *   dd if=bigfile of=/dev/xxx bs=512 count=10000
 */
static THD_WORKING_AREA(waReader, 128);
static THD_FUNCTION(Reader, arg) {

  (void)arg;
  chRegSetThreadName("reader");
  while (true) {
    msg_t msg = usbReceive(&USBD2, USBD2_DATA_AVAILABLE_EP,
                           rxbuf, sizeof (rxbuf));
    if (msg == MSG_RESET)
      chThdSleepMilliseconds(500);
  }
}


/* 
 * Application entry point.
 */
int main(void) {

  /*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *   and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   */
  halInit();
  chSysInit();

  /*
   * Creates the blinky thread.
   */
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO + 1, Thread1, NULL);

  /*
  * Creates the USB threads
  */
  chThdCreateStatic(waReader, sizeof(waReader), NORMALPRIO, Reader, NULL);
  chThdCreateStatic(waWriter, sizeof(waWriter), NORMALPRIO, Writer, NULL);

  /*
   * Normal main() thread activity, in this demo it does nothing except
   * sleeping in a loop and check the button state.
   */
  while (true) {
    chThdSleepMilliseconds(500);
    sdWrite(&SD3, (uint8_t*) READY_MSG, READY_MSG_N);
  }
}
