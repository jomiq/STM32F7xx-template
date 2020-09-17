/**
 * @file usb.h
 * @author jomik (jon.mihkkal.inga@gmail.com)
 * @brief Implements USB input/output routines
 * @version 0.1
 * @date 2020-09-17
 * 
 * (c) 2020
 * 
 */

#ifndef __USB_H__
#define __USB_H__

extern THD_WORKING_AREA(waUSBThread, 256);
THD_FUNCTION(USBThread, arg);


#endif // !__USB_H__