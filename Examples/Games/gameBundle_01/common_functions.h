// Estados de Kidsy
#define FLAT        0           // se usan en las llantas
#define OK          1
#define IDDLE       0
#define GAME_A      1
#define GAME_B      2
#define GAME_C      3
#define GAMEOVER    4

// Acciones de Kidsy en los 3 juegos
#define READY           5
#define INTERACT_SOUND  6
#define INTERACT_NEOP   7
#define INTERACT_BOTH   8
#define INCORRECT       9
#define CORRECT         10
#define VICTORY         11

signed int motorSpeedLeft;
signed int motorSpeedRight;
signed int setSpeedLeft = 0;
signed int setSpeedRight = 0;
int kidsyStatus = IDDLE;        // maneja en que estado del juego se encuentra
int arrowTouched = 0;           // guarda la flecha que se toco
double timeStamp;               // guarda una marca de tiempo

unsigned int timer_time = 50000; // Tiempo del Timer0 en microSegundos (50 ms)

// Declaramos un puntero tipo timer, compatible con ESP32
hw_timer_t * timer = NULL;
// Variable de sincronizacion para modificar una variable compartida
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

// Funcion de interrupcion del timer
void IRAM_ATTR onTimer() {
  // Para la variable compartida por loop y la funcion de interrupcion
  portENTER_CRITICAL_ISR(&timerMux);

  // En esta seccion se evalua la velocidad actual de los motores y se ajustan cada el timer interrumpe
  if(motorSpeedLeft < setSpeedLeft)         motorSpeedLeft+=acceleration;
  else if(motorSpeedLeft > setSpeedLeft)    motorSpeedLeft-=acceleration;
  if(motorSpeedRight < setSpeedRight)       motorSpeedRight+=acceleration;
  else if(motorSpeedRight > setSpeedRight)  motorSpeedRight-=acceleration;

  // carga los nuevos valores de velocidad en los motores
  Kidsy.Move.MotorLeft(motorSpeedLeft);
  Kidsy.Move.MotorRight(motorSpeedRight);

  // Para la variable compartida por loop y la funcion de interrupcion
  portEXIT_CRITICAL_ISR(&timerMux);   
}

void configureTimer() {
  // A continucacion configuramos el timer:
  
  // 1       - Inicializa el Timer1, Robbus Kidsy dispone de 4 timers (0 al 3)
  // 80      - Valor del preescaler para 80MHz / 80 = 1000000 veces por segundo
  // true    - Cuenta ascendente, false: cuenta descendente
  timer = timerBegin(1, 80, true);
  
  // timer    - Pasamos la variable timer creada
  // &onTimer - funcion onTimer
  // true     - la interrupcion generada sera ascendente, false: descendente
  timerAttachInterrupt(timer, &onTimer, true);  
  
  // timer      - Puntero al timer
  // timer_time - Valor del contador en el cual el timer interrumpira (ms para un valor de preescaler de 80 y velocidad de 80MHz)
  // true       - Interrupcion periodica, false: interrumpe una sola vez 
  timerAlarmWrite(timer, timer_time, true);

  //  Habilitamos el timer que acabamos de crear
  timerAlarmEnable(timer);
}

/* --------------------------------------------------------------------------------------------
 *  void manageLeds(bool l1, bool l2, bool l3, bool l4)
 * --------------------------------------------------------------------------------------------
 *  Funcion para manejo global de los LEDs de usuario.
 *  Por medio de esta funcion, podras controlar los 4 LEDs de forma conjunta.
 *  Por ejemplo, si quieres encender el LED1 y apagar el resto, puedes hacer lo siguiente:
 *  
 *  manageLeds(1,0,0,0);
 *  
 *  Como veras cada numero de la funcion representa un LED en orden ascendente, lo estados
 *  pueden ser:
 *  
 *  1 - enciende el LED
 *  0 - apaga el LED
 * -------------------------------------------------------------------------------------------- */
void manageLeds(bool l1, bool l2, bool l3, bool l4) {
  Kidsy.Led1.set(l1);
  Kidsy.Led2.set(l2);
  Kidsy.Led3.set(l3);
  Kidsy.Led4.set(l4);
}

