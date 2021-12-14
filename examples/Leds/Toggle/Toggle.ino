// ------------------------------------------------ ROBBUS KIDSY ----------------------------------------------
//
// EJEMPLO DE USO DE LOS LEDS DE USUARIO LED1, LED2, LED3 Y LED4
// Este ejemplo es de uso libre y esta pensado para dar una introduccion al hardware de Robbus Kidsy.
// Autor: Rocket Launcher
// Fecha: 17 de febrero de 2020
// ------------------------------------------------------------------------------------------------------------
//
// Antes de probar este codigo te recomendamos mirar el ejemplo Blink.
//
// Robbus Kidsy cuenta con 4 LEDs de usuario:
// - Led1
// - Led2
// - Led3
// - Led4
// Nota importante: El LED blanco junto al sensor de color no se puede controlar directamente.
//
// En este ejemplo aprenderemos a utilizar la funcion toggle para cambiar el estado de un LED. Si el LED esta
// encendido, al usar toggle se apagara, y si esta apagado, se encendera. Al hacerlo, indirectamente se cambia 
// el estado de su variable status, por lo que, consultando esa variable, se puede saber el estado actual 
// del LED.
//
// Cabe mencionar que el uso de Toggle es una mejor forma de controlar LEDs que el ejemplo set, el cual requiere
// de una variable para cambiar el estado.
// ------------------------------------------------------------------------------------------------------------

#include<RobbusKidsy.h>

RobbusKidsy Kidsy;        // Llama a Robbus Kidsy

void setup() {
  Serial.begin(115200);   // Inicializa comunicacion serial a 115200 baudios
  Kidsy.begin();          // Inicializa el hardware del Robbus Kidsy
}

void loop() {
  if(Kidsy.ButtonA.read() == PRESSED) {
    Kidsy.Led1.toggle();  // Al presionar el boton, el LED1 cambia de estado
    Serial.print("El Led1 que estaba ");

    // Ahora, revisamos el estado del LED...
    if(Kidsy.Led1.status == 1) Serial.println("apagado pasa a estar encendido");
    else Serial.println("encendido pasa a estar apagado");
  }
} 