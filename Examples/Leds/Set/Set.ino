// ------------------------------------------------ ROBBUS KIDSY ----------------------------------------------
//
// EJEMPLO DE USO DE LOS LEDS DE USUARIO LED1, LED2, LED3 y LED4 CON LOS BOTONES A, B y C CON LA FUNCION SET
// Este ejemplo es de uso libre y esta pensado para dar una introduccion al hardware de Robbus Kidsy.
// Autor: Rocket Launcher
// Fecha: 17 de febrero de 2020
// ------------------------------------------------------------------------------------------------------------
//
// Antes de ver este ejemplo, te recomendamos probar el ejemplo Blink.
//
// Robbus Kidsy cuenta con 4 LEDs de usuario:
// - Led1
// - Led2
// - Led3
// - Led4
// Nota importante: El LED blanco junto al sensor de color no se puede controlar directamente
//
// En este ejemplo aprenderemos usar la funcion set para prender o apagar un LED. Tambien aprenderemos 
// el uso del signo ! (negacion). Este operador sirve para invertir el estado de una variable booleana, 
// es decir, de un bit 0 o 1.
// Anteponiendo "!" a una variable booleana (1 o 0), su valor cambiara.
//
// Ejemplo:
// bool direccion = 0;      // variable direccion inica en 0
// direccion = !direccion   // invierte el valor, ahora vale 1
// ------------------------------------------------------------------------------------------------------------

#include<RobbusKidsy.h>

RobbusKidsy Kidsy;                // Llama a Robbus Kidsy

bool state_led1 = 1;              // Led1 comienza encendido
bool state_led2 = 0;              // Led2 comienza apagado
bool state_led3 = 1;              // Led3 comienza apagado
bool state_led4 = 0;              // Led4 comienza apagado

void setup() {
  Kidsy.begin();                  // Inicializa el hardware del Robbus Kidsy
  Kidsy.Led1.set(state_led1);     // La funcion set prende o apaga el LED con 1 o 0 respectivamente
  Kidsy.Led2.set(state_led2);
  Kidsy.Led3.set(state_led3);     
  Kidsy.Led4.set(state_led4);
}

void loop() {
  if(Kidsy.ButtonA.read() == PRESSED) {
    state_led1 = !state_led1;             // Cambia el valor de state_led1 y state_led2
    state_led2 = !state_led2;
  }
  if(Kidsy.ButtonB.read() == PRESSED) {
    state_led2 = !state_led2;             // Cambia el valor de state_led2 y state_led3
    state_led3 = !state_led3;
  }
  if(Kidsy.ButtonC.read() == PRESSED) {
    state_led3 = !state_led3;             // Cambia el valor de state_led3 y state_led4
    state_led4 = !state_led4;
  }

  Kidsy.Led1.set(state_led1);     // La funcion set prende o apaga el LED con 1 o 0 respectivamente
  Kidsy.Led2.set(state_led2);
  Kidsy.Led3.set(state_led3);     
  Kidsy.Led4.set(state_led4);
} 