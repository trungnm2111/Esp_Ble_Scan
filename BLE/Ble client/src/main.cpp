// #include "ble_client.h"

// void setup()
// {
//   Serial.begin(115200);
//   Mqtt_Init();
//   Serial.print("Setting up BLE client...");
//   start_bluetooth_client();
//   Serial.print("BLE client started...");
// }

// void loop()
// {
//   do_client_tasks();
//   publish_rssi();
// }




























// /*
//     Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleServer.cpp
//     Ported to Arduino ESP32 by Evandro Copercini
//     updates by chegewara
// */

// #include <BLEDevice.h>
// #include <BLEUtils.h>
// #include <BLEServer.h>

// int scanTime = 5; //In seconds
// BLEScan* pBLEScan;

// class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
//     void onResult(BLEAdvertisedDevice advertisedDevice) {
//       Serial.printf("Advertised Device: %s \n", advertisedDevice.toString().c_str());
//     }
// };

// void setup() {
//   Serial.begin(115200);
//   Serial.println("Scanning...");
//   BLEDevice::init("ESP32");
//   pBLEScan = BLEDevice::getScan(); //create new scan
//   pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
//   pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
//   pBLEScan->setInterval(1349);       //100ms /scan
//   pBLEScan->setWindow(449);  // less or equal setInterval value
// }

// void loop() {
//   // put your main code here, to run repeatedly:
//   BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
//   Serial.print("Devices found: ");
//   Serial.println(foundDevices.getCount());
//   Serial.println("Scan done!");
//   pBLEScan->clearResults();   // delete results fromBLEScan buffer to release memory
//   delay(2000);
// }

/*
  Complete Getting Started Guide: https://RandomNerdTutorials.com/esp32-bluetooth-low-energy-ble-arduino-ide/
  Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleScan.cpp
  Ported to Arduino ESP32 by Evandro Copercini
*/
#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

int scanTime = 5; // in seconds
BLEScan* pBLEScan;

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    Serial.printf("Advertised Device: %s \n", advertisedDevice.toString().c_str());
  }
};

void setup() {
  Serial.begin(115200);
  Serial.println("Scanning...");

  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan(); //create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99);  // less or equal setInterval value
}

void loop() {
  // put your main code here, to run repeatedly:
  BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
  Serial.print("Devices found: ");
  Serial.println(foundDevices.getCount());
  Serial.println("Scan done!");
  pBLEScan->clearResults();   // delete results fromBLEScan buffer to release memory
  delay(2000);
}