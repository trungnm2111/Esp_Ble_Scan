#include "ble_client.h"

/****************************************************************
 * Definitions and Constants
 ***************************************************************/
// static constexpr char DEVICE_NAME[]  = "Distance_Client";

/****************************************************************
 * Function Prototypes
 ***************************************************************/
/**Empty here */

/****************************************************************
 * Global Variables
 ***************************************************************/
// static boolean doConnect        = false;
// static boolean connected        = false;
static boolean doScan           = false;
static boolean toPublishRSSI    = false;
// static int     rssi_measurement = -9999;
static BLEAdvertisedDevice* myDevice;
static BLEClient*           pClient;

/****************************************************************
 * Classes
 ***************************************************************/

class MyClientCallback : public BLEClientCallbacks {
  void onConnect(BLEClient* pclient) {
      Serial.println("Client Connected");
  }

  void onDisconnect(BLEClient* pclient) {
    // connected = false;
    Serial.println("Client Disconnected");
  }
};

/**
 * Scan for BLE servers and find the first one that advertises the service we are looking for.
 */
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
 /**
   * Called for each advertising BLE server.
   */
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    // Serial.print("BLE Advertised Device found: ");
    // Serial.println(advertisedDevice.toString().c_str());
    // We have found a device, let us now see if it contains the service we are looking for.
    if (advertisedDevice.haveRSSI()) {
        myDevice = new BLEAdvertisedDevice(advertisedDevice);
        Serial.println(myDevice->getAddress().toString().c_str());
        Serial.printf("Rssi: %d", myDevice->getRSSI());
        Serial.printf("\n");
        Send_Message(myDevice->getAddress().toString().c_str(),myDevice->getRSSI());
    //   BLEDevice::getScan()->stop();
    //   myDevice = new BLEAdvertisedDevice(advertisedDevice);
    //   doConnect = true;
    //   doScan = true;
    } // Found our server
  } // onResult

}; // MyAdvertisedDeviceCallbacks

/****************************************************************
 * Function Definition
 ***************************************************************/

void start_bluetooth_client()
{
    BLEDevice::init("ESP32");
    BLEDevice::getScan()->clearResults();
    // Retrieve a Scanner and set the callback we want to use to be informed when we
    // have detected a new device.  Specify that we want active scanning and start the
    // scan to run for 5 seconds.
    BLEScan* pBLEScan = BLEDevice::getScan();
    pBLEScan->clearResults();
    pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
    pBLEScan->setInterval(1349);
    pBLEScan->setWindow(449);
    pBLEScan->setActiveScan(true);
    BLEScanResults foundDevices = pBLEScan->start(5, false);
    Serial.print("Devices found: ");
    Serial.println(foundDevices.getCount());
    pBLEScan->clearResults();
}

bool connectToServer() {
    Serial.print("Forming a connection to ");
    Serial.println(myDevice->getAddress().toString().c_str());
    
    // create client
    pClient = BLEDevice::createClient();
    Serial.println(" - Created client");

    pClient->setClientCallbacks(new MyClientCallback());

    // Connect to the remove BLE Server.
    pClient->connect(myDevice);  // if you pass BLEAdvertisedDevice instead of address, it will be recognized type of peer device address (public or private)
    Serial.println(" - Connected to server");

    // Obtain a reference to the service we are after in the remote BLE server.
    BLERemoteService* pRemoteService = pClient->getService(SERVICE_UUID);
    if (pRemoteService == nullptr) {
      Serial.print("Failed to find our service UUID: ");
      Serial.println(SERVICE_UUID);
      pClient->disconnect();
      return false;
    }
    Serial.println(" - Found our service");

    /**Do characteristic tasks here upon finding valid services*/
    // connected = true;

    return doScan;
}


void do_client_tasks()
{
    static constexpr unsigned CLIENT_TASK_PERIOD_MS = 500;
    static unsigned long last_called = millis();

    if(millis() - last_called < CLIENT_TASK_PERIOD_MS)
    {
        /**Not your turn yet */
        return;
    }

    // If the flag "doConnect" is true then we have scanned for and found the desired
    // BLE Server with which we wish to connect.  Now we connect to it.  Once we are 
    // connected we set the connected flag to be true.
    // if (doConnect == true) {
    //     if (connectToServer()) {
    //     Serial.println("We are now connected to the BLE Server.");
    //     } else {
    //     Serial.println("We have failed to connect to the server; there is nothin more we will do.");
    //     }
    //     doConnect = false;
    // }

    // If we are connected to a peer BLE Server, update the characteristic each time we are reached
    // with the current time since boot.
    // if(connected)
    // {
    //     String newValue = "Time since boot: " + String(millis()/1000);
    
    //     /**Do service tasks here */  
    //     if (myDevice->haveRSSI()){
    //         rssi_measurement = (int)pClient->getRssi();
    //     }
    // }

    if(doScan)
    {
        BLEScanResults foundDevices = BLEDevice::getScan()->start(5, false);
        Serial.print("Devices found: ");
        Serial.println(foundDevices.getCount());
        BLEDevice::getScan()->clearResults();
        // BLEDevice::getScan()->start(5, false);  // this is just example to start scan after disconnect, most likely there is better way to do it in arduino
        doScan = false;
    }

    last_called = millis();
}

void publish_rssi()
{
    constexpr unsigned RSSI_PUBLISH_PERIOD_MS = 500;
    // static String char_buffer;

    unsigned long last_publish_ms    = millis();
    
    // if(toPublishRSSI && ((millis() - last_publish_ms) >= RSSI_PUBLISH_PERIOD_MS))
    // {
    //     // Serial.printf("Rssi: %d \n", rssi_measurement);
    //     // Serial.println(myDevice->getAddress().toString().c_str());
    //     last_publish_ms = millis();
    // }
    /**Turn on/off */
    if(Serial.available())
    {
        String user_cmd = Serial.readString();
        
        user_cmd.replace("\r", "");
        user_cmd.replace("\n", "");

        // if(user_cmd == "start")
        // {
        //     toPublishRSSI = true;
        //     // doScan = true;
        // }
        if(user_cmd == "start")
        {
            doScan = true;
        }
        else if (user_cmd == "end")
        {
            toPublishRSSI = false;
        }
    }
}