// ------------------------------------------------ ROBBUS KIDSY ----------------------------------------------
//
// EJEMPLO DE USO DE LOS LEDS DE USUARIO LED1, LED2 LED3 y LED4
// Este ejemplo es de uso libre y esta pensado para dar una introduccion al hardware de Robbus Kidsy.
// Autor: Rocket Launcher
// Fecha: 17 de febrero de 2020
// ------------------------------------------------------------------------------------------------------------
// Robbus Kidsy cuenta con 4 LEDs de usuario:
// - Led1
// - Led2
// - Led3
// - Led4
//
// Nota importante: El LED blanco (LEDW) junto al sensor de color se controla por medio del sensor de color
//
// la funcion on(); enciende el led en cuestion, mientras que la funcion off(); lo apaga.
// ------------------------------------------------------------------------------------------------------------

#include<RobbusKidsy.h>

RobbusKidsy Kidsy;    // Llama a Robbus Kidsy

void setup() {
  Kidsy.begin();      // Inicializa el hardware del Robbus Kidsy
}

void loop() {
  Kidsy.Led1.on();    // Enciende LED 1
  Kidsy.Led4.off();   // Apaga LED 4
  delay(100);         // Espera 100 mili segundos
  Kidsy.Led2.on();    // Repite la secuencia para el siguiente par de LEDs...
  Kidsy.Led1.off();   
  delay(100);
  Kidsy.Led3.on();    
  Kidsy.Led2.off();  
  delay(100);
  Kidsy.Led4.on();  
  Kidsy.Led3.off();   
  delay(100);
} 