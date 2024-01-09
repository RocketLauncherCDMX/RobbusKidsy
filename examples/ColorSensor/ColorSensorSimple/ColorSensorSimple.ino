// ------------------------------------------------ ROBBUS KIDSY ------------------------------------------------
//
// EJEMPLO DE LECTURA DEL SENSOR DE COLOR
// Este ejemplo es de uso libre y esta pensado para dar una introduccion al hardware de Robbus Kidsy.
// Autor: Rocket Launcher
// Fecha: 8 de febrero de 2020
// Visita www.robbus.mx/kidsy para mas informacion
// --------------------------------------------------------------------------------------------------------------
//
// Robbus Kidsy cuenta con 1 sensor de color:
// - ColorSensor
//
// Este sensor es capaz de leer colores basicos y su cantidad de color blanco, por lo que puede diferenciar
// entre 8 colores diferentes: Negro, Rojo, Verde, Azul, Amarillo, Cian, Magenta y Blanco.
//
// Cada uno de estos colores puede ser leido de forma numerica, por medio de un "Alias" o por su nombre.
// Para leer un color, primero es necesario que se habilite el sensor de color. Los diferentes modulos
// de Robbus Kidsy se configuran y habilitan al llamar a la funcion Kidsy.begin(), a excepcion del sensor
// de color.
// La vista humana y de diversos animales, se basa en la cantidad de luz que llega a los ojos, lo mismo
// sucede con Robbus Kidsy. Para que pueder ver un color, se requiere de luz, por lo que a un lado del sensor
// veras un pequeño LED. Para que este LED no gaste bateria si no estas usando el sensor de color,
// implementamos una funcion de habilitacion llamada enable(), y otra de dehabilitacion llamada disable().
//
// Kidsy.ColorSensor.enable();    // Habilita el sensor y el LED
// Kidsy.ColorSensor.disable();   // Deshabilita el sensor y el LED
//
// Posteriormente basta con llamar la funcion read() para leer el color que Robbus esta viendo y revisar la
// variable value para revisar su valor numerico, o imprimir su nombre leyendo la variable name, por ejemplo:
//
// Kidsy.ColorSensor.read();              // Lee el color
// if(Kidsy.ColorSensor.value == RED) {}  // Evalua si el color es Rojo
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
// Ademas de los colores, Robbus puede medir la cantidad de luz blanca que esta viendo, usando la variable white
//
// Serial.println(Kidsy.ColorSensor.white);     // Imprime la cantidad de luz blanca
// --------------------------------------------------------------------------------------------------------------

#include<RobbusKidsy.h>
RobbusKidsy Kidsy;                // Llama a Robbus Kidsy

void setup() {
  Serial.begin(115200);           // Inicializa comunicacion serial a 115200 baudios
  Kidsy.begin();                  // Inicializa el hardware del Robbus Kidsy
  Kidsy.ColorSensor.enable();     // Habilita el sensor de color, enciende el LED blanco, y carga calibracion
  //Kidsy.ColorSensor.disable();    // Si quieres deshabilitar el sensor y apagar el LED blanco, usa esta funcion

  // MUY IMPORTANTE:
  // SI EL SENSOR DE COLOR NO DA MEDICIONES CORRECTAS, DEBERAS RECALIBRARLO. PUEDES USAR EL EJEMPLO
  // colorSensorCalibration DENTRO DE LA CARPETA ColorSensor PARA SABER COMO HACERLO, MIRAR EL MANUAL DE USUARIO,
  // O VISITAR LA PAGINA www.robbus.mx/kidsy
}

unsigned int red;             // componente de rojo
unsigned int green;           // componente de verde
unsigned int blue;            // componente de azul
unsigned int white;           // componente de blanco
unsigned int color_sense;     // Valor del color leido (0 - 7)


void loop() {
  // Lee cada componente del color por separado.
  red = Kidsy.ColorSensor.getRed();
  green = Kidsy.ColorSensor.getGreen();
  blue = Kidsy.ColorSensor.getBlue();
  white = Kidsy.ColorSensor.getWhite();

  // Imprime los componentes de cada color.
  Serial.print("R:");
  Serial.print(red);
  Serial.print(" G:");
  Serial.print(green);
  Serial.print(" B:");
  Serial.print(blue);
  Serial.print(", W:");
  Serial.print(white);
  Serial.print("\t");

  // Al usar la funcion read, primero lee los valores de los componentes rojo, verde, azul y blanco,
  // por lo que no es necesario usar los componentes para obtener el color que esta sensando.
  color_sense = Kidsy.ColorSensor.read();
  Serial.print("Color: ");
  Serial.print(color_sense);
  Serial.print(" - ");

  // Para imprimir el nombre del color, se usa la propiedad name de ColorSensor 
  Serial.println(Kidsy.ColorSensor.name);
  
  delay(10);
}