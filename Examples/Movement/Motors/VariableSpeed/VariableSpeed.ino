// ------------------------------------------------ ROBBUS KIDSY ----------------------------------------------
//
// EJEMPLO DE USO DE LOS MOTORES IZQUIERDO Y DERECHO CON VELOCIDAD VARIABLE
// Este ejemplo es de uso libre y esta pensado para dar una introduccion al hardware de Robbus Kidsy.
// Autor: Rocket Launcher
// Fecha: 22 de febrero de 2020
// ------------------------------------------------------------------------------------------------------------
//
// Robbus Kidsy cuenta con 2 motorreductores:
// - MotorLeft
// - MotorRight
// 
// Con este ejemplo entenderas el funcionamiento de los motores que impulsan a tu robus Kidsy, al variar
// su velocidad por medio de un ciclo for.
// Si no sabes como funciona el ciclo for, te invitamos a probar el ejemplo for en sentenciasCondicionales
// Para mover los motores por separado, basta con utilizar las funciones:
//
// Kidsy.Move.MotorLeft(speed);
// Kidsy.Move.MotorRight(speed);
//
// speed sera la velocidad que quieres darle a un motor en especifico y puede ser desde -255 a 255.
//
// -255 -> Velocidad maxima hacia atras en ese motor
//  255 -> Velocidad maxima hacia adelante en ese motor
//    0 -> Motor detenido
//
// Lo que se hara en el ciclo for es ir aumentando y disminuyendo el valor de speed de forma constante.
// 
// Considera que valores cercanos a 0 ya sean positivos o negativos, pueden no generar la fuerza
// suficiente para comenzar a girar las ruedas.
// ------------------------------------------------------------------------------------------------------------

#include<RobbusKidsy.h>

RobbusKidsy Kidsy;        // Llama a Robus Kidsy

// Variables para la velocidad de los motores
int speedLeft = 0;        // Motor izquierdo comienza detenido 
int speedRight = 0;       // Motor derecho comienza detenido
int wait = 20;            // tiempo entre una velocidad y otra

void setup() {
  Kidsy.begin();          // Inicializa el hardware del Robbus Kidsy
}

void loop() {
  for(int i=0; i<=255; i++) {   // ciclo for de 0 a 255
    Kidsy.Move.MotorLeft(i);    // mueve la llanta izquierda con el valor positivo (Adelante)
    Kidsy.Move.MotorRight(-i);  // mueve la llanta derecha con el valor negativo (Atras)
    delay(wait);
  }
  for(int i=255; i>0; i--) {    // ciclo for de 255 a 0
    Kidsy.Move.MotorLeft(i);
    Kidsy.Move.MotorRight(-i);
    delay(wait);
  }
  for(int i=0; i<=255; i++) {   // ciclo for de 0 a 255
    Kidsy.Move.MotorLeft(-i);   // invierte la direccion de las llantas mientras acelera
    Kidsy.Move.MotorRight(i);
    delay(wait);
  }
  for(int i=255; i>0; i--) {    // ciclo for de 255 a 0
    Kidsy.Move.MotorLeft(-i);   // invierte la direccion mientras desacelera
    Kidsy.Move.MotorRight(i);
    delay(wait);
  }
} 
