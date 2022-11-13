#include "main.hpp"


int scanTime = 5; //In seconds
BLEScan* pBLEScan;


/**
 * @brief 
 * 
 */
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks 
{
    void onResult(BLEAdvertisedDevice advertisedDevice) 
    {
      // String deviceAddress =advertisedDevice.getAddress().c_str();
      // String deviceManufacturerData = advertisedDevice.getManufacturerData();
      // String deviceTXPower = advertisedDevice.getTXPower().c_str();

      String deviceName = advertisedDevice.getName().c_str();
      String deviceManufacturerData = advertisedDevice.getManufacturerData().c_str();

      if(deviceName.indexOf("BLE-LOCKER") > -1)
      {
          // Serial.printf("Advertised Device: %s \n", advertisedDevice.toString().c_str());
          Serial.print(deviceName);
          Serial.print("  -  ");
          Serial.println(deviceManufacturerData);
      }
    }
};

/**
 * @brief 
 * 
 */
void setup() 
{
  Serial.begin(115200);
  Serial.println("Scanning...");

  BLEDevice::init("BLE-BUD");
  pBLEScan = BLEDevice::getScan(); //create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99);  // less or equal setInterval value
}

/**
 * @brief 
 * 
 */
void loop() 
{
  // put your main code here, to run repeatedly:
  BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
  // Serial.print("Devices found: ");
  // Serial.println(foundDevices.getCount());
  // Serial.println("Scan done!");
  pBLEScan->clearResults();   // delete results fromBLEScan buffer to release memory
  
  delay(20);

  // Если данные изменились - меняем состояние светодиода (дверь открыта или закрыта)
}