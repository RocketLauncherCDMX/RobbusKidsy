// ------------------------------------------------ ROBBUS KIDSY ------------------------------------------------
//
// EJEMPLO DE USO DE DETECCION DE COLORES PARA MODIFICAR EL COMPORTAMIENTO DE ROBBUS KIDSY
// Este ejemplo es de uso libre y esta pensado para dar una introduccion al hardware de Robbus Kidsy.
// Autor: Rocket Launcher
// Fecha: 12 de diciembre de 2021
// Para mas informacion, visita la pagina www.robbus.mx/kidsy
// --------------------------------------------------------------------------------------------------------------
//
// Este es un divertido ejemplo donde podras darle una funcionalidad real a cada color para que tu robot
// reaccione de diferente manera.
//
// Recordemos los colores que Kidsy puede ver:
//
//  -----------------------------
// | NUMERO | ALIAS   | NOMBRE   |
// | ----------------------------|
// | 0      | BLACK   | Negro    |
// | 1      | RED     | rojo     |
// | 2      | GREEN   | verde    |
// | 3      | BLUE    | azul     |
// | 4      | YELLOW  | amarillo |
// | 5      | CYAN    | cian     |
// | 6      | MAGENTA | magenta  |    
// | 7      | WHITE   | blanco   |
//  -----------------------------
//
// Puedes saber que color esta viendo Kidsy con la funcion:
// Kidsy.ColorSensor.read();
//
// Y el valor se guardara en la variable:
// Kidsy.ColorSensor.value;
//
// Por lo tanto, leyendo esta variable podremos usar un swich (ver ejemplo switch en la carpeta sentencias
// condicionales) para determinar que color esta viendo y asignarle una funcion distinta.
//
// Ejemplo:
// switch(Kidsy.ColorSensor.value) {
//   case RED:
//     Kidsy.Move.forward(100);
//     break;
// }
//
// El ejemplo anterior movera las llantas al frente al detectar el color ROJO.
// Con el Boton A, detendras o accionaras el movimiento al frente de Kidsy, claro que todo lo puedes cambiar
// para hacer tu propia funcionalidad. ¡No tengas miedo de probar! :)
// 
// --------------------------------------------------------------------------------------------------------------

#include<RobbusKidsy.h>

RobbusKidsy Kidsy;              // Llama a Robbus Kidsy

int pauseTime = 500;            // Tiempo que Kidsy ignorara los colores detectados despues de la primera deteccion
short flag_moving = false;      // Variable para activar/desactivar el movimiento con un boton
int forward_speed = 100;        // Velocidad de movimiento de las llantas llendo recto (1 - 255)
int turn_speed = 100;           // Velocidad de movimiento de las llantas al dar vuelta (1 - 255)
int delay_turn_left = 210;      // Pausa al dar vuelta a la izquierda (en milisegundos)
int delay_turn_right = 195;     // Pausa al dar vuelta a la derecha (en milisegundos)
double timeStamp;               // Variable para guardar el tiempo de ejecucion del programa (se usa para hacer
                                // pausas sin usar la funcion delay(); esto nos permitira detectar un color y
                                // hacer una pausa sin bloquear el programa para que no detecte 2 veces el mismo
                                // color. Esto solamente funciona si Kidsy ve un color y antes de medio segundo
                                // ya salio de el, es decir, con pequeños parches de color de unos 5x5cm. Si tu
                                // parche de color es mas grande, prueba diferentes valores de pauseTime.

void setup() {
  Serial.begin(115200);         // Inicializa comunicacion serial a 115200 baudios
  Kidsy.begin();                // Inicializa el hardware del Robbus Kidsy
  Kidsy.ColorSensor.enable();   // Habilita el sensor de color y el LED blanco
}

void loop() {
  if(Kidsy.ButtonA.read() == PRESSED) {                     // Si se presiona el Boton A
    if(flag_moving == false) Kidsy.Buzzer.playTone(500,50); // si la bandera es falsa, toca un tono
    else Kidsy.Buzzer.playTone(300,50);                     // si la bandera es verdadera, toca otro tono
    flag_moving = !flag_moving;                             // cambia el estado de la bandera
  }

  if(flag_moving == true) {               // Si la bandera es verdadera
    Kidsy.Move.forward(forward_speed);    // Mueve a Kidsy al frente por defecto
  }
  else Kidsy.Move.stop();                 // Si la bandera es falsa, no mueve a Kidsy
  
  Kidsy.ColorSensor.read();                   // Lee el color
  //Serial.println(Kidsy.ColorSensor.name);   // descomenta para imprimir el color

  Kidsy.Neopixel.color(Kidsy.ColorSensor.value, 50);  // Carga el color del sensor en el neopixel con un brillo de 50 (max 255)

  // Si la bandera de movimiento es verdadera y ya paso medio segundo desde que se guardo el valor del tiempo
  if(flag_moving == true && millis() > timeStamp + pauseTime) {
    switch(Kidsy.ColorSensor.value) {       // Evalua el color que percibio Kidsy
      case RED:                             // ROJO
        Kidsy.Move.turnRight(turn_speed);   // Vuelta a la derecha
        Kidsy.Buzzer.playTone(550,25);      // Tono correspondiente
        delay(delay_turn_right);            // pausa correspondiente
        timeStamp = millis();               // Guarda el valor del tiempo para la siguiente deteccion
        break;
      case GREEN:                           // Misma logica para VERDE, diferente funcionalidad
        Kidsy.Move.turnRight(255);
        Kidsy.Buzzer.playTone(650,50);
        delay(50);
        Kidsy.Buzzer.playTone(650,50);
        delay(50);
        Kidsy.Buzzer.playTone(650,50);
        delay(50);
        Kidsy.Buzzer.playTone(650,100);
        delay(500);
        Kidsy.Move.turnLeft(255);
        delay(500);
        flag_moving = false;
        break;
      case YELLOW:                          // Color AMARILLO
        Kidsy.Move.stop();
        Kidsy.Buzzer.playTone(2000,100);
        delay(500);
        Kidsy.Move.forward(forward_speed);        
        timeStamp = millis();
        break;
      case BLUE:                            // Color AZUL
        Kidsy.Move.turnLeft(turn_speed);
        Kidsy.Buzzer.playTone(700,25);
        delay(delay_turn_left);
        Kidsy.Move.stop();
        timeStamp = millis();
        break;

        // Si lo deseas puedes añadir los demas colores con diferentes funciones.
        // No olvides añadir timeStamp = millis(); al final para la pausa entre detecciones.
    }
  }
}