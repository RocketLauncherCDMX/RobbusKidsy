// ------------------------------------------------ ROBBUS KIDSY ------------------------------------------------
//
// EJEMPLO DE LECTURA ANALOGICA DE LAS FLECHAS DE DIRECCION
// Este ejemplo es de uso libre y esta pensado para dar una introduccion al hardware de Robbus Kidsy.
// Autor: Rocket Launcher
// Fecha: 20 de febrero de 2020
// --------------------------------------------------------------------------------------------------------------
//
// Robbus Kidsy cuenta con 4 flechas de direccion:
// - ArrowForward
// - ArrowBackward
// - ArrowLeft
// - ArrowRight
//
// Estas flechas son sensibles al tacto. Para leer su valor analogico se usa la funcion AnalogRead() la cual
// leera un valor entre 0 y 255 (8 bits) del convertidor ADC
//
// Y posteriormente usar la variable analog para saber su valor, por ejemplo:
//
// Kidsy.ArrowForward.analogRead();             // Lee la flecha Adelante en formato analogico
// Serial.println(Kidsy.ArrowForward.analog);   // Imprime el valor obtenido
// --------------------------------------------------------------------------------------------------------------

#include<RobbusKidsy.h>

RobbusKidsy Kidsy;        // Llama a tu Robbus Kidsy

void setup() {
  Serial.begin(115200);   // Inicializa comunicacion serial a 115200 baudios
  Kidsy.begin();          // Inicializa el hardware del Robbus Kidsy
}

void loop() {
  int analogForward = Kidsy.ArrowForward.analogRead();     // Lee la flecha Adelante
  int analogBackward = Kidsy.ArrowBackward.analogRead();   // Lee la flecha Atras
  int analogLeft = Kidsy.ArrowLeft.analogRead();           // Lee la flecha Izquierda
  int analogRight = Kidsy.ArrowRight.analogRead();         // Lee la flecha Derecha
  
  Serial.print("Forward: ");
  Serial.print(analogForward);    // Imprime el valor analogico de la flecha adelante
  Serial.print(", Backward: ");
  Serial.print(analogBackward);   // Imprime el valor analogico de la flecha atras
  Serial.print(", Left: ");
  Serial.print(analogLeft);       // Imprime el valor analogico de la flecha izquierda
  Serial.print(", Right: ");
  Serial.println(analogRight);    // Imprime el valor analogico de la flecha derecha
}
