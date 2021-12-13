// ------------------------------------------------ ROBBUS KIDSY ----------------------------------------------
//
// EJEMPLO DE MOVIMIENTO POR MEDIO DE LAS FLECHAS ADELANTE, ATRAS, IZQUIERDA, DERECHA 
// Este ejemplo es de uso libre y esta pensado para dar una introduccion al hardware de Robbus Kidsy.
// Autor: Rocket Launcher
// Fecha: 20 de febrero de 2020
// ------------------------------------------------------------------------------------------------------------
//
// En este ejemplo conoceremos las funciones basicas de movimiento de Robbus.
//
// Robbus Kidsy cuenta con 2 motorreductores:
// - MotorLeft
// - MotorRight
//
// Y con 4 flechas de direccion:
// - ArrowForward
// - ArrowBackward
// - ArrowLeft
// - ArrowRight
//
// Para leer una flecha, basta que uses la funcion read(), y luego consultes la variable status,
// de la flecha en cuestion, por ejemplo:
// 
// Kidsy.ArrowForward.read();                 // Lee la flecha Adelante
// if(Kidsy.ArrowForward.status == TOUCHED)   // Sera verdadero al tocar la flecha
// 
// Para mover las llantas de tu robot, puedes hacerlo con las funciones:
//
// Kidsy.Move.MotorLeft(speed);
// Kidsy.Move.MotorRight(speed);
//
// O bien, puedes mover directamente a tu robot con las funciones:
//
// Kidsy.Move.forward(speed);
// Kidsy.Move.backward(speed);
// Kidsy.Move.turnLeft(speed);
// Kidsy.Move.turnRight(speed);
//
// Que haran que Robbus se mueva adelante, atras, y gire sobre su eje a la izquierda y derecha.
//
// speed sera la velocidad que quieres darle a un motor en especifico y puede ser desde -255 a 255.
//
//  255 -> Velocidad maxima hacia adelante en ese motor
//    0 -> Motor detenido
// -255 -> Velocidad maxima hacia atras en ese motor
// 
// Considera que valores cercanos a 0 ya sean positivos o negativos, pueden no generar la fuerza
// suficiente para comenzar a girar las ruedas.
//
// El siguiente codigo accionara las llantas de Robbus dependiendo que flecha se toque.
// ------------------------------------------------------------------------------------------------------------

#include<RobbusKidsy.h>

RobbusKidsy Kidsy;    // Llama a Robbus Kidsy

int speed = 100;      // velocidad para las distintas funciones de movimiento

void setup() {
  Serial.begin(115200);   // Inicializa la comunicacion serial por USB a 115200 baudios
  Kidsy.begin();          // Inicializa el hardware del Robbus Kidsy
}

void loop() {
  Kidsy.ArrowForward.read();    // captura estado nuevo de la flecha Frente
  Kidsy.ArrowBackward.read();   // captura estado nuevo de la flecha Atras
  Kidsy.ArrowLeft.read();       // captura estado nuevo de la flecha Izquierda
  Kidsy.ArrowRight.read();      // captura estado nuevo de la flecha Derecha

  // REVISION DE LA FLECHA FRENTE
  // ----------------------
  if(Kidsy.ArrowForward.status == TOUCHED) {
    Kidsy.Move.forward(speed);                  // Si el boton se presiono, avanza hacia ADELANTE
    delay(1000);                                // espera 1 segundo
    Kidsy.Move.stop();                          //detiene el Robbus
  }
  if(Kidsy.ArrowForward.status == NOTOUCHED) {
    // Puedes decirle que hacer al soltar la flecha                          
  }

  // REVISION DE LA FLECHA ATRAS
  // ----------------------
  if(Kidsy.ArrowBackward.status == TOUCHED) {
    Kidsy.Move.backward(speed);                 // Si el boton se presiono, avanza hacia ATRAS
    delay(1000);                                // espera 1 segundo
    Kidsy.Move.stop();                          //detiene el Robbus
  }
  if(Kidsy.ArrowBackward.status == NOTOUCHED) {
    // Puedes decirle que hacer al soltar la flecha  
  }

  // REVISION DE LA FLECHA IZQUIERDA
  // ----------------------
  if(Kidsy.ArrowLeft.status == TOUCHED) {
    Kidsy.Move.turnLeft(speed);                 // Si el boton se presiono, avanza gira a la IZQUIERDA
    delay(1000);                                // espera 1 segundo
    Kidsy.Move.stop();                          //detiene el Robbus
  }
  if(Kidsy.ArrowLeft.status == NOTOUCHED) {
    // Puedes decirle que hacer al soltar la flecha  
  }

  // REVISION DE LA FLECHA DERECHA
  // ----------------------
  if(Kidsy.ArrowRight.status == TOUCHED) {
    Kidsy.Move.turnRight(speed);                // Si el boton se presiono, gira a la DERECHA
    delay(1000);                                // espera 1 segundo
    Kidsy.Move.stop();                          //detiene el Robbus
  }
  if(Kidsy.ArrowRight.status == NOTOUCHED) {
    // Puedes decirle que hacer al soltar la flecha  
  }
} 
