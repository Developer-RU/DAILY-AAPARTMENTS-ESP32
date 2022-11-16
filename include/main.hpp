#ifndef MAIN_H_
#define MAIN_H_


#include "Arduino.h"
#include "app_config.hpp"

#include "BLEDevice.h"
#include "BLEUtils.h"

#include "task_buttons.hpp"


/**
 * @brief 
 * 
 * Connect to the BLE Server that has the name, Service, and Characteristics
 * 
 * @param pAddress 
 * @return true 
 * @return false 
 */
bool connectToServer(void);
  
/**
 * @brief Get the cpuID object
 * 
 */
void get_cpuID(void);

/**
 * @brief 
 * 
 */
void gpio_init(void);

/**
 * @brief 
 * 
 */
void ble_init(void);

/**
 * @brief 
 * 
 */
void ble_deinit(void);

/**
 * @brief 
 * 
 */
void setup(void);

/**
 * @brief Get the All Heap object
 * 
 * @return String 
 */
String getAllHeap(void);

/**
 * @brief 
 * 
 */
void loop(void);


#endif