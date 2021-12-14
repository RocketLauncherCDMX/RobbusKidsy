// ------------------------------------------------ ROBBUS KIDSY ------------------------------------------------
//
// EJEMPLO DE USO DEL NEOPIXEL EN CONJUNTO CON EL SENSOR DE COLOR
// Este ejemplo es de uso libre y esta pensado para dar una introduccion al hardware de Robbus Kidsy.
// Autor: Rocket Launcher
// Fecha: 8 de febrero de 2020
// Visita www.robbus.mx/kidsy para mas informacion
// --------------------------------------------------------------------------------------------------------------
//
// Robbus Kidsy cuenta con 1 sensor de color y un Neopixel:
// - ColorSensor
// - Neopixel
//
// Si aun no hay explorado estos dos elementos por separado, te invitamos a probar los ejemplos:
// ColorSensorSimple y NeopixelFunctions.
//
// Este sensor es capaz de leer colores basicos y su cantidad de color blanco, por lo que puede diferenciar
// entre 8 colores diferentes: Negro, Rojo, Verde, Azul, Amarillo, Cian, Magenta y Blanco.
//
// La tabla de los colores, alias y nombres se detalla a continuacion:
//
//  -----------------------------
// | NUMERO | ALIAS   | NOMBRE   |
// | ----------------------------|
// | 0      | BLACK   | Negro    |
// | 1      | RED     | Rojo     |
// | 2      | GREEN   | Verde    |
// | 3      | BLUE    | Azul     |
// | 4      | YELLOW  | Amarillo |
// | 5      | CYAN    | Cian     |
// | 6      | MAGENTA | Magenta  |    
// | 7      | WHITE   | Blanco   |
//  -----------------------------
//
// Este mismo esquema de colores lo maneja el Neopixel, por lo que son compatibles. Para mostrar el color que
// esta viendo Robbus Kidsy en el Neopixel basta con:
//
// 1. Leer el color con la funcion ColorSensor.read();
// 2. Cargar ese color con la funcion Neopixel.color(ColorSensor.value, brightness);
//
// En resumen, a una funcion del Neopixel, se le carga el valor de una funcion del sensor y el brillo (1 - 255).
// --------------------------------------------------------------------------------------------------------------

#include<RobbusKidsy.h>

RobbusKidsy Kidsy;              // Llama a Robbus Kidsy

void setup() {
  Serial.begin(115200);         // Inicializa comunicacion serial a 115200 baudios
  Kidsy.begin();                // Inicializa el hardware del Robbus Kidsy
  Kidsy.ColorSensor.enable();   // Habilita el sensor de color y el LED blanco
}

void loop() {
  Kidsy.ColorSensor.read();                           // Lee el color
  Serial.println(Kidsy.ColorSensor.name);             // Imprime el color

  Kidsy.Neopixel.color(Kidsy.ColorSensor.value, 50);      // Carga el color del sensor en el neopixel con un brillo de 50 (max 255)
}
