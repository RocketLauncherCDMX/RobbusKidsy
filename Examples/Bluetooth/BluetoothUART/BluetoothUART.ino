/*
*   Este ejemplo crea una conexion basica entre Rob.bit Kidsy y un telefono, por bluetooth 4.2.
*   El presente ejemplo funciona hasta el día 02/03/2020 con la aplicación blueFruit de Adafruit
*   para telefonos Android. https://play.google.com/store/apps/details?id=com.adafruit.bluefruit.le.connect&hl=es
*   
*   Pasos para probar este ejemplo (modo Beta):
*
*   1.- Compilar y subir con la placa Adafruit ESP32 Feather.
*   2.- Serciorarse que tu Rob.bit Kidsy tenga el interruptor en posicion ON.
*   3.- Instalar la aplicacion bluefruit (enlace arriba) en dispositivos Android (No probado en iOs).
*   4.- Abrir la aplicación. Aparecera la pantalla Select Device, escoger Rob.bit Kidsy.
*   5.- El Led1 se encendera, indicando que la conexion fue exitosa. Si no se enciende o marca error
*       volver a intentar.
*   6.- Seleccionar Controller en la siguiente pantalla.
*   7.- Dentro de Available Pins, seleccionar Control Pad.   
*/
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <Rob_bit_Kidsy.h>

Rob_bit Kidsy;

BLEServer *pServer = NULL;
BLECharacteristic * pTxCharacteristic;
bool deviceConnected = false;
bool oldDeviceConnected = false;
int txValue;

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E" // UART service UUID
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"


class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
      Kidsy.Led1.on();
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
      Kidsy.Led1.off();
    }
};

class MyCallbacks: public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) {
    std::string rxValue = pCharacteristic->getValue();

    if (rxValue.length() > 0) {
      Serial.println("*********");
      Serial.print("Received Value: ");
      for (int i = 0; i < rxValue.length(); i++)
        Serial.print(rxValue[i]);

      Serial.println();
      Serial.println("*********");

      if (rxValue == "!B516") Kidsy.Move.forward(100);
      else if (rxValue == "!B507") Kidsy.Move.stop();
      if (rxValue == "!B615") Kidsy.Move.backward(100);
      else if (rxValue == "!B606") Kidsy.Move.stop();
      if (rxValue == "!B714") Kidsy.Move.turnLeft(150);
      else if (rxValue == "!B705") Kidsy.Move.stop();
      if (rxValue == "!B813") Kidsy.Move.turnRight(150);
      else if (rxValue == "!B804") Kidsy.Move.stop();

      if (rxValue == "!B11:") {
        Kidsy.Buzzer.playTone(2000, 50);
        Kidsy.Led1.on();
      }
      else if (rxValue == "!B219") {
        Kidsy.Buzzer.playTone(2500, 50);
        Kidsy.Led1.off();
      }
      else if (rxValue == "!B318") {
        Kidsy.Buzzer.playTone(3000, 50);
        Kidsy.Led2.on();
      }
      else if (rxValue == "!B417") {
        Kidsy.Buzzer.playTone(3500, 50);
        Kidsy.Led2.off();
      }
    }
  }
};

void setup() {
  Serial.begin(115200);
  Kidsy.begin();
  Kidsy.ColorSensor.enable();

  // Create the BLE Device
  BLEDevice::init("Rob.bit Kidsy");

  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create a BLE Characteristic
  pTxCharacteristic = pService->createCharacteristic(
                    CHARACTERISTIC_UUID_TX,
                    BLECharacteristic::PROPERTY_NOTIFY
                  );
                      
  pTxCharacteristic->addDescriptor(new BLE2902());

  BLECharacteristic * pRxCharacteristic = pService->createCharacteristic(
                       CHARACTERISTIC_UUID_RX,
                      BLECharacteristic::PROPERTY_WRITE
                    );

  pRxCharacteristic->setCallbacks(new MyCallbacks());

  // Start the service
  pService->start();

  // Start advertising
  pServer->getAdvertising()->start();
  Serial.println("Waiting a client connection to notify...");
}

void loop() {
    Kidsy.ColorSensor.read();
    Kidsy.Neopixel.color(Kidsy.ColorSensor.value);
    Serial.println(Kidsy.ColorSensor.name);
    
    if (deviceConnected) {
        pTxCharacteristic->setValue(&Kidsy.ColorSensor.value, 1);
        pTxCharacteristic->notify();
    delay(10); // bluetooth stack will go into congestion, if too many packets are sent
  }

    // disconnecting
    if (!deviceConnected && oldDeviceConnected) {
        delay(500); // give the bluetooth stack the chance to get things ready
        pServer->startAdvertising(); // restart advertising
        Serial.println("start advertising");
        oldDeviceConnected = deviceConnected;
    }
    // connecting
    if (deviceConnected && !oldDeviceConnected) {
    // do stuff here on connecting
        oldDeviceConnected = deviceConnected;
    }
}