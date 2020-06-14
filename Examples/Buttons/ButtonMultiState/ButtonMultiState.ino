// ------------------------------------------------ ROBBUS KIDSY ----------------------------------------------
//
// EJEMPLO DE USO DE LOS BOTONES DE USUARIO A, B y C CON PULSACION LARGA Y MANTENIDA
// Este ejemplo es de uso libre y esta pensado para dar una introduccion al hardware de Robbus Kidsy.
// Autor: Rocket Launcher
// Fecha: 17 de febrero de 2020
// ------------------------------------------------------------------------------------------------------------
// Robbus Kidsy cuenta con 3 Botones de usuario:
// - ButtonA
// - ButtonB
// - ButtonC
//
// Con este ejemplo entenderas los diferentes usos que se le pueden dar a los botones de tu Robot.
// Los botones de usuario A, B y C pueden regresar 4 estados diferentes:
//
// 0 - RELEASED       - El boton paso de estar presionado a liberarse
// 1 - PRESSED        - El boton paso de estar en en reposo a presionarse
// 2 - HOLD_PRESSED   - Se mantiene presionado
// 3 - HOLD_RELEASED  - Se mantiene sin preisonar
//
// Nota importante: El boton RESET sirve para reiniciar el programa de tu robot y no se puede programar
// ------------------------------------------------------------------------------------------------------------

#include<RobbusKidsy.h>

RobbusKidsy Kidsy;        // Llama a Robbus Kidsy

int times_A;
int times_B;
int times_C;              // Cuenta las pulsaciones de los botones

void setup() {
  Serial.begin(115200);   // Inicializa comunicacion serial a 115200 baudios
  Kidsy.begin();          // Inicializa el hardware del Robbus Kidsy
}

void loop() {
  Kidsy.ButtonA.read();   // Lee el ultimo estado del boton A
  Kidsy.ButtonB.read();   // Lee el ultimo estado del boton B
  Kidsy.ButtonC.read();   // Lee el ultimo estado del boton C

  switch(Kidsy.ButtonA.status) {  
    case PRESSED:
      Serial.print("BotonA Presionado ");
      Serial.print(++times_A);    // aumenta el contador y lo imprime
      Serial.println(" veces");
      Kidsy.Led1.on();  // enciende el LED1
      break;
    case RELEASED:
      Serial.println("BotonA suelto");
      Kidsy.Led1.off();  // apaga el LED1
      break;
    case HOLD_PRESSED:
      // mientras el boton permanezca presionado, entrara en este case
      //Serial.println("BotonA permanece presionado");
      break;
    case HOLD_RELEASED:
      // mientras el boton permanezca suelto, entrara en este case
      //Serial.println("BotonA permanece suelto");
      break;
  }

  switch(Kidsy.ButtonB.status) {
    case PRESSED:
      Serial.print("BotonB Presionado ");
      Serial.print(++times_B);    // aumenta el contador y lo imprime
      Serial.println(" veces");
      Kidsy.Led2.on();   // enciende el LED2
      break;
    case RELEASED:
      Serial.println("BotonB suelto");
      Kidsy.Led2.off();   // apaga el LED2
      break;
    case HOLD_PRESSED:
      // mientras el boton permanezca presionado, entrara en este case
      //Serial.println("BotonB permanece presionado");
      break;
    case HOLD_RELEASED:
      // mientras el boton permanezca suelto, entrara en este case
      //Serial.println("BotonB permanece suelto");
      break;
  }

  switch(Kidsy.ButtonC.status) {
    case PRESSED:
      Serial.print("BotonC Presionado ");
      Serial.print(++times_C);    // aumenta el contador y lo imprime
      Serial.println(" veces");
      Kidsy.Led3.on();   // enciende el LED3
      break;
    case RELEASED:
      Serial.println("BotonC suelto");
      Kidsy.Led3.off();   // apaga el LED3
      break;
    case HOLD_PRESSED:
      // mientras el boton permanezca presionado, entrara en este case
      //Serial.println("BotonC permanece presionado");
      break;
    case HOLD_RELEASED:
      // mientras el boton permanezca suelto, entrara en este case
      //Serial.println("BotonC permanece suelto");
      break;
  }
} 