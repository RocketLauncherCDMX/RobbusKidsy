// ------------------------------------------------ ROBBUS KIDSY ------------------------------------------------
//
// EJEMPLO DE USO DEL NEOPIXEL
// Este ejemplo es de uso libre y esta pensado para dar una introduccion al hardware de Robbus Kidsy.
// Autor: Rocket Launcher
// Fecha: 17 de febrero de 2020
// --------------------------------------------------------------------------------------------------------------
//
// Antes de probar este programa, te invitamos a mirar primero el ejemplo NeopixelSimple.
//
// Robbus Kidsy cuenta con un LED RGB (Red, Green, Blue o rojo, verde, azul), capaz de mostrar una gama amplia
// de colores, dependiendo de los valores de cada uno de sus componentes.
// Un LED RGB tiene en su interior 3 LEDs diferentes de cada color primario, y al prenderlos con diferente
// intensidad, se logra el cambio de color.
// Como los Neopixels son de 8 bits, quiere decir que cada color puede tener un valor entre 0 (apagado) y
// 255 (encendido al maximo), por lo que 256*256*256 = ¡16,777,216 colores diferentes!
//
// El Neopixel tiene las siguientes funciones
// funciones basicas:
//   color(red,green,blue);               // componentes rojo, azul y verde, con valores de 0 a 255
//   color(color, brightness);            // nombre del color con un brillo entre 1 y 255
//   color(color);                        // nombre del color al maximo brillo
//   off();                               // apaga el neopixel
//
// asi como efectos:
//   fadeIn(color, speed);                // enciende el neopixel lentamente en un color, con intervalos de ms
//   fadeOut(color, speed);               // apaga el neopixel lentamente en un color, con intervalos de ms
//   fadeInOut(color,speed,between,end);  // enciende el led en un color, espera un tiempo, apaga, espera otro tiempo
//   heartBeat(color);                    // efecto de corazon latiendo en un color
//   heartBeat(color, brightness);        // efecto de corazon latiendo en un color, con un brillo maximo
//
// Tambien cuenta con 8 colores predefinidos, enumerados del 0 al 7
//
// BLACK         0 (Apagado)
// RED           1
// GREEN         2
// BLUE          3
// YELLOW        4
// CYAN          5
// MAGENTA       6
// WHITE         7
// 
// Nota importante: Todos los efectos del Neopixel tardan un tiempo en completarse, durante el cual, el programa
// esperara a que termine, no pudiendo hacer otra cosa.
//
// Cuando se presione el Boton A, el LED hara un latido en rojo.
// Al presionar el boton B, el LED encendera en un color.
// Con el boton C, El Neopixel apagara y encendera lentamente en magenta.
// --------------------------------------------------------------------------------------------------------------

#include<RobbusKidsy.h>

RobbusKidsy Kidsy;     // Llama a Robbus Kidsy

void setup() {
  Kidsy.begin();       // Inicializa el hardware de Robbus Kidsy
}

void loop() {
  // Si se presiona boton A, efecto latido en rojo
  if(Kidsy.ButtonA.read() == PRESSED) {
    Kidsy.Neopixel.heartBeat(RED);        // efecto de latido en rojo
  }

  // Si se presiona boton B, enciende cada componente de color (rojo, verde y azul) a un nivel diferente
  if(Kidsy.ButtonB.read() == PRESSED) {
    Kidsy.Neopixel.color(200, 60, 14);    // asigna los valores rojo, verd y azul
    delay(500);                           // espera medio segundo
    Kidsy.Neopixel.off();                 // apaga Neopixel
  }

  // Si se presiona boton C, efecto de encender y apagar lentamente en Magenta
  if(Kidsy.ButtonC.read() == PRESSED) {
    Kidsy.Neopixel.fadeInOut(MAGENTA);    // enciende y apaga el neopixel lentamente en un color aleatorio
  }
}