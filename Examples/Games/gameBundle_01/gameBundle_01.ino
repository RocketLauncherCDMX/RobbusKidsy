// ------------------------------------------------ ROBBUS KIDSY ----------------------------------------------
//
// SET DE JUEGOS POR DEFECTO DE ROBBUS KIDSY GAME_BUNDLE_01
// Este es el firmware que trae tu Robbus Kidsy por defecto cuando lo recibes por primera vez.
// Autor: Rocket Launcher
// Fecha: 06 de Junio de 2020
// ------------------------------------------------------------------------------------------------------------
//
// El presente firmware fue pensado en una funcionalidad multiple que integrara todas las caracteristicas
// de tu Robbus Kidsy para hacerlo entretenido, divertido y funcional. Con el, tendras acceso a un set de
// 3 juegos diferentes por medio de los botones de usuario.
// Cada juego es independiente de los demas y se juega de forma unica.
// 
// Cuando compras tu Robbus Kidsy, no hay necesidad de programarlo, ya que este codigo se encuentra en
// su memoria, pero por si estuviste experimentando y probando los diferentes programas que hemos escrito
// para ti, no te preocupes, siempre podras volver a Kidsy a su estado de inicial.
//
// Al encender a Kidsy, podras accecer a los juegos presionando cualquiera de los 3 botones de usuario
// A B o C, para acceder a los juegos. Las caracteristicas de cada juego son:
//
// ---------------------
// Juego A: Kidsy dice.
// ---------------------
// Este juego esta inspirado en el clasico "simon dice", donde debes de imitar lo que hace simon.
// En este caso, kidsy comenzara a moverse en las cuatro direcciones diferentes:
//
// - Adelante
// - Atras
// - Giro a la izquierda
// - Giro a la derecha
// 
// Lo hara de manera aleatoria y haciendo sonidos y encendiendo su Neopixel en colores diferentes.
// La primera ronda sera de 1 movimiento, que al finalizar, deberas repetir por medio de las flechas
// sensibles al tacto. Si aciertas el movimiento, kidsy pasara a la siguiente ronda, añadiendo un nuevo
// movimiento aleatorio a la lista. Debes estar muy atento, ya que entre mas direcciones sean, mas
// te costara recordarlas. Si aciertas a todas, habras ganado, y kidsy se pondra muy feliz.
// Si te equivocas en alguna, podras volver a intentarlo. 

// Para mas informacion sobre este programa, puedes entrar a: https://robbus.mx/kidsy
// donde encontraras posters con la informacion y mas.
// ------------------------------------------------------------------------------------------------------------
//
// ---------------------
// Juego B: Guia a Kidsy.
// ---------------------
// Aquí deberas decirle a Kidsy por donde debe ir, con la ayuda de las flechas tactiles, podras
// grabar un camino en su memoria. Para comenzar este juego, primero presiona el Boton B, entraras
// al juego. Acontinuacion, para grabar hasta 100 direcciones diferentes, presiona A. El LED4 se
// encendera, confirmando que te encuentras en modo grabacion. Kidsy esta listo para recordar
// las indicaciones. 
// Si seleccionar el juego tratar de reproducir las incidaciones, Kidsy te mostrara un error,
// debido a que aun no tendra un camino que seguir.
//
// Las indicaciones que le puedes dar a Kidsy son: 
//
// - Adelante
// - Atras
// - Giro a la izquierda
// - Giro a la derecha
// 
// Presiona cuantas flechas quieras grabar y al terminar, presiona de nuevo A.
// Kidsy te indicara con el Neopixel y un tono que la grabacion esta completa. Para reproducir las
// indicaciones, presiona B. Kidsy se preparara para hacerlo y comenzara el juego.
// Cada movimiento se vera reflejado en un tono y en el Neopixel.
//
// Crea un circuito de obstaculos con las cosas que tengas a la mano y haz que Kidsy lo supere.
// Si no lo consigues a la primera, no te preocupes, repitiendo la secuencia de grabacion podras
// cargar un nuevo camino en la memoria de Kidsy.


// Para mas informacion sobre este programa, puedes entrar a: https://robbus.mx/kidsy
// donde encontraras posters con la informacion y mas.
//
// ---------------------
// Juego C: Kidsy Kart.
// ---------------------
// En este divertido juego podras controlar a Kidsy como si de un go Kart se tratase. Esto por
// medio de Bluetooth y una aplicacion.
//
// No solo podras controlarlo, si no que podras interactuar con diferentes colores gracias al
// sensor de color.
//
// VERDE    - Te dara un turbo de 5 segundos.
// AZUL     - Hara que vayas lento, como si de agua se tratase.
// MAGENTA  - Te ponchara una llanta aleatoriamente.
//
// En el caso del Magenta, te dejara con una llanta ponchada, lo que significa que la llanta
// afectada ira mucho mas lento, lo que hara que Kidsy se incline hacia un lado, para solucionarlo
// deberas tocar las flecha del lado correspondiente por al menos 2 segundos. Kidsy te mostrara
// cuando haya sido arreglada. Si no lo haces, no podras acceder al color verde y Kidsy se quedara
// asi el resto de la carrera.  
//
// Para mas informacion sobre este programa, puedes entrar a: https://robbus.mx/kidsy
// donde encontraras posters con la informacion y mas.
// ------------------------------------------------------------------------------------------------------------

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <RobbusKidsy.h>

