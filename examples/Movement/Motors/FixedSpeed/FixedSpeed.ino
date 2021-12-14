// ------------------------------------------------ ROBBUS KIDSY ----------------------------------------------
//
// EJEMPLO DE USO DE LOS MOTORES IZQUIERDO Y DERECHO A UNA VELOCIDAD FIJA
// Este ejemplo es de uso libre y esta pensado para dar una introduccion al hardware de Robbus Kidsy.
// Autor: Rocket Launcher
// Fecha: 20 de febrero de 2020
// ------------------------------------------------------------------------------------------------------------
//
// Robbus Kidsy cuenta con 2 motorreductores:
// - MotorLeft
// - MotorRight
// 
// Con este ejemplo entenderas el funcionamiento de los motores que impulsan a tu Robot
// Si quieres mover las llantas por separado, basta con utilizar las funciones:
//
// Kidsy.Move.MotorLeft(speed);
// Kidsy.Move.MotorRight(speed);
//
// speed sera la velocidad que quieres darle a un motor en especifico y puede ser desde -255 a 255.
// -255 -> Velocidad maxima hacia atras en ese motor
//  255 -> Velocidad maxima hacia adelante en ese motor
//    0 -> Motor detenido
//
// Y si quieres mover a tu robot puedes usar las funciones:
//
// Kidsy.Move.forward(speed);   -> Avanza hacia el frente con la velocidad de speed
// Kidsy.Move.backward(speed);  -> Avanza hacia atras con la velocidad de speed
// Kidsy.Move.turnLeft(speed);  -> Vuelta a la izquierda con velocidad de speed
// Kidsy.Move.turnRight(speed); -> Vuelta a la derecha con velocidad de speed
// Kidsy.Move.stop();           -> Detiene ambas llantas
//
// Cuando usas las funcones de movimiento del robot (no de las llantas), el valor de speed solamente
// puede ser positivo, puesto que la direccion ya esta dada en la funcion
// speed sera la velocidad que quieres darle al movimiento del robot y puede ser desde 0 a 255.
// 255 -> Velocidad maxima del robot hacia un lado especifico
//   0 -> Robot detenido
// 
// Considera que valores cercanos a 0 ya sean positivos o negativos, pueden no generar la fuerza
// suficiente para comenzar a girar las ruedas.
//
// En conclusion, las funciones para mover las llantas, requieren velocidad con signo para saber la direccion, 
// y las funciones para mover el robot no requiere velocidad con signo, puesto que la direccion ya esta dicha.
// ------------------------------------------------------------------------------------------------------------

#include<RobbusKidsy.h>

RobbusKidsy Kidsy;        // Llama a Robbus Kidsy

// Variables para la velocidad de los motores, cambiando su valor controlas velocidad y sentido.
int speedLeft = 100;                // 100 hacia el FRENTE, puedes poner un valor entre -255 y 255
int speedRight = -100;              // 100 hacia ATRAS, puedes poner un valor entre -255 y 255
unsigned int speedForward = 150;    // robot hacia adelante en 150, puedes poner un valor entre 0 y 255

void setup() {
  Kidsy.begin();          // Inicializa el hardware del Robbus Kidsy
}

void loop() {
  Kidsy.ButtonA.read();   // captura estado nuevo del boton A
  Kidsy.ButtonB.read();   // captura estado nuevo del boton B
  Kidsy.ButtonC.read();   // captura estado nuevo del boton C

  // REVISION DEL BOTON A
  // ----------------------
  if(Kidsy.ButtonA.status == PRESSED) {
    Kidsy.Move.MotorLeft(speedLeft);        // Si el boton se presiono, acciona motor izquierdo
  }
  if(Kidsy.ButtonA.status == RELEASED) {
    Kidsy.Move.MotorLeft(0);                // Si el boton se solto, llanta izquierda detenida
  }

  // REVISION DEL BOTON B
  // ----------------------
  if(Kidsy.ButtonB.status == PRESSED) {
    Kidsy.Move.forward(speedForward);       // Si el boton se presiono, robot hacia el frente
  }
  if(Kidsy.ButtonB.status == RELEASED) {
    Kidsy.Move.stop();                      // Si el boton se solto, robot detenido
  }
  
  // REVISION DEL BOTON C
  // ----------------------
  if(Kidsy.ButtonC.status == PRESSED) { 
    Kidsy.Move.MotorRight(speedRight);       // Si el boton se presiono, acciona ambos motores
  }
  if(Kidsy.ButtonC.status == RELEASED) { 
    Kidsy.Move.MotorRight(0);                // Si el boton se solto, llanta derecha detenida
  }
} 
