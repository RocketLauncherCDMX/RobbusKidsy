// ------------------------------------------------ ROBBUS KIDSY ----------------------------------------------
//
// EJEMPLO DE USO DE LOS BOTONES DE USUARIO A, B y C CON PULSACION LARGA Y MANTENIDA PARA MANEJAR LOS MOTORES
// Este ejemplo es de uso libre y esta pensado para dar una introduccion al hardware de Robbus Kidsy.
// Autor: Rocket Launcher
// Fecha: 12 de febrero de 2020
// ------------------------------------------------------------------------------------------------------------
//
// Robbus Kidsy cuenta con 3 Botones de usuario:
// - ButtonA
// - ButtonB
// - ButtonC
// Nota importante: El boton RESET sirve para reiniciar el programa de tu Robbus Kidsy y no se puede programar
//
// Con este ejemplo entenderas los diferentes usos que se le pueden dar a los botones de tu Robot.
// Los botones de usuario A, B y C pueden regresar 4 estados diferentes:
//
// 0 - RELEASED       - El boton paso de estar presionado a liberarse
// 1 - PRESSED        - El boton paso de estar en en reposo a presionarse
// 2 - HOLD_PRESSED   - Se mantiene presionado
// 3 - HOLD_RELEASED  - Se mantiene sin preisonar
//
// En este ejemplo se utilizan los 4 estados para crear una funcionalidad extra de cada boton que ya conoces
// de sobra, que es emular el comportamiento de una tecla de tu teclado de computadora, la cual funciona en 
// cuanto la presionas, y si la mantienes presionada aproximadamente 1 segundo, comienza a repetir la accion 
// unas 20 veces por segundo, es decir cada 50ms o 20Hz. concretamente, cuando presiones el boton A, la
// velocidad de las llantas incrementara positivamente (giro hacia adelante) 1 unidad, luego lo hara de 5 en 5,
// de igual forma lo hara el boton C de forma negativa (giro hacia atras). Es importante destacar que la
// velocidad maxima de las llantas es de 255, siendo este el valor maximo de un byte (8 bits).
// El botón B devolvera el valor de la velocidad a 0 de forma suave. En resumen:
//
// A - Incrementa velocidad hacia al frente
// B - Detiene gradualmente los motores
// C - Incrementa velocidad hacia atras
//
// Tu puedes usar el numero asignado del 0 al 3 o la etiqueta de igual forma. Los estados 0 y 1  
// (PRESSED y RELEASED) sirven para ejecutar acciones de 1 sola vez, por ejemplo, imprimir que el boton
// se presiono o se libero, mientras que los estados 2 y 3 (HOLD_PRESSED y HOLD_RELEASED) sirven para
// repetir una accion de forma indefinida, por ejemplo, mostrar el estado de una variable mientras el boton
// se encuentre presionado. Los 4 estados son muy funcionales, Usalos sabiamente.
// ------------------------------------------------------------------------------------------------------------

#include<RobbusKidsy.h>

RobbusKidsy Kidsy;        // Llama a tu Robbus Kidsy

void setup() {
  Serial.begin(115200);   // Inicializa la comunicacion serial por USB a 115200 baudios
  Kidsy.begin();          // Inicializa el hardware de Robbus Kidsy
}

int speed = 0;            // guarda la velocidad de los motores

// banderas - Son variables booleanas que guardan estados verdaderos o falsos
bool flag_firstHold_A = false;    // primera vez que se mantiene el boton A
bool flag_repeat_A = false;       // activa la repeticion de la accion al presionar el boton A
bool flag_firstHold_B = false;    // primera vez que se mantiene el boton B
bool flag_repeat_B = false;       // activa la repeticion de la accion al presionar el boton B
bool flag_firstHold_C = false;    // primera vez que se mantiene el boton B
bool flag_repeat_C = false;       // activa la repeticion de la accion al presionar el boton B
bool flag_stopping = false;       // permite detener los motores poco a poco

long timestamp;   // guarda el valor de la funcion millis() que cuenta el tiempo de ejecucion del programa

