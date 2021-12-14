// ------------------------------------------------ ROBBUS KIDSY ------------------------------------------------
//
// EJEMPLO DE LECTURA DIGITAL DE LAS FLECHAS DE DIRECCION
// Este ejemplo es de uso libre y esta pensado para dar una introduccion al hardware de Robbus Kidsy
// Autor: Rocket Launcher
// Fecha: 28 de febrero de 2020
// --------------------------------------------------------------------------------------------------------------
//
// Kidsy Robbus cuenta con 4 flechas de direccion:
// - ArrowForward
// - ArrowBackward
// - ArrowLeft
// - ArrowRight
//
// Estas flechas son sensibles al tacto. Para leer su valor basta con usar la funcion read() la cual:
// Leera un 1 o TOUCHED cuando se toca
// Leera un 0 o NOTOUCHED cuando se suelta
//
// Y posteriormente usar la variable status para saber su valor, por ejemplo:
//
// Kidsy.ArrowForward.read();                   // Lee la flecha Adelante
// if(Kidsy.ArrowForward.status == TOUCHED) {}  // Evalua si la flecha se presiono
//
// Tambien puedes guardar lo que la funcion leyo en una variable declarada por ti:
// int forward = Kidsy.ArrowForward.read();
// --------------------------------------------------------------------------------------------------------------

#include<RobbusKidsy.h>

RobbusKidsy Kidsy;        // Llama a tu Robbus Kidsy

void setup() {
  Serial.begin(115200);   // Inicializa comunicacion serial a 115200 baudios
  Kidsy.begin();          // Inicializa el hardware del Robbus Kidsy
}

void loop() {
  int forward = Kidsy.ArrowForward.read();      // guarda el valor de flecha Adelante
  int backward = Kidsy.ArrowBackward.read();    // guarda el valor de flecha Atras
  int left = Kidsy.ArrowLeft.read();            // guarda el valor de flecha Izquierda
  int right = Kidsy.ArrowRight.read();          // guarda el valor de flecha Derecha
  
  if(forward == TOUCHED) Serial.println("Flecha adelante se presiono");
  else if(forward == NOTOUCHED) Serial.println("Flecha adelante se solto");

  if(backward == TOUCHED) Serial.println("Flecha atras se presiono");
  else if(backward == NOTOUCHED) Serial.println("Flecha atras se solto");

  if(left == TOUCHED) Serial.println("Flecha izquierda se presiono");
  else if(left == NOTOUCHED) Serial.println("Flecha izquierda se solto");

  if(right == TOUCHED) Serial.println("Flecha derecha se presiono");
  else if(right == NOTOUCHED) Serial.println("Flecha derecha se solto");
}