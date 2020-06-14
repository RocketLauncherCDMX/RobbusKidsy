// ------------------------------------------------ ROB.BIT KIDSY -----------------------------------------------
//
// Nivel de dificultad: FACIL
//
// EJEMPLO DE USO DEL NEOPIXEL EN CONJUNTO CON EL SENSOR DE COLOR
// Este ejemplo es de uso libre y esta pensado para dar una introduccion al hardware del robot Rob.bit Kidsy.
// Autor: Fabian Alfonso Flores
// Empresa: Proyectil Mx
// Fecha: 8 de febrero de 2020
// --------------------------------------------------------------------------------------------------------------
//
// Rob.bit Kidsy cuenta con 1 sensor de color y un Neopixel:
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
//  ----------------------------
// | NUMERO | ALIAS   | NOMBRE  |
// | ---------------------------|
// | 0     | BLACK   | Negro    |
// | 1     | RED     | rojo     |
// | 2     | GREEN   | verde    |
// | 3     | BLUE    | azul     |
// | 4     | YELLOW  | amarillo |
// | 5     | CYAN    | cian     |
// | 6     | MAGENTA | magenta  |    
// | 7     | WHITE   | blanco   |
//  ----------------------------
//
// Este mismo esquema de colores lo maneja el Neopixel, por lo que son compatibles. Para mostrar el color que
// esta viendo Rob.bit Kidsy en el Neopixel basta con:
//
// 1. Leer el color con la funcion ColorSensor.read();
// 2. Cargar ese color con la funcion:
//    Kidsy.Neopixel.color(Kidsy.ColorSensor.value);
//
// Puede parecer complejo, pero en relidad es muy sencillo, a una funcion del Neopixel, se le carga el valor de
// una funcion del sensor.
// --------------------------------------------------------------------------------------------------------------

#include<Rob_bit_Kidsy.h>

Rob_bit Kidsy;    // Declara una instancia tipo Rob.bit, llamada de igual forma

bool buzzer_state = false;    // Activa o desactiva el buzzer
int new_color, old_color;     // identifica si el color ha cambiado

void setup() {
  Serial.begin(115200);       // Inicializa comunicacion serial a 115200 baudios
  Kidsy.begin();              // Inicializa el hardware del Rob.bit Kidsy
  Kidsy.ColorSensor.enable(); // Habilita el sensor de color y el LED blanco
}

void loop() {
  // Los colores que regresa la variable color_value de la clase ColorSensor pueden ser:
  // 0 - No identificable
  // 1 - ROJO
  // 2 - VERDE
  // 3 - AZUL
  // 4 - AMARILLO
  // 5 - CYAN
  // 6 - MAGENTA
  // 7 - BLANCO
  Kidsy.ColorSensor.read();               // Obtiene la medicion del sensor de color
  Serial.println(Kidsy.ColorSensor.name); // Imprime el nombre del color mas cercano

  // Imprime el valor individual de cada componente del color (RGB)
  /*Serial.print("Red: ");
  Serial.print(Kidsy.ColorSensor.red);
  Serial.print(", Green: ");
  Serial.print(Kidsy.ColorSensor.green);
  Serial.print(", Blue: ");
  Serial.println(Kidsy.ColorSensor.blue);*/

  new_color = Kidsy.ColorSensor.value;    // el color leido es aproximado a los primarios o secundarios (RGBCMY)

  if(Kidsy.ButtonA.read() == PRESSED) buzzer_state = !buzzer_state;   // El boton A activa o desactiva el Buzzer
  
  if(new_color != old_color) {                        // si el color cambio ...
    Kidsy.Neopixel.color(Kidsy.ColorSensor.value);    // ... muestra el color en el neopixel
    if(buzzer_state == true) {                        // si el buzzer esta habilitado ...
      switch(Kidsy.ColorSensor.value) {               // ... evalua que color se leyo
        case RED:     
          Kidsy.Buzzer.playTone(100);   // reproduce un sonido a una determinada frecuencia, indefinidamente
          break;    
        case GREEN:   
          Kidsy.Buzzer.playTone(200); 
          break;    
        case BLUE:    
          Kidsy.Buzzer.playTone(300); 
          break; 
        case YELLOW:  
          Kidsy.Buzzer.playTone(400); 
          break;
        case CYAN:    
          Kidsy.Buzzer.playTone(500); 
          break;
        case MAGENTA: 
          Kidsy.Buzzer.playTone(600); 
          break;
      }
    }
    else Kidsy.Buzzer.noTone();   // noTone detiene la reproduccion indefinida de playTone(Frequency);
  }  
  old_color = new_color;          // el nuevo color leido se convierte en el viejo color en la siguiente vuelta al loop
}