RobbusKidsy Kidsy;              // Llama a Robbus Kidsy

// Estados de Kidsy
#define FLAT        0           // se usan en las llantas
#define OK          1
#define IDDLE       0
#define GAME_A      1
#define GAME_B      2
#define GAME_C      3
#define GAMEOVER    4

// Acciones de Kidsy en los 3 juegos
#define READY           5
#define INTERACT_SOUND  6
#define INTERACT_NEOP   7
#define INTERACT_BOTH   8
#define INCORRECT       9
#define CORRECT         10
#define VICTORY         11

int kidsyStatus = IDDLE;        // maneja en que estado del juego se encuentra
int arrowTouched = 0;           // guarda la flecha que se toco

// Constantes y variables del juego A
// -------------------------------------------------------------------------------------
#define MAX_MOVES   10          // numero maximo de movimientos
#define SPEED_A     150         // velocidad de los movimientos de kidsy
#define DURATION_A  200         // tiempo que mantendra cada movimiento

bool flag_waitTouch = false;    // bandera para esperar el toque de una flecha
bool timeOut = false;           // bandera de limite de tiempo
int moves[MAX_MOVES];           // guarda hasta MAX_MOVE movimientos diferentes
int nextMove = 0;               // lista de movimientos
int movesNumber = 0;            // numero de movimientos por ronda
double timeStamp;               // guarda una marca de tiempo
// -------------------------------------------------------------------------------------

// Constantes y variables del juego B
// -------------------------------------------------------------------------------------
#define MAX_ARROWS  100         // limite de memoria para guardar direcciones
#define SPEED_B     150         // velocidad de los movimientos de kidsy
#define DURATION_B  170         // tiempo que mantendra cada movimiento
bool flag_record = false;       // bandera para grabar las flechas en la memoria
bool flag_configureB = false;   // bandera para configurar el juego B
bool flag_memOk = false;        // bandera de contenido en memoria, comienza vacia
int arrowBuffer[MAX_ARROWS];    // memoria para almacenar hasta 100 direcciones
int arrowCounter = 0;           // contador de direcciones guardadas
// -------------------------------------------------------------------------------------

// Constantes y variables del juego C
// -------------------------------------------------------------------------------------
#define FLAT_SPEED       100        // velocidad de una llanta ponchada (magenta)
#define NOMINAL_SPEED    150        // velocidad nominal (negro o rojo)
#define BOOST_SPEED      255        // velocidad con boost (amarillo)
#define WATER_SPEED      75         // velocidad en el agua (azul)
#define BOOST_TIME       5000       // tiempo que dura el turbo en milisegundos

bool flag_configureC = false;
bool flag_error = false;            // bandera para llamar las accion de incorrecto
bool flag_interact = false;         // bandera para llamar action de interaccion
bool flag_boost = false;            // bandera para el turbo
bool flag_left_forward = false;     // boton izquierdo adelante suelto
bool flag_left_backward = false;    // boton izquierdo atras suelto
bool flag_right_forward = false;    // boton derecho adelante suelto
bool flag_right_backward = false;   // boton derecho atras suelto

int speed_C_left = NOMINAL_SPEED;   // velocidad llanta izquierda
int speed_C_right = NOMINAL_SPEED;  // velocidad llanta derecha
int random_tireFlat = NONE;         // cuando se pasa por el morado, una llanta se poncha aleatoriamente
double boost_timer;                 // contador del tiempo en turbo
// -------------------------------------------------------------------------------------

// Constantes y variables clases del bluetooth (juego C)
// -------------------------------------------------------------------------------------
#define SERVICE_UUID           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E" // UART service UUID
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

BLEServer *pServer = NULL;
BLECharacteristic * pTxCharacteristic;
bool deviceConnected = false;
bool oldDeviceConnected = false;
uint8_t txValue = 0;

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};

