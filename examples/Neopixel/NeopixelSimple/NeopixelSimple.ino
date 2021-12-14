// ------------------------------------------------ ROBBUS KIDSY ------------------------------------------------
//
// EJEMPLO DE USO DEL NEOPIXEL
// Este ejemplo es de uso libre y esta pensado para dar una introduccion al hardware de Robbus Kidsy.
// Autor: Rocket Launcher
// Fecha: 03 de marzo de 2020
// --------------------------------------------------------------------------------------------------------------
//
// Robbus Kidsy cuenta con un LED en el centro de las flechas, capaz de mostrar una gran cantidad de colores.
// Este LED, llamado Neopixel es RGB, tiene en su interior 3 LEDs diferentes: Rojo, Verde y Azul, que al 
// combinarlos en distintas proporciones, muestran un nuevo color.
//
// Tu Kidsy cuenta con 8 color predefinidos, los cuales pueden ser:
//
// BLACK
// RED
// GREEN
// BLUE
// YELLOW
// CYAN
// MAGENTA
// WHITE
//
// Y los cuales se muestran por medio de la funcion:
// Kidsy.Neopixel.color();
// --------------------------------------------------------------------------------------------------------------


#include<RobbusKidsy.h>

RobbusKidsy Kidsy;      // Llama a Robbus Kidsy

void setup() {
  Kidsy.begin();        // Inicializa el hardware de Robbus Kidsy
}

void loop() {
  Kidsy.Neopixel.color(RED);    // Muestra el color Rojo
  delay(1000);
  Kidsy.Neopixel.color(GREEN);  // Muestra el color Verde
  delay(1000);
  Kidsy.Neopixel.color(BLUE);   // Muestra el color Azul
  delay(1000);
}