/* --------------------------------------------------------------------------------------------
 *  void actions(int action)
 * --------------------------------------------------------------------------------------------
/*  Funcion para manejo de las acciones de Kidsy.
 *  Esta funcion se encarga de controlar acciones concretas de tu Kidsy, las cuales involucran
 *  diferente hardware, como el Buzzer, el Neopixel. Cabe destacar que los movimientos tienen
 *  su propia funcion.
 *  
 *  Por ejemplo, desde aqui puedes expresar cuando algo que hiciste fue correcto, por medio
 *  de la siguiente forma:
 *  
 *  actions(CORRECT);
 *  
 *  Las palabras que lleva la funcion pueden ser:
 *  
 *  READY
 *  INTERACT_SOUND
 *  INTERACT_NEOP
 *  INTERACT_BOTH
 *  INCORRECT
 *  CORRECT
 *  VICTORY
 *  
 *  Y cada accion genera diferentes sonidos, patrones de luz y movimiento.
 * -------------------------------------------------------------------------------------------- */
void actions(int action) {
  switch(action) {                        // Revisa que accion se quiere realizar
    case FORWARD:                         // accion: ADELANTE
      Serial.println("  ADELANTE");
      if(flag_sound == ON) Kidsy.Buzzer.playTone(1000, 50);    // Tono representativo: 1000Hz
      Kidsy.Neopixel.color(WHITE, color_intensity);        // Color representativo: BLANCO
      delay(50);
      Kidsy.Neopixel.off();               // apaga Neopixel
      break;
    case BACKWARD:
      Serial.println("  ATRAS");
      if(flag_sound == ON) Kidsy.Buzzer.playTone(1100, 50);    // Tono representativo: 1100Hz
      Kidsy.Neopixel.color(MAGENTA, color_intensity);      // Color representativo: MAGENTA
      delay(50);
      Kidsy.Neopixel.off();               // apaga Neopixel
      break;
    case LEFT:
      Serial.println("  IZQUIERDA");
      if(flag_sound == ON) Kidsy.Buzzer.playTone(1200, 50);    // Tono representativo: 1200Hz
      Kidsy.Neopixel.color(CYAN, color_intensity);         // Color representativo: CYAN
      delay(50);
      Kidsy.Neopixel.off();               // apaga Neopixel
      break;
    case RIGHT:
      Serial.println("  DERECHA");
      if(flag_sound == ON) Kidsy.Buzzer.playTone(1300, 50);    // Tono representativo: 1300Hz
      Kidsy.Neopixel.color(YELLOW, color_intensity);       // Color representativo: AMARILLO
      delay(50);
      Kidsy.Neopixel.off();               // apaga Neopixel
      break;
    case READY:                           // indica que kidsy esta listo para comenzar
      Serial.print(" 3 ...");
      if(flag_sound == ON) Kidsy.Buzzer.playTone(1000, 250);   // tonos sucesivos con colores de Neopixel 
      delay(500);
      Serial.print(" 2 ...");
      Kidsy.Neopixel.color(RED, color_intensity);
      if(flag_sound == ON) Kidsy.Buzzer.playTone(1000, 250);
      delay(500);
      Serial.print(" 1 ...");
      Kidsy.Neopixel.color(YELLOW, color_intensity);
      if(flag_sound == ON) Kidsy.Buzzer.playTone(1000, 250);
      delay(500);
      Serial.println(" ¡Comienza!");
      Kidsy.Neopixel.color(GREEN, color_intensity);
      if(flag_sound == ON) Kidsy.Buzzer.playTone(2000, 250);
      delay(1000);
      Kidsy.Neopixel.color(OFF);
      break;
    case INTERACT_SOUND:                  // indica una interaccion de usuario con sonido
      if(flag_sound == ON) Kidsy.Buzzer.playTone(1000, 50);
      break;
    case INTERACT_NEOP:                   // indica una interaccion de usuario con neopixel
      Kidsy.Neopixel.color(GREEN, color_intensity);
      delay(25);
      Kidsy.Neopixel.off();
      break;
    case INTERACT_BOTH:                   // indica una interaccion de usuario con sonido y neopixel
      if(flag_sound == ON) Kidsy.Buzzer.playTone(1000);
      Kidsy.Neopixel.color(GREEN, color_intensity);
      delay(25);
      Kidsy.Buzzer.noTone();
      Kidsy.Neopixel.off();
      break;
    case CORRECT:                         // indica que lo que se hizo fue correcto
      if(flag_sound == ON) Kidsy.Buzzer.playTone(2000, 25);    // tono de accion correcta
      delay(100);
      if(flag_sound == ON) Kidsy.Buzzer.playTone(2000, 25);
      delay(100);
      Kidsy.Neopixel.heartBeat(GREEN, color_intensity);    // color de Neopixel: VERDE
      break;
    case INCORRECT:                       // indica que lo que se hizo fue incorrecto
      if(flag_sound == ON) Kidsy.Buzzer.playTone(200);         // tono de accion incorrecta
      Kidsy.Neopixel.heartBeat(RED, color_intensity);      // color de neopixel: ROJO
      Kidsy.Buzzer.noTone();
      break;
    case VICTORY:                         // indica que se gano el juego en cuestion
      if(flag_sound == ON) {
        Kidsy.Buzzer.playTone(800, 100);    // tono compuesot de victoria
        delay(100);
        Kidsy.Buzzer.playTone(800, 100);
        delay(50);
        Kidsy.Buzzer.playTone(800, 100);
        delay(50);
        Kidsy.Buzzer.playTone(1200, 500);
        delay(500);
      }
      
      Kidsy.Neopixel.color(GREEN, color_intensity);        // neopixel se mantiene en VERDE
      break;
  }
}