void actions(int);

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

      if (rxValue == "!B516") flag_left_forward = true;         // flecha adelante presionada
      if (rxValue == "!B507") {                                 // flecha adelante suelta
        flag_left_forward = false;        
        Kidsy.Move.MotorLeft(0);
      }
      if (rxValue == "!B615") flag_left_backward = true;        // flecha atras presionada
      if (rxValue == "!B606") {                                 // flecha atras suelta
        flag_left_backward = false; 
        Kidsy.Move.MotorLeft(0);     
      }
      if (rxValue == "!B714")                                   // flecha izquierda presionada
      if (rxValue == "!B705")                                   // flecha izquierda suelta
      if (rxValue == "!B813")                                   // flecha derecha presionada
      if (rxValue == "!B804")                                   // flecha derecha suelta

      if (rxValue == "!B11:") {                                 // boton 1 presionado
        
      }
      if (rxValue == "!B10:");                                  // boton 1 suelto
      if (rxValue == "!B219") flag_right_forward = true;        // boton 2 presionado
      if (rxValue == "!B20:") {                                 // boton 2 suelto
        flag_right_forward = false;
        Kidsy.Move.MotorRight(0);
      }
      if (rxValue == "!B318") {                                 // boton 3 presionado
        
      }
      if (rxValue == "!B309");                                  // boton 3 suelto
      if (rxValue == "!B417") flag_right_backward = true;       // boton 4 presionado
      if (rxValue == "!B408") {                                 // boton 4 presionado
        flag_right_backward = false;      
        Kidsy.Move.MotorRight(0);
      }
    }
  }
};
// -------------------------------------------------------------------------------------

void setup() {
  Serial.begin(115200);         // Inicializa comunicacion serial a 115200 baudios
  Kidsy.begin();                // Inicializa el hardware del Robbus Kidsy

  // Si conectas tu Kidsy por USB y abres su terminal serial, podras ver que esta haciendo
  Serial.println("Hola, mi nombre es Robbus Kidsy, y tengo 3 juegos diferentes para ti:");
  Serial.println("BOTON A - Kidsy dice");
  Serial.println("BOTON B - Guia a Kidsy");
  Serial.println("BOTON C - Kidsy Kart");
  Serial.println("Para comenzar a jugar, presiona el boton del juego que quieras.");
}

/* --------------------------------------------------------------------------------------------
 *  void manageLeds(bool l1, bool l2, bool l3, bool l4)
 * --------------------------------------------------------------------------------------------
 *  Funcion para manejo global de los LEDs de usuario.
 *  Por medio de esta funcion, podras controlar los 4 LEDs de forma conjunta.
 *  Por ejemplo, si quieres encender el LED1 y apagar el resto, puedes hacer lo siguiente:
 *  
 *  manageLeds(1,0,0,0);
 *  
 *  Como veras cada numero de la funcion representa un LED en orden ascendente, lo estados
 *  pueden ser:
 *  
 *  1 - enciende el LED
 *  0 - apaga el LED
 * -------------------------------------------------------------------------------------------- */
void manageLeds(bool l1, bool l2, bool l3, bool l4) {
  Kidsy.Led1.set(l1);
  Kidsy.Led2.set(l2);
  Kidsy.Led3.set(l3);
  Kidsy.Led4.set(l4);
}

/* --------------------------------------------------------------------------------------------
 *  void actions(int action)
 * --------------------------------------------------------------------------------------------
/*  Funcion para manejo de las acciones de Kidsy.
 *  Esta funcion se encarga de controlar acciones concretar de tu Kidsy, las cuales involucran
 *  diferente hardware, como el Buzzer, el Neopixel. Cabe destacar que los movimientos tienen
 *  su propia funcion.
 *  
 *  Por ejemplo, desde aqui puedes expresar cuando algo que hiciste fue correcto, por medio
 *  de la siguiente forma:
 *  
 *  actions(CORRECT);
 *  
 *  Las palabras que lleva la funcion pueden ser:
 *  
 *  READY
 *  INTERACT_SOUND
 *  INTERACT_NEOP
 *  INTERACT_BOTH
 *  INCORRECT
 *  CORRECT
 *  VICTORY
 *  
 *  Y cada accion genera diferentes sonidos, patrones de luz y movimiento.
 * -------------------------------------------------------------------------------------------- */
