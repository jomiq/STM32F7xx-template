/**
 * @file usb.c
 * @author jomik (jon.mihkkal.inga@gmail.com)
 * @brief Basic USB recieve/transmit 
 * @version 0.1
 * @date 2020-09-17
 * 
 * (c) 2020
 * 
 */

#include "ch.h"
#include "hal.h"

#include "usb.h"


THD_WORKING_AREA(waUSBThread, 256);
THD_FUNCTION(USB_Thread, arg){
    // Nothing here yet.
}

