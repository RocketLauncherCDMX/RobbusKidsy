// ------------------------------------------------ ROBBUS KIDSY ------------------------------------------------
//
// EJEMPLO DE USO DEL NEOPIXEL CON REGULACION DE BRILLO
// Este ejemplo es de uso libre y esta pensado para dar una introduccion al hardware de Robbus Kidsy.
// Autor: Rocket Launcher
// Fecha: 03 de marzo de 2020
// --------------------------------------------------------------------------------------------------------------
// Si aun no has visto el ejemplo NeopixelSimple, te invitamos a probarlo primero.
// 
// Robbus Kidsy cuenta con un LED en el centro de las flechas, capaz de mostrar una gran cantidad de colores.
// Este LED, llamado Neopixel es RGB, tiene en su interior 3 LEDs diferentes: Rojo, Verde y Azul, que al 
// combinarlos en distintas proporciones, muestran un nuevo color. Estos colores, a su vez pueden ser regulados
// para variar el brillo.
//
// Tu Kidsy cuenta con 8 color predefinidos, los cuales pueden ser:
//
// BLACK (Apagado)
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
// 
// Esta funcion puede regular el brillo del Neopixel, tanto en color solido, como en los efectos, por ejemplo:
// Kidsy.Neopixel.color(RED, 100);         // Mostrara el color rojo con un brillo de 100 (maximo 255)
//
// Tambien dispone de algunos interesantes efectos, como un latido:
// Kidsy.Neopixel.heartBeat(GREEN, 50);   // Mostrara el efecto heartBeat con un brillo de 50 (maximo 255);
// --------------------------------------------------------------------------------------------------------------

#include<RobbusKidsy.h>

RobbusKidsy Kidsy;      // Llama a Robbus Kidsy

void setup() {
  Kidsy.begin();        // Inicializa el hardware de Robbus Kidsy
}

void loop() {
  Kidsy.Neopixel.color(RED, 50);    // Muestra el color Rojo con brillo 50
  delay(1000);
  Kidsy.Neopixel.color(GREEN, 50);  // Muestra el color Verde con brillo 50
  delay(1000);
  Kidsy.Neopixel.color(BLUE, 50);   // Muestra el color Azul con brillo 50
  delay(1000);

  Kidsy.Neopixel.heartBeat(CYAN, 50);    // Muestra latido en Cyan con brillo 50
  delay(1000);
  Kidsy.Neopixel.heartBeat(MAGENTA, 100);  // Muestra latido en Magenta con brillo 100
  delay(1000);
  Kidsy.Neopixel.heartBeat(YELLOW, 255);   // Muestra latido en Amarillo con brillo al maximo
  delay(1000);
}