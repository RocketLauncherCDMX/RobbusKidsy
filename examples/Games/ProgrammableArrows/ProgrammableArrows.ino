// ------------------------------------------------ ROBBUS KIDSY ----------------------------------------------
//
// SET DE JUEGOS POR DEFECTO DE ROBBUS KIDSY GAME_BUNDLE_01
// Este es el firmware que trae tu Robbus Kidsy por defecto cuando lo recibes por primera vez.
// Autor: Rocket Launcher
// Fecha: 06 de Junio de 2020
// ------------------------------------------------------------------------------------------------------------
//
// El presente firmware fue pensado en una funcionalidad multiple que integrara todas las caracteristicas
// de tu Robbus Kidsy para hacerlo entretenido, divertido y funcional.
//
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
//
// Para mas informacion sobre este programa, puedes entrar a: https://robbus.mx/kidsy
// donde encontraras posters con la informacion y mas.
// ------------------------------------------------------------------------------------------------------------

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

void actions(int);

void setup() {
  Serial.begin(115200);         // Inicializa comunicacion serial a 115200 baudios
  Kidsy.begin();                // Inicializa el hardware del Robbus Kidsy
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

void loop() {
  if(Kidsy.ButtonA.read() == PRESSED) {           // evalua el boton A
    actions(INTERACT_SOUND);                      // reproduce accion interacion
    if(flag_record == false) {                    // si al presionar el boton la bandera esta inactiva
      Serial.println("Modo grabacion - Presiona las flechas que quieras guardar");
      arrowCounter = 0;                           // reinicia contador de direcciones
      flag_record = true;                         // se se presiona boton A cambia el estado de bandera
      Kidsy.Led4.set(flag_record);                // refleja el estado grabacion en LED4
    }
  }

  while(flag_record == true) {                    // Mientras el modo grabacion esta activo
    if(Kidsy.ButtonA.read() == PRESSED) {         // Si se presiona de nuevo boton A
      if(arrowCounter > 0) {
        flag_memOk = true;                        // activa bandera de contenido en memoria
        flag_record = false;                      // desactiva bandera de grabacion
        Kidsy.Led4.set(flag_record);              // refleja el estado grabacion en LED4
        Serial.println("Fin de grabacion - Presiona boton B para reproducir");
        actions(CORRECT);                         // reproduce accion correcta
      }
      else {
        Serial.println("No puedes salir del modo de grabacion sin haber guardado ninguna flecha");
        actions(INCORRECT);                       // accion incorrecta
      }
    }
    
    arrowTouched = getArrow();                    // lee las 4 flechas ...
    if(arrowTouched != NONE) {
      if(arrowCounter < MAX_ARROWS) {               // si aun hay memoria disponible
        arrowBuffer[arrowCounter] = arrowTouched;   // guarda la direccion tocada
        Serial.print(arrowCounter);
        Serial.print(": ");
        actions(arrowTouched);                      // ejecuta la accion de la flecha correspondiente
        arrowCounter++;                             // aumenta contador
      }
      else {                                        // si la memoria llego a su limite
        actions(CORRECT);
        Serial.println("Memoria llena, presiona B para reproducir");
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
}