void actions(int action) {
  switch(action) {                        // Revisa que accion se quiere realizar
    case FORWARD:                         // accion: ADELANTE
      Serial.println("  ADELANTE");
      Kidsy.Buzzer.playTone(1000, 50);    // Tono representativo: 1000Hz
      Kidsy.Neopixel.color(WHITE);        // Color representativo: BLANCO
      delay(50);
      Kidsy.Neopixel.off();               // apaga Neopixel
      break;
    case BACKWARD:
      Serial.println("  ATRAS");
      Kidsy.Buzzer.playTone(1100, 50);    // Tono representativo: 1100Hz
      Kidsy.Neopixel.color(MAGENTA);      // Color representativo: MAGENTA
      delay(50);
      Kidsy.Neopixel.off();               // apaga Neopixel
      break;
    case LEFT:
      Serial.println("  IZQUIERDA");
      Kidsy.Buzzer.playTone(1200, 50);    // Tono representativo: 1200Hz
      Kidsy.Neopixel.color(CYAN);         // Color representativo: CYAN
      delay(50);
      Kidsy.Neopixel.off();               // apaga Neopixel
      break;
    case RIGHT:
      Serial.println("  DERECHA");
      Kidsy.Buzzer.playTone(1300, 50);    // Tono representativo: 1300Hz
      Kidsy.Neopixel.color(YELLOW);       // Color representativo: AMARILLO
      delay(50);
      Kidsy.Neopixel.off();               // apaga Neopixel
      break;
    case READY:                           // indica que kidsy esta listo para comenzar
      Serial.print(" 3 ...");
      Kidsy.Buzzer.playTone(1000, 250);   // tonos sucesivos con colores de Neopixel 
      delay(500);
      Serial.print(" 2 ...");
      Kidsy.Neopixel.color(RED);
      Kidsy.Buzzer.playTone(1000, 250);
      delay(500);
      Serial.print(" 1 ...");
      Kidsy.Neopixel.color(YELLOW);
      Kidsy.Buzzer.playTone(1000, 250);
      delay(500);
      Serial.println(" ¡Comienza!");
      Kidsy.Neopixel.color(GREEN);
      Kidsy.Buzzer.playTone(2000, 250);
      delay(1000);
      Kidsy.Neopixel.color(OFF);
      break;
    case INTERACT_SOUND:                  // indica una interaccion de usuario con sonido
      Kidsy.Buzzer.playTone(1000, 50);
      break;
    case INTERACT_NEOP:                   // indica una interaccion de usuario con neopixel
      Kidsy.Neopixel.color(GREEN);
      delay(25);
      Kidsy.Neopixel.off();
      break;
    case INTERACT_BOTH:                   // indica una interaccion de usuario con sonido y neopixel
      Kidsy.Buzzer.playTone(1000);
      Kidsy.Neopixel.color(GREEN);
      delay(25);
      Kidsy.Buzzer.noTone();
      Kidsy.Neopixel.off();
      break;
    case CORRECT:                         // indica que lo que se hizo fue correcto
      Kidsy.Buzzer.playTone(2000, 25);    // tono de accion correcta
      delay(100);
      Kidsy.Buzzer.playTone(2000, 25);
      delay(100);
      Kidsy.Neopixel.heartBeat(GREEN);    // color de Neopixel: VERDE
      break;
    case INCORRECT:                       // indica que lo que se hizo fue incorrecto
      Kidsy.Buzzer.playTone(200);         // tono de accion incorrecta
      Kidsy.Neopixel.heartBeat(RED);      // color de neopixel: ROJO
      Kidsy.Buzzer.noTone();
      break;
    case VICTORY:                         // indica que se gano el juego en cuestion
      Kidsy.Buzzer.playTone(800, 100);    // tono compuesot de victoria
      delay(100);
      Kidsy.Buzzer.playTone(800, 100);
      delay(50);
      Kidsy.Buzzer.playTone(800, 100);
      delay(50);
      Kidsy.Buzzer.playTone(1200, 500);
      delay(500);
  
      Kidsy.Neopixel.color(GREEN);        // neopixel se mantiene en VERDE
      
      Kidsy.Move.turnLeft(150);           // Kidsy realiza movimientos de victoria
      delay(400);
      Kidsy.Move.turnRight(150);
      delay(400);
      Kidsy.Move.turnLeft(150);
      delay(400);
      Kidsy.Move.turnRight(150);
      delay(400);
      Kidsy.Move.turnLeft(150);
      delay(400);
      Kidsy.Move.turnRight(150);
      delay(400);
      Kidsy.Move.stop();
      break;
  }
}

/* --------------------------------------------------------------------------------------------
 *  void makeMove(int moveList)
 * --------------------------------------------------------------------------------------------
 *  Funcion para manejo los movimientos de Kidsy
 *  Esta funcion se encarga de controlar acciones de movimiento de Kidsy, acompañadas por
 *  colores de neopixel.  
 *  
 *  Por ejemplo, kidsy puede girar a la izquierda mientras muestra su color correspondiente,
 *  por medio de la siguiente forma:
 *    
 *  makeMove(LEFT);
 *  
 *  Con lo que, aparte del giro a la izquierda, Kidsy encendera en CYAN el Neopixel y
 *  llamara las acciones de esa direccion (funcion previa)
 * -------------------------------------------------------------------------------------------- */
