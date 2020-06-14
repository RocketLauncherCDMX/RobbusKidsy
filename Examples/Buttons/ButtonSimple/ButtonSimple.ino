// ------------------------------------------------ ROBBUS KIDSY ----------------------------------------------
//
// EJEMPLO DE USO DE LOS BOTONES DE USUARIO A, B y C
// Este ejemplo es de uso libre y esta pensado para dar una introduccion al hardware del robot Robbus Kidsy.
// Autor: Rocket Launcher
// Fecha: 12 de febrero de 2020
// ------------------------------------------------------------------------------------------------------------
//
// Robbus Kidsy cuenta con 3 Botones de usuario:
// - ButtonA
// - ButtonB
// - ButtonC
// Nota importante: El boton RESET sirve para reiniciar el programa de tu Robbus Kidsy y no se puede programar
// 
// Con este ejemplo entenderas el funcionamiento de los botones de usuario de tu Robot
// Simplemente usa la funcion Kidsy.ButtonX.read() donde X es A, B o C.
// ------------------------------------------------------------------------------------------------------------

#include<RobbusKidsy.h>

RobbusKidsy Kidsy;        // Llama a Robbus Kidsy

void setup() {
  Serial.begin(115200);   // Inicializa comunicacion serial a 115200 baudios
  Kidsy.begin();          // Inicializa el hardware de Robbus Kidsy
}

void loop() {
  if(Kidsy.ButtonA.read() == PRESSED) Serial.println("Boton A Presionado");
  if(Kidsy.ButtonB.read() == PRESSED) Serial.println("Boton B Presionado");
  if(Kidsy.ButtonC.read() == PRESSED) Serial.println("Boton C Presionado");
} 