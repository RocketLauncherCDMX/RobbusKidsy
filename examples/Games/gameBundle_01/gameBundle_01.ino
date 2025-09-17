// ------------------------------------------------ ROBBUS KIDSY ----------------------------------------------
//
// SET DE JUEGOS POR DEFECTO DE ROBBUS KIDSY GAME_BUNDLE_01
// Este es el firmware que trae tu Robbus Kidsy por defecto cuando lo recibes por primera vez.
// Autor: Rocket Launcher
// Fecha: 06 de Junio de 2020
// Para mas informacion, visita www.robbus.mx/kidsy
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
// Si al seleccionar el juego tratas de reproducir las incidaciones, Kidsy te mostrara un error,
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

#include <RobbusKidsy.h>

RobbusKidsy Kidsy;             // Llama a Robbus Kidsy

// Algunos parametros que puedes configurar rapidamente en tu Robbus Kidsy son:
// ----------------------------------------------------------------------------------------------
// TODOS LOS JUEGOS
unsigned int color_intensity = 50;  // La intensidad del Neopixel (1 - 255);
short flag_sound = ON;             // ON = sonido habilitado
                                     // OFF = sonido deshabilitado
uint8_t acceleration = 60;           // aceleracion de las llantas (1 - 255);
                                    
// ----------------------------------------------------------------------------------------------
// GAME A
#define MAX_MOVES     10          // numero maximo de movimientos
#define SPEED_A       200         // velocidad de los movimientos de kidsy (minimo 1, maximo 255)
#define DURATION_A    300         // tiempo que mantendra cada movimiento en ms
#define PAUSE_TIME_A  100         // pausa entre un movimiento y el siguiente en milisegundos
// ----------------------------------------------------------------------------------------------
// GAME B
#define MAX_ARROWS    100         // limite de memoria para guardar direcciones
#define SPEED_B       200         // velocidad de los movimientos de kidsy
#define DURATION_B    350         // tiempo que mantendra cada movimiento
#define PAUSE_TIME_B  200         // pausa entre un movimiento y el siguiente en milisegundos
//
// ----------------------------------------------------------------------------------------------
// GAME C
#define FLAT_SPEED          100    // velocidad de una llanta ponchada (magenta)
#define NOMINAL_SPEED       150    // velocidad nominal (negro)
#define BOOST_SPEED         255    // velocidad con boost (amarillo)
#define SHOAL_WATER_SPEED   100    // velocidad en el agua poco profunda (cyan)
#define DEEP_WATER_SPEED    50     // velocidad en el agua profunda (azul)
#define BOOST_TIME          5000   // tiempo que dura el turbo en milisegundos
//
// ----------------------------------------------------------------------------------------------

#include "common_functions.h"
#include "game_a.h"
#include "game_b.h"
#include "game_c.h"

void setup() {
  Serial.begin(115200);         // Inicializa comunicacion serial a 115200 baudios
  Kidsy.begin();                // Inicializa el hardware del Robbus Kidsy
  configureTimer();             // Inicializacion y configuracion del Timer para llantas

  // Si conectas tu Kidsy por USB y abres su terminal serial, podras ver que esta haciendo
  Serial.println("cof cof... perdona, olvidaba que no hablas robot, lo dire en español.");
  Serial.println("Hola, mi nombre es Robbus Kidsy, y tengo 3 juegos diferentes para ti:");
  Serial.println("BOTON A - Kidsy dice");
  Serial.println("BOTON B - Guia a Kidsy");
  Serial.println("BOTON C - Kidsy Kart");
  Serial.println("Para comenzar a jugar, presiona el boton del juego que quieras.");
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
        actions(INTERACT_SOUND);              // accion de interaccion con el boton
        kidsyStatus = GAME_B;                 // selecciona juego B
      }
      if(Kidsy.ButtonC.read() == PRESSED) {
        actions(INTERACT_SOUND);              // accion de interaccion con el boton
        kidsyStatus = GAME_C;                 // selecciona juego C
      }
      break;
      
    // caso A, ingresa al juego A.
    case GAME_A:
      call_game_a();
      break;
    // caso B, ingresa al juego B.
    case GAME_B:
      call_game_b();
      break;
    // caso C, ingresa al juego C.
    case GAME_C:
      call_game_c();
      break;
  }
}