void makeMove(int moveList, int speed, int duration) {
  switch(moveList) {
    case FORWARD:
      Kidsy.Neopixel.color(WHITE);
      actions(FORWARD);
      Kidsy.Move.forward(speed);
      delay(duration);
      Kidsy.Neopixel.off();
      Kidsy.Move.stop();
      break;
    case BACKWARD:
      Kidsy.Neopixel.color(MAGENTA);
      actions(BACKWARD);
      Kidsy.Move.backward(speed);
      delay(duration);
      Kidsy.Neopixel.off();
      Kidsy.Move.stop();
      break;
    case LEFT:
      Kidsy.Neopixel.color(CYAN);
      actions(LEFT);
      Kidsy.Move.turnLeft(speed);
      delay(duration);
      Kidsy.Neopixel.off();
      Kidsy.Move.stop();
      break;
    case RIGHT:
      Kidsy.Neopixel.color(YELLOW);
      actions(RIGHT);
      Kidsy.Move.turnRight(speed);
      delay(duration);
      Kidsy.Neopixel.off();
      Kidsy.Move.stop();
      break;
  }
}

/* --------------------------------------------------------------------------------------------
 *  int getArrow()
 *  -------------------------------------------------------------------------------------------
 *  Funcion para obtener la flecha que se toco de entre las cuatro.
 *  Esta funcion trata las cuatro flechas tactiles como un solo modulo, y determina que 
 *  flecha se toco, o si ninguna flecha de ha tocado.
 *  
 *  Se requiere una variable tipo int para guardar el valor que regresa, de la siguiente forma:
 *  
 *  int arrowTouched;
 *  arrowTouched = getArrow();
 *  
 *  Y si posteriormente busca que se haya presionado la flecha IZQUIERDA puedes hacer esto:
 *  
 *  if(arrowTouched == LEFT) { }
 *  
 *  Si no se toco ninguna flecha, regresara 0. Los valores de las palabras son:
 *  
 *  NONE        -   0
 *  FORWARD     -   1
 *  BACKWARD    -   2
 *  LEFT        -   3
 *  RIGHT       -   4
-------------------------------------------------------------------------------------------- */
int getArrow() {
  if(Kidsy.ArrowForward.read() == TOUCHED) return(FORWARD);
  else if(Kidsy.ArrowBackward.read() == TOUCHED) return(BACKWARD);
  else if(Kidsy.ArrowLeft.read() == TOUCHED) return(LEFT);
  else if(Kidsy.ArrowRight.read() == TOUCHED) return(RIGHT);
  else return(false);
}

/* --------------------------------------------------------------------------------------------
 *  void sendString()
 *  -------------------------------------------------------------------------------------------
 *  Funcion que manda un arreglo de caracteres por bluetooth
 *  Una vez que Kidsy está conectado a un dispositivo Bluetooth, con esta funcion se puede
 *  mandar un arreglo de caracteres por notificacion. Se utiliza para mandar informacion
 *  de los colores que va viendo.
-------------------------------------------------------------------------------------------- */
void sendString(String string) {
  for(int i=0; i<string.length(); i++) {
    txValue = (uint8_t)string.charAt(i);
    pTxCharacteristic->setValue(&txValue, 1);
    pTxCharacteristic->notify();
  }
  txValue = '\n';
  pTxCharacteristic->setValue(&txValue, 1);
  pTxCharacteristic->notify();
}