void loop() {
  // Lee los estados de los botones A y B, estos pueden ser 4:
  // 0 - RELEASED       - El boton paso de estar presionado a liberarse
  // 1 - PRESSED        - El boton paso de estar en en reposo a presionarse
  // 2 - HOLD_PRESSED   - Se mantiene presionado
  // 3 - HOLD_RELEASED  - Se mantiene sin preisonar
  Kidsy.ButtonA.read();
  Kidsy.ButtonB.read();
  Kidsy.ButtonC.read();

  // REVISION DEL BOTON A
  // --------------------------------------------------------------------------------------------------------------------
  if(Kidsy.ButtonA.status == PRESSED) {
    speed--;                // si se presiona el boton B, la velocidad aumenta en 1
    flag_stopping = false;  // si la bandera de detener estaba activa, la desactiva
  }
  // Si el boton se mantiene presionado, y es la primera vez ...
  else if(Kidsy.ButtonA.status == HOLD_PRESSED && flag_firstHold_A == false) {
    flag_firstHold_A = true;    // ... activa bandera de boton presionado
    timestamp = millis();       // guarda el valor actual del tiempo en ms
  }
  // Si la bandera de boton presionado ya esta activa, pero aun no ha pasado el tiempo para la repeticion ...
  else if(Kidsy.ButtonA.status == HOLD_PRESSED && flag_firstHold_A == true && flag_repeat_A == false) {
    if(millis() > timestamp + 1000) flag_repeat_A = true; // ... revisa si se supero el tiempo para activar repeticion
  }
  
  if(Kidsy.ButtonA.status == RELEASED) {  // si el boton se solto
    flag_firstHold_A = false;   // desactiva bandera de boton presionado por primera vez
    flag_repeat_A = false;      // desactiva bandera de repeticion
  }

  if(flag_repeat_A == true) {   // si la bandera de repeticion esta activa
    speed -= 5;            // hace incrementos rapidos de 5
    delay(100);
  }
  // --------------------------------------------------------------------------------------------------------------------

  // REVISION DEL BOTON B
  // --------------------------------------------------------------------------------------------------------------------
  if(Kidsy.ButtonB.status == PRESSED) {
    if(speed != 0) flag_stopping = true;    // Si los motores no estan detenidos, activa bandera para hacerlo
  }
  
  if(Kidsy.ButtonB.status == RELEASED) {  
  }
  // --------------------------------------------------------------------------------------------------------------------

  // REVISION DEL BOTON C
  // --------------------------------------------------------------------------------------------------------------------
  if(Kidsy.ButtonC.status == PRESSED) {
    speed++;                // si se presiona el boton C, la velocidad aumenta en 1
    flag_stopping = false;  // si la bandera de detener estaba activa, la desactiva
  }
  // Si el boton se mantiene presionado, y es la primera vez ...
  else if(Kidsy.ButtonC.status == HOLD_PRESSED && flag_firstHold_C == false) {
    flag_firstHold_C = true;    // ... activa bandera de boton presionado
    timestamp = millis();       // guarda el valor actual del tiempo en ms
  }
  // Si la bandera de boton presionado ya esta activa, pero aun no ha pasado el tiempo para la repeticion ...
  else if(Kidsy.ButtonC.status == HOLD_PRESSED && flag_firstHold_C == true && flag_repeat_C == false) {
    if(millis() > timestamp + 1000) flag_repeat_C = true; // ... revisa si se supero el tiempo para activar repeticion
  }
  
  if(Kidsy.ButtonC.status == RELEASED) {  // si el boton se solto
    flag_firstHold_C = false;   // desactiva bandera de boton presionado por primera vez
    flag_repeat_C = false;      // desactiva bandera de repeticion
  }

  if(flag_repeat_C == true) {   // si la bandera de repeticion esta activa
    speed += 5;                 // hace incrementos rapidos de 5
    delay(100);
  }
  // --------------------------------------------------------------------------------------------------------------------

  if(speed > 255) speed = 255;  // si la velocidad supera los 8 bits (+-255), se mantiene
  if(speed < -255) speed = -255;

  if(flag_stopping == true) {   // si la bandera para detener la llantas se activo...
    if(speed < 0) speed++;      // y la velocidad es mayor a 0 (hacia el frente), la disminuye
    else if(speed > 0) speed--; // si es menor a 0, (hacia atras), la aumenta
    else flag_stopping = false; // pero si es 0 desactiva la bandera (ya estan detenidas)
    delay(10);                  // espera en cada ciclo de reduccion de velocidad
  }

  // Asigna la velocidad en los motores, si se quiere cambiar el sentido hacia atras, usar BACK
  Kidsy.Move.MotorLeft(speed);    
  Kidsy.Move.MotorRight(speed);

  Serial.print("velocidad: ");    // imprime la velocidad de ambos motores
  Serial.println(speed);
} 