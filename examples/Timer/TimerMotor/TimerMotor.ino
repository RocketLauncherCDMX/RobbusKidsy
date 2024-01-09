// ------------------------------------------------ ROBBUS KIDSY ----------------------------------------------
//
// EJEMPLO DE USO DE LOS TIMERS POR HARDWARE
// Este ejemplo es de uso libre y esta pensado para dar una introduccion al hardware de Robbus Kidsy.
// Autor: Rocket Launcher
// Fecha: 03 de marzo de 2020
// ------------------------------------------------------------------------------------------------------------
// Importante: Este ejemplo es avanzado, recomendable solamente si ya sabes programar en C y si tienes nociones
// del hardware interno del ESP32.

// Robbus Kidsy cuenta con 4 timers:
// Timer0
// Timer1
// Timer2
// Timer3
//
// Para mas informacion sobre el Hardware, puedes consultar la hoja de datos WROOM32.
// ------------------------------------------------------------------------------------------------------------
//
// En este ejemplo se utilizara el Timer 0 para hacer una temporizacion de 50ms, dentro de la cual, cargaremos
// la velocidad de las llantas, pudiendo de esta forma incrementar o decrementar su valor de forma suave y sin
// robarle procesamiento al loop principal. Por ejemplo, si la velocidad actual es 0, y nosotros le asignamos 100,
// cada 50ms la velocidad se incrementara en 1 hasta llegar al 100. Esta forma tambien nos permite utilizar la
// instruccion delay sin que el loop se bloquee.

#include<RobbusKidsy.h>

RobbusKidsy Kidsy;            // Llama a Robbus Kidsy

#define MAX_SPEED     100     // Velocidad maxima de los motores
#define TIMER_TIME    50000   // Tiempo del Timer0 en microSegundos (50 ms)

// Declaramos un puntero tipo timer, compatible con ESP32
hw_timer_t * timer = NULL;
// Variable de sincronizacion para modificar una variable compartida
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

// Variables para la velocidad de los motores
// Los valores pueden ser:
// POSITIVO   1 a  255  ->   1 es la velocidad minima,  255 la maxima, hacia ADELANTE.
// NEGATIVO  -1 a -255  ->  -1 es la velocidad minima, -255 la maxima, hacia ATRAS.
//            0 es totalmente detenido.
// Una velocidad muy baja (menor a +-25) puede que no sea suficiente para comenzar a mover el motor

int motorSpeedLeft = 0;      // 100 hacia el FRENTE
int motorSpeedRight = 0;     // 100 hacia ATRAS
int setSpeedLeft = 0;
int setSpeedRight = 0;

// Funcion de interrupcion del timer
void IRAM_ATTR onTimer() {
  // Para la variable compartida por loop y la funcion de interrupcion
  portENTER_CRITICAL_ISR(&timerMux);  

  // En esta seccion se evalua la velocidad actual de los motores y se ajustan cada el timer interrumpe
  if(motorSpeedLeft < setSpeedLeft)         motorSpeedLeft++;
  else if(motorSpeedLeft > setSpeedLeft)    motorSpeedLeft--;
  if(motorSpeedRight < setSpeedRight)       motorSpeedRight++;
  else if(motorSpeedRight > setSpeedRight)  motorSpeedRight--;

  // carga los nuevos valores de velocidad en los motores
  Kidsy.Move.MotorLeft(motorSpeedLeft);
  Kidsy.Move.MotorRight(motorSpeedRight);

  // Para la variable compartida por loop y la funcion de interrupcion
  portEXIT_CRITICAL_ISR(&timerMux);   
}

void setup() {
  Serial.begin(115200);   // Inicializa la comunicacion serial por USB a 115200 baudios
  Kidsy.begin();          // Inicializa el hardware del Robbus Kidsy

  // A continucacion configuramos el timer:
  
  // 0       - Inicializa el Timer0, Robbus Kidsy dispone de 4 timers (0 al 3)
  // 80      - Valor del preescaler para 80MHz / 80 = 1000000 veces por segundo
  // true    - Cuenta ascendente, false: cuenta descendente
  timer = timerBegin(0, 80, true);
  
  // timer    - Pasamos la variable timer creada
  // &onTimer - funcion onTimer
  // true     - la interrupcion generada sera ascendente, false: descendente
  timerAttachInterrupt(timer, &onTimer, true);  
  
  // timer   - Puntero al timer
  // 5000    - Valor del contador en el cual el timer interrumpira (ms para un valor de preescaler de 80 y velocidad de 80MHz)
  // true    - Interrupcion periodica, false: interrumpe una sola vez 
  timerAlarmWrite(timer, 5000, true);

  //  Habilitamos el timer que acabamos de crear
  timerAlarmEnable(timer);               
}

