// ------------------------------------------------ ROBBUS KIDSY ------------------------------------------------
//
// RECALIBRACION DE LAS FLECHA TACTILES
// Este ejemplo es de uso libre y esta pensado para dar una introduccion al hardware de Robbus Kidsy.
// Autor: Rocket Launcher
// Fecha: 27 de Abril de 2023
// Visita www.robbus.mx/kidsy para mas informacion
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

// No siempre las flechas detectan adecuadamente el toque, esto puede deverse a que con el tiempo
// la conductividad electrica difiera ligeramente de cuando se calibro, no obstante, tu puedes recalibrarlas.
// El procedimiento para hacerlo es el siguiente:
//
// 1. Si has cambiado el programa de Robbus Kidsy, asegurate que la funcion Kidsy.begin() esten en tu programa.
// 2. Prende tu Kidsy, si no enciende, puede deberse a falta de bateria, recuerda recargarlo.
// 3. Configura el puerto Serial a 115200 Baudios. Para la calibracion, no es necesario que este conectado por
//    USB, pero te sera mas sencillo seguir las instrucciones por Serial.
// 4. Manten presionado el boton B mientras presionas y sueltas el boton RESET.
// 5. Sabras que has entrado al modo de calibracion de las flechas tactiles por que parpadeara el Neopixel en verde.
// 6. Si estas conectado por USB, sigue los pasos que se te indican, de lo contrario continua leyendo.
// 7. Los 4 LEDs de usuario se encenderan, sin presionar ninguna flecha, deberas presionar el boton B, esto
//    calibrara el estado sin tocar de las mismas.
// 8. Ahora se encendera solamente un LED, lo que significa que deberas mantener presionada una flecha mientras 
//    presiona el boton B. Para una mejor lectura, usa ambas manos. Cada LED representa una flecha distinta,
//    la secuencia es:
//    - LED1 -> Adelante
//    - LED2 -> Atras
//    - LED3 -> Izquierda
//    - LED4 -> Derecha
// 9. Una vez que termines, el Neopixel parpadeara en verde y la calibracion habra concluido.
// 
// Cuando completes la calibracion, los valores se quedaran guardados en la memoria Flash del microcontrolador,
// por lo que aun despues de apagarlo y volverlo a encender, Kidsy recordara esos mismos valores.
//
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