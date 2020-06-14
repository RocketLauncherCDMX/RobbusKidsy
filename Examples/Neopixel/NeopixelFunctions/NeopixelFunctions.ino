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
//   color(color, brightness);            // color con un brillo entre 1 y 255
//   color(color);                        // color al maximo brillo
//   off();                               // apaga el neopixel
//
// asi como efectos:
//   fadeIn(color, speed);                // enciende el neopixel lentamente en un color, con intervalos de ms
//   fadeOut(color, speed);               // apaga el neopixel lentamente en un color, con intervalos de ms
//   fadeInOut(color,speed,between,end);  // enciende el led en un color, espera un tiempo, apaga, espera otro tiempo
//   heartBeat(color);                    // efecto de corazon latiendo en un color
//   rainbow(wait);                       // efecto arcoiris con un retardo en ms entre un paso y el siguiente
// 
// Nota importante: Todos los efectos del Neopixel tardan un tiempo en completarse, durante el cual, el programa
// esperara a que termine, no pudiendo hacer otra cosa, esto no aplica a las funciones basicas.
//
// En este ejemplo también aprenderemos a usar la funcion random() la cual genera un numero aleatorio entre dos
// numeros distintos, por ejemplo:
//
// random(1,10);    // numero aleatorio entre 1 y 10
//
// Cuando se presione el Boton A, el LED hara un efecto.
// Al presionar el boton B, el LED encendera en un color aleatorio cada vez.
// Con el boton C, El Neopixel se apagara.
// --------------------------------------------------------------------------------------------------------------


#include<RobbusKidsy.h>

RobbusKidsy Kidsy;     // Llama a Robbus Kidsy

void setup() {
  Kidsy.begin();       // Inicializa el hardware de Robbus Kidsy
}

void loop() {
  // Si se presiona boton A, Neopixel en modo arcoiris con una separacion de 15ms entre cada cambio de tono
  if(Kidsy.ButtonA.read() == PRESSED) {
    Kidsy.Neopixel.rainbow(15);
    Kidsy.Neopixel.off();           // despues del efecto apaga Neopixel
    delay(500);
    Kidsy.Neopixel.heartBeat(RED);  // efecto de latido en rojo
  }

  // Si se presiona boton B, Neopixel en color aleatorio      rojo            verde          azul
  if(Kidsy.ButtonB.read() == PRESSED) Kidsy.Neopixel.color(random(0,255), random(0,255), random(0,255));

  // Apaga el Neopixel con boton C
  if(Kidsy.ButtonC.read() == PRESSED) Kidsy.Neopixel.off();
}