void loop() {
  Kidsy.ButtonA.read();                       // captura estado nuevo del boton A
  Kidsy.ButtonB.read();                       // captura estado nuevo del boton B

  Kidsy.ArrowForward.read();                  // Captura el valor digital de la flecha arriba
  Kidsy.ArrowBackward.read();                 // Captura el valor digital de la flecha abajo
  Kidsy.ArrowLeft.read();                     // Captura el valor digital de la flecha izquierda
  Kidsy.ArrowRight.read();                    // Captura el valor digital de la flecha derecha

  if(Kidsy.ArrowForward.status == TOUCHED) {  // Si se toca flecha adelante...
    setSpeedLeft = MAX_SPEED;                 // motor izquierdo al maximo adelante
    setSpeedRight = MAX_SPEED;                // motor derecho al maximo adelante
    Kidsy.Neopixel.color(RED);                // Neopixel en rojo
    Kidsy.Buzzer.playTone(400,50);            // Tono a 400Hz por 50ms
  }

  if(Kidsy.ArrowBackward.status == TOUCHED) { // Si se toca flecha atras...
    setSpeedLeft = -MAX_SPEED;                // motor izquierdo al maximo atras
    setSpeedRight = -MAX_SPEED;               // motor derecho al maximo atras
    Kidsy.Neopixel.color(GREEN);              // Neopixel en verde
    Kidsy.Buzzer.playTone(500,50);            // Tono a 500Hz por 50ms
  }

  if(Kidsy.ArrowLeft.status == TOUCHED) {     // Si se toca flecha izquierda...
    setSpeedLeft = -MAX_SPEED;                // motor izquierdo al maximo atras
    setSpeedRight = MAX_SPEED;                // motor derecho al maximo adelante
    Kidsy.Neopixel.color(BLUE);               // Neopixel en azul
    Kidsy.Buzzer.playTone(600,50);            // Tono a 600Hz por 50ms
  }

  if(Kidsy.ArrowRight.status == TOUCHED) {    // Si se toca flecha derecha...
    setSpeedLeft = MAX_SPEED;                 // motor izquierdo al maximo adelante
    setSpeedRight = -MAX_SPEED;               // motor derecho al maximo atras
    Kidsy.Neopixel.color(MAGENTA);            // Neopixel en magenta
    Kidsy.Buzzer.playTone(700,50);            // Tono a 700Hz por 50ms
  }
  
  // REVISION DEL BOTON A
  // ----------------------
  if(Kidsy.ButtonA.status == PRESSED) {       // Si se presiona Boton A
    setSpeedLeft = 0;                         // Detiene ambos motores
    setSpeedRight = 0;
    Kidsy.Neopixel.color(BLACK);              // Apaga el Neopixel
  }
  else if(Kidsy.ButtonA.status == RELEASED) {
    // Coloca lo que quieres que ejecute al soltar el boton
  }
  
  // REVISION DEL BOTON B
  // ----------------------
  if(Kidsy.ButtonB.status == PRESSED) { 
    // Coloca lo que quieres que ejecute al presionar el boton
  }
  else if(Kidsy.ButtonB.status == RELEASED) { 
    // Coloca lo que quieres que ejecute al soltar el boton
  }

  // REVISION DEL BOTON C
  // ----------------------
  if(Kidsy.ButtonC.status == PRESSED) { 
    // Coloca lo que quieres que ejecute al presionar el boton
  }
  else if(Kidsy.ButtonC.status == RELEASED) { 
    // Coloca lo que quieres que ejecute al soltar el boton
  }

  Serial.print("MotorIzq = ");
  Serial.print(motorSpeedLeft); 
  Serial.print(", MotorDer = ");
  Serial.println(motorSpeedRight); 
} 