/* --------------------------------------------------------------------------------------------
 *  void makeMove(int moveList)
 * --------------------------------------------------------------------------------------------
 *  Funcion para manejo los movimientos de Kidsy
 *  Esta funcion se encarga de controlar acciones de movimiento de Kidsy, acompañadas por
 *  colores de neopixel.  
 *  
 *  Por ejemplo, kidsy puede girar a la izquierda mientras muestra su color correspondiente,
 *  por medio de la siguiente forma:
 *    
 *  makeMove(LEFT);
 *  
 *  Con lo que, aparte del giro a la izquierda, Kidsy encendera en CYAN el Neopixel y
 *  llamara las acciones de esa direccion (funcion previa)
 * -------------------------------------------------------------------------------------------- */
void makeMove(int moveList, unsigned int speed, int duration) {
  switch(moveList) {
    case FORWARD:
      Kidsy.Neopixel.color(WHITE, color_intensity);
      actions(FORWARD);
      setSpeedLeft = speed;
      setSpeedRight = speed;
      delay(duration);
      Kidsy.Neopixel.off();
      setSpeedLeft = 0;
      setSpeedRight = 0;
      break;
    case BACKWARD:
      Kidsy.Neopixel.color(MAGENTA, color_intensity);
      actions(BACKWARD);
      setSpeedLeft = -speed;
      setSpeedRight = -speed;
      delay(duration);
      Kidsy.Neopixel.off();
      setSpeedLeft = 0;
      setSpeedRight = 0;
      break;
    case LEFT:
      Kidsy.Neopixel.color(CYAN, color_intensity);
      actions(LEFT);
      setSpeedLeft = -speed;
      setSpeedRight = speed;
      delay(duration);
      Kidsy.Neopixel.off();
      setSpeedLeft = 0;
      setSpeedRight = 0;
      break;
    case RIGHT:
      Kidsy.Neopixel.color(YELLOW, color_intensity);
      actions(RIGHT);
      setSpeedLeft = speed;
      setSpeedRight = -speed;
      delay(duration);
      Kidsy.Neopixel.off();
      setSpeedLeft = 0;
      setSpeedRight = 0;
      break;
  }
}

/* --------------------------------------------------------------------------------------------
 *  int getArrow()
 *  -------------------------------------------------------------------------------------------
 *  Funcion para obtener la flecha que se toco de entre las cuatro.
 *  Esta funcion trata las cuatro flechas tactiles como un solo modulo, y determina que 
 *  flecha se toco, o si ninguna flecha de ha tocado.
 *  
 *  Se requiere una variable tipo int para guardar el valor que regresa, de la siguiente forma:
 *  
 *  int arrowTouched;
 *  arrowTouched = getArrow();
 *  
 *  Y si posteriormente busca que se haya presionado la flecha IZQUIERDA puedes hacer esto:
 *  
 *  if(arrowTouched == LEFT) { }
 *  
 *  Si no se toco ninguna flecha, regresara 0. Los valores de las palabras son:
 *  
 *  NONE        -   0
 *  FORWARD     -   1
 *  BACKWARD    -   2
 *  LEFT        -   3
 *  RIGHT       -   4
-------------------------------------------------------------------------------------------- */
int getArrow() {
  if(Kidsy.ArrowForward.read() == TOUCHED)        return(FORWARD);
  else if(Kidsy.ArrowBackward.read() == TOUCHED)  return(BACKWARD);
  else if(Kidsy.ArrowLeft.read() == TOUCHED)      return(LEFT);
  else if(Kidsy.ArrowRight.read() == TOUCHED)     return(RIGHT);
  else return(false);
}
