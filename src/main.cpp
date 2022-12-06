#include "main.hpp"


static BLEUUID serviceUUID(UART_SERVICE_UUID);
static BLEUUID charUUID(UART_SERVICE_RX);

static BLERemoteService * pRemoteService;
static BLERemoteCharacteristic * pRemoteCharacteristic;

static BLEAddress * pServerAddress;
static esp_ble_addr_type_t type;

static BLEClient * pClient;
static BLEScan * pBLEScan;

static String cpuId = "";

static boolean doConnect = false, doConnectHandset = false;
static boolean connected = false;


boolean newCommand = false;
uint8_t valCommand = 0;


/**
 * @brief 
 * 
 */
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks 
{
    /**
     * @brief 
     * 
     * @param advertisedDevice 
     */
    void onResult(BLEAdvertisedDevice advertisedDevice) 
    {
        String deviceName = advertisedDevice.getName().c_str();

        // Для трубки

        if(deviceName.indexOf("BLE-HANDSET") > -1)
        { 
            advertisedDevice.getScan()->stop();
            pServerAddress = new BLEAddress(advertisedDevice.getAddress());
            type = advertisedDevice.getAddressType();

            doConnectHandset = true;
            doConnect = false;

            DEBUG.printf("Advertised Device: %s \n", advertisedDevice.toString().c_str());
            
            return;
        }



        // Для замка

        else if(deviceName.indexOf(DEVICE_NAME) > -1 && doConnectHandset == false)
        { 
            advertisedDevice.getScan()->stop();
            pServerAddress = new BLEAddress(advertisedDevice.getAddress());
            type = advertisedDevice.getAddressType();

            doConnect = true;
            doConnectHandset = false;

            DEBUG.printf("Advertised Device: %s \n", advertisedDevice.toString().c_str());

            return;

        }
        else{}

        
    }
};

/**
 * @brief 
 * 
 */
class MyClientCallback : public BLEClientCallbacks 
{
    /**
     * @brief 
     * 
     * @param pclient 
     */
    void onConnect(BLEClient* pclient) 
    {
        DEBUG.println(">>> onConnect");
    }

    /**
     * @brief 
     * 
     * @param pclient 
     */
    void onDisconnect(BLEClient* pclient) 
    {
        connected = false;
        DEBUG.println(">>> onDisconnect");
        delete (pServerAddress);
    }
};

/**
 * @brief 
 * 
 * Connect to the BLE Server that has the name, Service, and Characteristics
 * 
 * @param pAddress 
 * @return true 
 * @return false 
 */
bool connectToServer() 
{
    pClient->setClientCallbacks(new MyClientCallback());
    
    if(!pClient->connect(* pServerAddress, type))
    {
        delete (pServerAddress);
        connected = false;
        return false;
    }

    DEBUG.println("Connected to server");

    pRemoteService = pClient->getService(serviceUUID);

    if (pRemoteService == nullptr) 
    {
        pClient->disconnect();

        delete (pRemoteService);
        delete (pServerAddress);
        connected = false;

        return false;
    }

    pRemoteCharacteristic = pRemoteService->getCharacteristic(charUUID);

    if (pRemoteCharacteristic  == nullptr) 
    {
        pClient->disconnect();
    
        delete (pRemoteCharacteristic);
        delete (pRemoteService);
        delete (pServerAddress);
        connected = false;

        return false;
    }

    connected = true;

    return true;
}
  
/**
 * @brief Get the cpuID object
 * 
 */
void get_cpuID()
{
    uint64_t chipid = ESP.getEfuseMac();
    cpuId = String((uint16_t)(chipid >> 32)) + String((uint32_t)chipid);
}

/**
 * @brief 
 * 
 */
void gpio_init()
{
    pinMode(PIN_LED, OUTPUT);

    pinMode(PIN_BTN_OPEN, INPUT_PULLUP);
    pinMode(PIN_BTN_CLOSE, INPUT_PULLUP);

    pinMode(PIN_BTN_ADV_FAST, INPUT_PULLUP);
    pinMode(PIN_BTN_ADV_LOW, INPUT_PULLUP);
}

/**
 * @brief 
 * 
 */
void ble_init()
{
    BLEDevice::init(APP_NAME);
    
    pBLEScan = BLEDevice::getScan();
    pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
    pBLEScan->setActiveScan(true);
    pBLEScan->setInterval(INTERVAL_SCAN);
    pBLEScan->setWindow(99);

    pClient = BLEDevice::createClient();
}

/**
 * @brief 
 * 
 */
void ble_deinit()
{
    BLEDevice::deinit(false);
}

/**
 * @brief 
 * 
 */
void setup() 
{
    gpio_init();
    get_cpuID();

    DEBUG.begin(DEBUG_BAUNDRATE);    


    ble_init();  

    xTaskCreate(TaskButtons,   "TaskButtons",   STACK_SIZE_BUTTONS,  NULL, 1, NULL);     // +++
 
}

/**
 * @brief Get the All Heap object
 * 
 * @return String 
 */
String getAllHeap()
{
    char temp[300];

    sprintf(temp, "Heap: Free:%i, Min:%i, Size:%i, Alloc:%i", ESP.getFreeHeap(), ESP.getMinFreeHeap(), ESP.getHeapSize(), ESP.getMaxAllocHeap());
    
    return temp;
}

/**
 * @brief 
 * 
 */
void loop() 
{
    // ble_init();   

    pBLEScan->start(TIME_SCAN, true);


    /*
    
        Edit state led from data manufacture data device (no connect)
        
    */


    if(newCommand)
    {
        DEBUG.println("Device connecting!");


        if ((doConnect == true || doConnectHandset == true ) && newCommand == true)
        {
            if (connectToServer()) 
            {
                DEBUG.println("We are now connected to the BLE Server.");
            } 
            else 
            {
                DEBUG.println("We have failed to connect to the server; there is nothin more we will do.");
            }

            doConnectHandset = false;
            doConnect = false;
        }


        if (connected) 
        {            
            String command = "";

            if(valCommand == 1) { command = "open";  }
            
            if(valCommand == 2) { command = "close"; }

            if(valCommand == 3) { command = "norm";  }

            if(valCommand == 4) { command = "speed"; }
            
        
            DEBUG.println(command);
            pRemoteCharacteristic->writeValue(command.c_str(), sizeof(command));
            delay(250);

            DEBUG.println("Disconnecting");

            pClient->disconnect();
            pBLEScan->erase(pClient->getPeerAddress());
            ESP.restart();
            
            // pBLEScan->erase(*pServerAddress);
            //BLEDevice::getScan()->erase(pClient->getPeerAddress());

            valCommand = 0;
            newCommand = false;

        }

    }


    pBLEScan->clearResults();

    connected = false; 
    doConnectHandset = false;
    doConnect = false;

    // ble_deinit();
    
    //DEBUG.println(getAllHeap());

    delay(1500);
}