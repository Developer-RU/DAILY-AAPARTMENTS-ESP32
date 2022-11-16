#ifndef APP_CONFIG_H_
#define APP_CONFIG_H_

#include "Arduino.h"


#define APP_NAME                    "BLE-BUD-ESP32"
#define VERSION_FIRMWARE            "1.0.0"

#define CONFIG_NAME                 "config.json"


#define DEBUG                       Serial
#define DEBUG_BAUNDRATE             115200


#define STACK_SIZE_BUTTONS            8192


#define PIN_LED                     25

#define PIN_BTN_OPEN                12
#define PIN_BTN_CLOSE               14

#define PIN_BTN_ADV_FAST            17
#define PIN_BTN_ADV_LOW             32


#define DEVICE_NAME                 "BLE-LOCKER"

#define TIME_SCAN                   4
#define INTERVAL_SCAN               100



//// UART_SERVICE ////
/**********************************************************************************************/
#define UART_SERVICE_UUID           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
// #define UART_SERVICE_TX             "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define UART_SERVICE_RX             "6E400003-B5A3-F393-E0A9-E50E24DCCA9E" 
/**********************************************************************************************/




#endif