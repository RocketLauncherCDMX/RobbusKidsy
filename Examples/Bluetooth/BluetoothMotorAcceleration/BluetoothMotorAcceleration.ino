/*
    Video: https://www.youtube.com/watch?v=oCMOYS71NIU
    Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleNotify.cpp
    Ported to Arduino ESP32 by Evandro Copercini

   Create a BLE server that, once we receive a connection, will send periodic notifications.
   The service advertises itself as: 6E400001-B5A3-F393-E0A9-E50E24DCCA9E
   Has a characteristic of: 6E400002-B5A3-F393-E0A9-E50E24DCCA9E - used for receiving data with "WRITE" 
   Has a characteristic of: 6E400003-B5A3-F393-E0A9-E50E24DCCA9E - used to send data with  "NOTIFY"

   The design of creating the BLE server is:
   1. Create a BLE Server
   2. Create a BLE Service
   3. Create a BLE Characteristic on the Service
   4. Create a BLE Descriptor on the characteristic
   5. Start the service.
   6. Start advertising.

   In this example rxValue is the data received (only accessible inside that function).
   And txValue is the data to be sent, in this example just a byte incremented every second. 
*/
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <Rob_bit_Kidsy.h>

Rob_bit Kidsy;

#define MAX_SPEED     100     // Velocidad maxima de los motores
#define TIMER_TIME    50000   // Tiempo del Timer0 en microSegundos

// Declaramos un puntero tipo timer, compatible con ESP32
hw_timer_t * timer = NULL;
// Variable de sincronizacion para modificar una variable compartida
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

// Variables para la velocidad de los motores
// Los valores pueden ser:
// POSITIVO   1 a  255  ->   1 es la velocidad minima,  255 la maxima, hacia ADELANTE.
// NEGATIVO  -1 a -255  ->  -1 es la velocidad minima, -255 la maxima, hacia ATRAS.
//            0 es totalmente detenido.
// Una velocidad muy baja (menor a +-25) puede que no sea suficiente para comenzar a mover el motor

int motorSpeedLeft = 0;      // 100 hacia el FRENTE
int motorSpeedRight = 0;    // 100 hacia ATRAS
int setSpeedLeft = 0;
int setSpeedRight = 0;

// Funcion de interrupcion del timer
void IRAM_ATTR onTimer() {
  // Para la variable compartida por loop y la funcion de interrupcion
  portENTER_CRITICAL_ISR(&timerMux);  

  // En esta seccion se evalua la velocidad actual de los motores y se ajustan cada el timer interrumpe
  if(motorSpeedLeft < setSpeedLeft)         motorSpeedLeft++;
  else if(motorSpeedLeft > setSpeedLeft)    motorSpeedLeft--;
  if(motorSpeedRight < setSpeedRight)       motorSpeedRight++;
  else if(motorSpeedRight > setSpeedRight)  motorSpeedRight--;

  // carga los nuevos valores de velocidad en los motores
  Kidsy.Move.MotorLeft(motorSpeedLeft);
  Kidsy.Move.MotorRight(motorSpeedRight);

  // Para la variable compartida por loop y la funcion de interrupcion
  portEXIT_CRITICAL_ISR(&timerMux);   
}

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
      Kidsy.Neopixel.heartBeat(RED);
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
      Kidsy.Neopixel.heartBeat(BLUE);
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

      if (rxValue == "!B516") {
        setSpeedLeft = MAX_SPEED;
        setSpeedRight = MAX_SPEED;
      }
      else if (rxValue == "!B507") {
        setSpeedLeft = 0;
        setSpeedRight = 0;
      }
      if (rxValue == "!B615") {
        setSpeedLeft = -MAX_SPEED;
        setSpeedRight = -MAX_SPEED;
      }
      else if (rxValue == "!B606") {
        setSpeedLeft = 0;
        setSpeedRight = 0;
      }
      if (rxValue == "!B714") {
        setSpeedLeft = -MAX_SPEED;
        setSpeedRight = MAX_SPEED;
      }
      else if (rxValue == "!B705") {
        setSpeedLeft = 0;
        setSpeedRight = 0;
      }
      if (rxValue == "!B813") {
        setSpeedLeft = MAX_SPEED;
        setSpeedRight = -MAX_SPEED;
      }
      else if (rxValue == "!B804") {
        setSpeedLeft = 0;
        setSpeedRight = 0;
      }

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

  // A continucacion configuramos el timer:
  
  // 0       - Inicializa el Timer0, Rob.bit Kidsy dispone de 4 timers (0 al 3)
  // 80      - Valor del preescaler para 80MHz / 80 = 1000000 veces por segundo
  // true    - Cuenta ascendente, false: cuenta descendente
  timer = timerBegin(0, 80, true);
  
  // timer    - Pasamos la variable timer creada
  // &onTimer - funcion onTimer
  // true     - la interrupcion generada sera ascendente, false: descendente
  timerAttachInterrupt(timer, &onTimer, true);  
  
  // timer   - Puntero al timer
  // 5000    - Valor del contador en el cual el timer interrumpira (ms para un valor de preescaler de 80 y velocidad de 80MHz)
  // true    - Interrupcion periodica, false: interrumpe una sola vez 
  timerAlarmWrite(timer, 5000, true);

  //  Habilitamos el timer que acabamos de crear
  timerAlarmEnable(timer);               

  // ---------------------------------------------------------------------------------------------------------------------------

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