void loop() {
  // Lo primero que se hace es determinar el estado de Kidsy. A esto se le conoce como
  // "Maquina de estados" y no es otra cosa que una lista de pasos a seguir  para que
  // tu robot vaya siguiendolos
  switch(kidsyStatus) {
    
    // caso IDDLE: Kidsy no esta haciendo nada, espera un boton.
    case IDDLE:
      if(Kidsy.ButtonA.read() == PRESSED) {
        actions(INTERACT_SOUND);              // accion de interaccion con el boton
        kidsyStatus = GAME_A;                 // selecciona juego A
      }
      if(Kidsy.ButtonB.read() == PRESSED) {
        actions(INTERACT_SOUND);                    // accion de interaccion con el boton
        kidsyStatus = GAME_B;                 // selecciona juego B
      }
      if(Kidsy.ButtonC.read() == PRESSED) {
        actions(INTERACT_SOUND);                    // accion de interaccion con el boton
        kidsyStatus = GAME_C;                 // selecciona juego C
      }
      break;
      
    // caso Juego A, ingresa al juego A.
    case GAME_A:
      manageLeds(1,0,0,0);                  // LED1 ON, LED2-LED4 OFF
      Serial.println("Juguemos: KIDSY DICE");
      Serial.println("Me movere y tienes que recordar mis movimientos, luego, presionar las flechas");
      Serial.println("conforme los fui haciendo. ¡Ganaras si los recuerdas todos!");
      Serial.println();
      Serial.print("¿Estas list@?, comenzaremos en ...");

      timeOut = false;                      // resetea bandera de limite de tiempo
      randomSeed(analogRead(0));            // se usaran valores aleatorios entre 0 y 4
      for(int i=0; i<MAX_MOVES; i++) moves[i] = random(1,5);  // genera los movimientos aleatorioamente
      actions(READY);                       // reproduce tonos y luces
      movesNumber = 1;                      // cantidad de movimientos en cada ronda, comienza en 1
      nextMove = 0;                         // contador de movimientos, comienza en 0
      while(kidsyStatus != GAMEOVER) {      // Se queda aqui hasta que se finalice el juego
        Serial.println();
        Serial.print("Ronda ");
        Serial.print(movesNumber);
        Serial.println(": ");
        for(int i=0; i<movesNumber; i++) {  // ciclo de movimientos por ronda, si aciertas, aumenta en 1
          makeMove(moves[i], SPEED_A, DURATION_A);      // llama la funcion para moverse, con el valor del siguiente mov.
          delay(250);
        }
        Serial.println();
        Serial.println("Tu turno...");
        for(int i=0; i<movesNumber; i++) {
          timeStamp = millis();             // guarda una marca de tiempo
          timeOut = false;                  // cada que espera un toque flecha, reinicia la bandera
          do {                              // Al menos 1 vez lee si hay toque de una flecha...
            arrowTouched = getArrow();      // lee las 4 flechas
            if(millis() > timeStamp+5000) { // si pasa el limite de tiempo sin tocas flechas (5 seg)
              timeOut = true;               // se activa bandera limite de tiempo
              Serial.println("¡Tiempo!, debes ser mas rapido.");
            } 
          }
          while(arrowTouched == NONE && timeOut == false);     // ... mientras no se toque ninguna
          if(arrowTouched == moves[i]) {                  // examina si la flecha es la del movimiento ...
            switch(arrowTouched) {
              case FORWARD:   actions(FORWARD);   break;  // determina las acciones de ese toque
              case BACKWARD:  actions(BACKWARD);  break;
              case LEFT:      actions(LEFT);      break;
              case RIGHT:     actions(RIGHT);     break;
            }
          }
          else {    // ... si no corresponde al movimiento de Kidsy
            if(timeOut == false) Serial.println("¡Incorrecto!");
            actions(INCORRECT);
            Serial.println("Intentalo de nuevo presionando el BOTON A o cambia de juego");
            Serial.println();
            kidsyStatus = GAMEOVER;     // pasa al estado Fin del juego
            i=movesNumber;              // obliga al contador de movimientos a terminar
          }
        }
        if(kidsyStatus == GAME_A) {     // Si continua en modo de Juego A, quiere decir que fue correcto
          delay(500);
          movesNumber++;                // incrementa en 1 el numero de movimientos a repetir
          actions(CORRECT);             // reproduce accion correcta
          if(movesNumber == 11) {       // Si se llega al fin de los 10 movimientos
            actions(VICTORY);           // reproduce la accion VICTORIA
            Serial.println("Si quieres intentarlo de nuevo, presiona el BOTON A.");
            Serial.println();
            kidsyStatus = GAMEOVER;     // Termina el juego.
          }
        }
      }
      kidsyStatus = IDDLE;    // Pasa al estado de inactivadad para estar listo para otro juego.
      break;
      
    // caso Juego B, ingresa al juego B.
    case GAME_B:
      if(flag_configureB == false) {          // bandera para acceder a alaconfiguracion del juego B
        flag_configureB = true;               // solo accede una vez al entrar a este juego
        manageLeds(0,1,0,0);                  // LED2 encendido, LED1, LED3, LED4 apagados
        Serial.println("Juguemos: GUIA A KIDSY");
        Serial.println("Dejare que tu me guies por donde tengo que ir.");
        Serial.println("Para hacerlo, deberas grabar una lista de instrucciones presionando las flechas.");
        Serial.println("Si estas listo, presiona A para comenzar a indicarme direcciones");
      }
      
      if(Kidsy.ButtonA.read() == PRESSED) {   // evalua el boton A
        actions(INTERACT_SOUND);                    // reproduce accion interacion
        if(flag_record == false) {            // si al presionar el boton la bandera esta inactiva
          Serial.println("Modo grabacion - Presiona las flechas que quieras guardar");
          arrowCounter = 0;                   // reinicia contador de direcciones
          flag_record = true;                 // se se presiona boton A cambia el estado de bandera
          Kidsy.Led4.set(flag_record);        // refleja el estado grabacion en LED4
        }
      }

      while(flag_record == true) {                    // Mientras el modo grabacion esta activo
        if(Kidsy.ButtonA.read() == PRESSED) {         // Si se presiona de nuevo boton A
          if(arrowCounter > 0) {
            flag_memOk = true;                        // activa bandera de contenido en memoria
            flag_record = false;                      // desactiva bandera de grabacion
            Kidsy.Led4.set(flag_record);              // refleja el estado grabacion en LED4
            Serial.println("Fin de grabacion - Presiona boton C para reproducir");
            actions(CORRECT);                         // reproduce accion correcta
          }
          else {
            Serial.println("No puedes salir del modo de grabacion sin haber guardado ninguna flecha");
            actions(INCORRECT);                       // accion incorrecta
          }
        }
        
        arrowTouched = getArrow();                 // lee las 4 flechas ...
        if(arrowTouched != NONE) {
          if(arrowCounter < MAX_ARROWS) {               // si aun hay memoria disponible
            arrowBuffer[arrowCounter] = arrowTouched;   // guarda la direccion tocada
            Serial.print(arrowCounter);
            Serial.print(": ");
            actions(arrowTouched);                       // ejecuta la accion de la flecha correspondiente
            arrowCounter++;                             // aumenta contador
          }
          else {                                        // si la memoria llego a su limite
            actions(CORRECT);
            Serial.println("Memoria llena, presiona C para reproducir");
            flag_record = false;                        // desactiva bandera de modo grabacion
            flag_memOk = true;                          // activa bandera de contenido en memoria
          }
        }
      }

      if(Kidsy.ButtonB.read() == PRESSED) {
        if(flag_memOk == false) {
          actions(INCORRECT);
          Serial.println("Memoria vacia, primero graba algunas direcciones. Presiona A para grabar.");
        }
        else {
          actions(READY);
          Serial.println("Reproduciendo direcciones...");
          for(int i=0; i<arrowCounter; i++) {
            Serial.print("Paso ");
            Serial.print(i);
            Serial.print(": ");
            makeMove(arrowBuffer[i], SPEED_B, DURATION_B);
            delay(200);
          }
          actions(CORRECT);
          Serial.println("Fin de la secuencia. Presiona B para repetir, o A para grabar una nueva");
        }
      }
      
      break;
    case GAME_C:
      if(flag_configureC == false) {          // bandera para acceder a alaconfiguracion del juego B
        flag_configureC = true;             // solo accede una vez al entrar a este juego
        manageLeds(0,0,1,0);
        Serial.println("Juguemos: KIDSY KART");
        Serial.println("En este juego yo me transformare en un go kart y tu podras controlarme.");
        Serial.println("Recuerda que tengo Bluetooth, por que podras usar tu celular para hacerlo");
        Serial.println("aunque por el momento, solo soy compatible con celulares Android :)");
        Serial.println("Lo mejor de todo es que podras usar habilidades conmigo, y estar al");
        Serial.println("pendiente de cuidarte de ciertas amenazas, por medio de colores");
        Serial.println();

        // Crea el dispositivo bluetooth
        BLEDevice::init("Robbus Kidsy");

        // Crea el servidor bluetooth
        pServer = BLEDevice::createServer();
        pServer->setCallbacks(new MyServerCallbacks());

        // Crea el servivio bluetooth
        BLEService *pService = pServer->createService(SERVICE_UUID);

        // Crea la caracteristica bluetooth
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

        // Comienza el servicio
        pService->start();

        Serial.print("Bluetooth encendido, buscando conexion ");

        // Comienza saludo
        pServer->getAdvertising()->start();
      }
      // disconnecting
      if (!deviceConnected && oldDeviceConnected) {
          delay(500);                               // da tiempo al bluetooth de estar listo
          pServer->startAdvertising();              // reinicia saludo
          oldDeviceConnected = deviceConnected;
          Kidsy.Neopixel.heartBeat(RED);
          Kidsy.ColorSensor.enable();               // Deshabilita el sensor de color y el LED blanco
          Serial.println();
          Serial.println("Desconectado");
          Serial.println("Sensor de color inhabilitado");
      }
      // connecting
      if (deviceConnected && !oldDeviceConnected) {
        oldDeviceConnected = deviceConnected;       // actualiza el estado de la conexion a conectado
        Kidsy.Led4.on();
        Kidsy.Neopixel.heartBeat(GREEN);
        Kidsy.ColorSensor.enable();   // Habilita el sensor de color y el LED blanco
        Serial.println();
        Serial.println("Conectado correctamente");
        Serial.println("Sensor de color habilitado");
      }

      if(deviceConnected == false) {        // Kidsy no esta conectado por bluetooth
        Kidsy.Led4.off();                   // Apaga LED4
        Serial.print(".");
        Kidsy.Neopixel.heartBeat(YELLOW);   
        delay(3000);                        // cada 3 segundos checa la conexion
      }

      // En esta seccion se lee el sensor de color
      Kidsy.ColorSensor.read();
      Kidsy.Neopixel.color(Kidsy.ColorSensor.value);
      switch(Kidsy.ColorSensor.value) {
        case RED:
            // El negor se confunde facilmente con el rojo, por esto figura como parte del camino
            if(flag_boost == false) Kidsy.Buzzer.noTone();  
            if(random_tireFlat == 0 && flag_boost == false) speed_C_left = speed_C_right = NOMINAL_SPEED;
          break;
        // El verde es el color del turbo. Duracion: 5000 milisegundos
        case GREEN:
          if(random_tireFlat == 0 && flag_boost == false) {   // si no esta ya en turbo o ponchado
            Serial.println("Boost speed");
            sendString("Zona Turbo");
            flag_boost = true;                                // activa el turbo 
            boost_timer = millis();                           // guarda el tiempo inicial
            Kidsy.Buzzer.playTone(500);                       // reproduce tono
          }
          break;
        case BLUE:
          // Color del agua
          if(random_tireFlat == 0 && flag_boost == false) {   // Si no esta ponchado o en turbo
            speed_C_left = WATER_SPEED;                       // carga la velocidad del agua
            speed_C_right = WATER_SPEED;
            sendString("Zona de agua");
            Serial.println("Water speed");
            Kidsy.Buzzer.playTone(2000);                      // reproduce tono
          }
          break;
        case MAGENTA:
          // solo entra si no hay llanta ponchada y no hay turbo activo
          if(random_tireFlat == NONE && flag_boost == false) {
            actions(INCORRECT);               
            randomSeed(millis());                     // determina cual llanta se ponchara aleatoriamente
            random_tireFlat = random(LEFT,RIGHT+1);   // menor inclusivo, mayor exclusivo
            Serial.print("Llanta ponchada: ");
            Serial.println(random_tireFlat);
            if(random_tireFlat == LEFT) {
              speed_C_left = FLAT_SPEED;
              sendString("La llanta izquierda se poncho");
              sendString("Para arreglar, toca por 2 segundos la flecha izquierda de Kidsy");
              Serial.println("Llanta izquierda");
            }
            else if(random_tireFlat == RIGHT) {
              speed_C_right = FLAT_SPEED;
              sendString("La llanta derecha se poncho");
              sendString("Para arreglar, toca por 2 segundos la flecha derecha de Kidsy");
              Serial.println("Llanta derecha");
            }
          }
          break;
        case YELLOW:
          
          break;
        case CYAN:
          // Color del agua
          if(random_tireFlat == 0 && flag_boost == false) {   // Si no esta ponchado o en turbo
            speed_C_left = WATER_SPEED;                       // carga la velocidad del agua
            speed_C_right = WATER_SPEED;
            sendString("Zona de agua");
            Serial.println("Water speed");
            Kidsy.Buzzer.playTone(2000);                      // reproduce tono
          }
          break;
        case WHITE:
          break;
        case BLACK:
          // El negro representa el asfalto del camino y la velocidad es la nominal
          if(flag_boost == false) Kidsy.Buzzer.noTone();    // si no esta en turbo, silencia Buzzer        
          if(random_tireFlat == 0 && flag_boost == false) { // si no esta ponchado o en turbo
            speed_C_left = speed_C_right = NOMINAL_SPEED;   // restaura velocidad nominal
          }
          break;
      }

      if(flag_boost == true) {                        // Si se activa el turbo (color verde)
        speed_C_left = BOOST_SPEED;                   // carga velocidad maxima
        speed_C_right = BOOST_SPEED;
        // si paso el tiempo del turbo, lo desactiva
        if(millis() > boost_timer + BOOST_TIME) {     // si pasaron 5 segundos
          flag_boost = false;                         // desactiva el turbo
          Kidsy.Buzzer.noTone();                      // silencia el buzzer
          sendString("Turbo agotado");
          Serial.println("Turbo agotado");
        }
      }

      if(random_tireFlat != NONE) {             // Si se encuentra una llanta ponchada
        arrowTouched = getArrow();              // lee las flechas
        Serial.println(arrowTouched);
        if(arrowTouched == random_tireFlat) {   // si la flecha es la misma que la llanta
          if(random_tireFlat == LEFT) sendString("Llanta izquierda arreglada");
          else if(random_tireFlat == RIGHT) sendString("Llanta derecha arreglada");
          random_tireFlat = 0;                  // arregla llanta
          speed_C_left = NOMINAL_SPEED;         // restaura la velocidad en ambas llantas
          speed_C_right = NOMINAL_SPEED;
          actions(CORRECT);                     // reproduce accion correcta
        }
      }

      // reproduce errores
      if(flag_error == true) {
        flag_error = false;
        actions(INCORRECT);
      }

      // reproduce interacciones
      if(flag_interact == true) {
        flag_interact = false;
        actions(INTERACT_BOTH);
      }

      // En esta seccion se cargan constantemente los valores de velocidad en las llantas
      // dependiendo de los botones que se estan presionando
      if(flag_left_forward == true) Kidsy.Move.MotorLeft(speed_C_left);
      if(flag_left_backward == true) Kidsy.Move.MotorLeft(-speed_C_left);
      if(flag_right_forward == true) Kidsy.Move.MotorRight(speed_C_right);
      if(flag_right_backward == true) Kidsy.Move.MotorRight(-speed_C_right);
      break;
  }
}