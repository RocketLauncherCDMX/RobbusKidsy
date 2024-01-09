// ------------------------------------------------ ROBBUS KIDSY ----------------------------------------------
//
// EJEMPLO DE LECTURA DEL PAD DE FLECHAS TACTILES
// Este ejemplo es de uso libre y esta pensado para dar una introduccion al hardware de Robbus Kidsy.
// Autor: Rocket Launcher
// Fecha: 06 de diciembre de 2021
// Visita www.robbus.mx/kidsy para mas informacion
// ------------------------------------------------------------------------------------------------------------
//
// En este ejemplo conoceremos la forma de leer el pad de flechas tactiles.
//
// Robbus Kidsy cuenta con 4 flechas de direccion:
// - ArrowForward
// - ArrowBackward
// - ArrowLeft
// - ArrowRight
//
// Para leer una flecha, basta que uses la funcion read(), y luego consultes la variable status,
// de la flecha en cuestion, por ejemplo:
// 
// Kidsy.ArrowForward.read();                 // Lee la flecha Adelante
// if(Kidsy.ArrowForward.status == TOUCHED)   // Usando el valor obtenido en un if
// 
// No obstante, habra ocasiones en las que necesites identificar que flecha se presiono.
// Para esta tarea existe una funcion llamada
//
// Kidsy.getArrowPad();
//
// La cual devuelve el valor de la flecha presionada.

// Para mover las llantas de tu robot, puedes hacerlo con las funcion:
//
// if(Kidsy.getArrowPad()) == FORWARD) {}   // Evalua si de las cuatro flechas, se presiono adelante
// if(Kidsy.getArrowPad()) == BACKWARD) {}  // Evalua si de las cuatro flechas, se presiono atras
// if(Kidsy.getArrowPad()) == LEFT) {}      // Evalua si de las cuatro flechas, se presiono izquierda
// if(Kidsy.getArrowPad()) == RIGHT) {}     // Evalua si de las cuatro flechas, se presiono derecha
//
// Tambien puedes usar una sentencia switch para evaluar la flecha presionada.
//
// El siguiente codigo imprime que flecha se presiona del pad.
// ------------------------------------------------------------------------------------------------------------

#include<RobbusKidsy.h>

RobbusKidsy Kidsy;    // Llama a Robbus Kidsy

int arrow;            // variable para guardar el valor de la flecha presionada

void setup() {
  Serial.begin(115200);   // Inicializa la comunicacion serial por USB a 115200 baudios
  Kidsy.begin();          // Inicializa el hardware del Robbus Kidsy
}

void loop() {
  int arrow = Kidsy.getArrowPad();    // La funcion lee las 4 flechas a la vez
  if(arrow != 0) {                    // si alguna flecha se presiono, ejecuta lo siguiente
    Serial.print("Flecha ");
    switch(arrow) {
      case FORWARD:   Serial.print("Adelante");   break;
      case BACKWARD:  Serial.print("Atras");      break;
      case LEFT:      Serial.print("Izquierda");  break;
      case RIGHT:     Serial.print("Derecha");    break;
    }
    Serial.println(" presionada");
  }
} 