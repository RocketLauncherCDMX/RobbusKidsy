// ------------------------------------------------ ROBBUS KIDSY ------------------------------------------------
//
// RECALIBRACION DEL SENSOR DE COLOR
// Este ejemplo es de uso libre y esta pensado para dar una introduccion al hardware de Robbus Kidsy.
// Autor: Rocket Launcher
// Fecha: 31 de Agosto de 2021
// Visita www.robbus.mx/kidsy para mas informacion
// --------------------------------------------------------------------------------------------------------------
//
// Robbus Kidsy cuenta con 1 sensor de color y un Neopixel:
// - ColorSensor
// - Neopixel
//
// Si aun no has explorado estos dos elementos por separado, te invitamos a probar los ejemplos:
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
//
// --------------------------------------------------------------------------------------------------------------
// Este programa permite visualizar el color que Robbus Kidsy esta viendo, por medio de su Neopixel.
// No siempre el sensor de color toma los valores correctos de los colores, esto puede deverse a que tus colores
// difieren ligeramente de los que se usaron al calibrarlo, no obstante, tu puedes recalibralo.
// El procedimiento para hacerlo es el siguiente:
//
// 1. Si has cambiado el programa de Robbus Kidsy, asegurate que las funciones Kidsy.begin() y 
//    Kidsy.ColorSensor.enable() esten en tu programa.
// 2. Prende tu Kidsy, si no enciende, puede deberse a falta de bateria, recuerda recargarlo.
// 3. Configura el puerto Serial a 115200 Baudios. Para la calibracion, no es necesario que este conectado por
//    USB, pero te sera mas sencillo seguir las instrucciones por Serial.
// 4. Manten presionado el boton A mientras presionas y sueltas el boton RESET.
// 5. Sabras que has entrado al modo de calibracion del sensor de color por que parpadeara el Neopixel en rojo.
// 6. Si estas conectado por USB, sigue los pasos que se te indican, de lo contrario continua leyendo.
// 7. El Neopixel se quedara encendido en un color, deberas colocar a Kidsy encima del color correspondiente
//    Cuidando que el color quede exactamente debajo del Neopixel.
// 8. Presiona de nuevo el boton A y se encendera el siguiente color, continua posicionando y presionando A.
// 9. El ciclo para calibrar los colores es: Rojo, Verde, Azul, Amarillo, Cyan, Magenta, Blanco y Negro.
// 10. Una vez que termines, el Neopixel parpadeara en verde y la calibracion habra concluido.
// 
// Cuando completes la calibracion, los valores se quedaran guardados en la memoria Flash del microcontrolador,
// por lo que aun despues de apagarlo y volverlo a encender, Kidsy recordara esos mismos valores.
//
// Nota: El color negro se mostrara en el Neopixel como apagado.
//
// --------------------------------------------------------------------------------------------------------------

#include<RobbusKidsy.h>

RobbusKidsy Kidsy;              // Llama a Robbus Kidsy

void setup() {
  Serial.begin(115200);         // Inicializa comunicacion serial a 115200 baudios
  Kidsy.begin();                // Inicializa el hardware del Robbus Kidsy
  Kidsy.ColorSensor.enable();   // Habilita el sensor de color y el LED blanco
}

void loop() {  
  Kidsy.ColorSensor.read();                               // Lee el color
  Serial.println(Kidsy.ColorSensor.name);                 // Imprime el color
  Kidsy.Neopixel.color(Kidsy.ColorSensor.value, 50);      // Carga el color del sensor en el neopixel con un brillo de 50 (max 255)
}