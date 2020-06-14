// ------------------------------------------------ ROB.BIT KIDSY -----------------------------------------------
//
// Nivel de dificultad: DIFICIL
//
// BUFFER DE MEMORIA PARA GUARDAR MOVIMIENTOS POR MEDIO DE LAS FLECHAS TOUCH 
// Este ejemplo es de uso libre y esta pensado para dar una introduccion al hardware del robot Rob.bit Kidsy.
// Autor: Fabian Alfonso Flores
// Empresa: Proyectil Mx
// Fecha: 01 de marzo de 2020
// --------------------------------------------------------------------------------------------------------------
//
// En este ejemplo se desarrolla una funcionalidad compleja, la cual consiste en un buffer de memoria que puede
// ir guardando las flechas que se tocan, para posteriormente usarse en el movimiento de tu Rob.bit Kidsy.
// El programa hace lo siguiente:
//
// 1. Revisa el boton A para activar o desactivar la grabacion de las flechas en memoria.
// 2. Cuando se activa el modo grabacion, se enciende el LED1. A partir de aqui se guardaran las flechas.
// 3. Cada que toques una flecha, el Neopixel emitira un destello verde y se escuchara el buzzer.
// 4. Por defecto este programa puede guardar 100 flechas distintas, esto por medio de la constante BUFFER_MAX.
// 5. Si se llega al limite del buffer, el Neopixel emitira un latido en rojo.
// 6. En cualquier momento se puede volver a presionar el boton A para terminar la grabacion.
// 7. Una vez que el LED1 se apague, la secuencia estara salvada en la memoria RAM.
// 8. Para reproducir la secuencia, basta con presionar el boton B, lo que encendera el Neopixel en azul.
// 9. La secuenca comenzara, acelerando lentamente las llantas hacia el lado deseado.
// 10. Al terminar la secuencia, el Neopixel se apagara y los motores se detendran.
//
// Además de la funcionalidad descrita, puedes revisar la comunicacion Serial para obtener la informacion
// de cada paso que se va ejecutando, asi como el contenido del Buffer de memoria.
//
// Las Flechas se guardan en RAM, por lo que al presionar de nuevo el boton A o RESET, se perdera la secuencia
// anterior.
//
// Para la aceleracion y desaceleracion de los motores se utiliza el Timer0 del microcontrolador, el cual esta
// interrumpiendo cada milisegundo para actualizar en una unidad la velocidad de ambos motores.
//
// Cada que se completa un paso de la secuencia, los motores se esperan en la nueva velocidad y direccion el
// tiempo que indica la variable timeBetweenMoves que por defecto esta en 250mS. Ademas, la velocidad maxima
// que alcanzan los motores esta dada por la variable movingSpeed, con un valor maximo de 255.
//
// Los modulos utilizados en este ejemplo son:
//
// - Ambos motores
// - Botones A y B
// - Flechas capacitivas
// - Buzzer
// - Neopixel
// - Timer0
// --------------------------------------------------------------------------------------------------------------

#include<Rob_bit_Kidsy.h>

Rob_bit Kidsy;              // Declara una instancia tipo Kidsy, llamada de igual forma

hw_timer_t * timer = NULL;  // apuntador para el timer0
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;   // portMUX usado por el RTOS

#define FORWARD     1   // variables guardadas en el buffer cuando se toca una flecha
#define BACKWARD    2
#define LEFT        3
#define RIGHT       4

#define BUFFER_MAX  100 // Numero maximo de flechas que se pueden guardar

unsigned int timeBetweenMoves = 50;    // Tiempo entre cada movimiento (no cuenta la aceleracion)
int movingSpeed = 150;                  // velocidad maxima

// BANDERAS
bool flag_record = false;           // activa / desactiva la grabacion
bool flag_play = false;             // activa la reproduccion de los movimientos
bool flag_newElement = false;       // sirve para evaluar una nueva flecha
bool flag_bufferFull = false;       // indica si el buffer esta lleno o no
bool flag_reachLeftSpeed = false;   // indica si se alcanzo la velocidad del motor izquierdo
bool flag_reachRightSpeed = false;  // lo mismo con el motor derecho

unsigned int counter = 0;       // contador para el numero de flechas guardadas 
int next_leftSpeed = 0;         // siguiente velocidad de la rueda izquierda
int next_rightSpeed = 0;        // siguiente velocidad de la rueda derecha
int set_leftSpeed = 0;          // velocidad actual del motor izquierdo
int set_rightSpeed = 0;         // velocidad actual del motor derecho

unsigned int arrowBuffer[BUFFER_MAX] = {0};   // Buffer de memoria inicializado a 0

// Funcion de interrupcion del Timer0
void IRAM_ATTR onTimer() {
  portENTER_CRITICAL_ISR(&timerMux);                          // Evita que surga una nueva interrupcion mientras esta en el Timer
  if(set_leftSpeed < next_leftSpeed) set_leftSpeed++;         // Estos cuatro renglones ajustan en un paso la velocidad
  else if(set_leftSpeed > next_leftSpeed) set_leftSpeed--;    // cada que el timer interrumpe (1 vez cada ms)
  if(set_rightSpeed < next_rightSpeed) set_rightSpeed++;
  else if(set_rightSpeed > next_rightSpeed) set_rightSpeed--;
  
  Kidsy.Move.MotorLeft(set_leftSpeed);    // Despues de actualizar la velocidad, actualiza los motores
  Kidsy.Move.MotorRight(set_rightSpeed);  
  
  portEXIT_CRITICAL_ISR(&timerMux);   // Vuelve a permitir las interrupciones
}

void setup() {
  Serial.begin(115200);   // Inicia la comunicacion Serial
  Kidsy.begin();          // Inicializa el hardware del Rob.bit Kidsy
  
  // Configuracion del timer:
  // 0       - Inicializa el Timer0, Rob.bit Kidsy dispone de 4 timers (0 al 3)
  // 80      - Valor del preescaler para 80MHz / 80 = 1000000 veces por segundo
  // true    - Cuenta ascendente, false: cuenta descendente
  timer = timerBegin(0, 80, true);
  
  // timer    - Pasamos la variable timer creada
  // &onTimer - funcion onTimer
  // true     - la interrupcion generada sera ascendente, false: descendente
  timerAttachInterrupt(timer, &onTimer, true);  
  
  // timer   - Puntero al timer
  // 1000    - Valor del contador en el cual el timer interrumpira (ms para un valor de preescaler de 80 y velocidad de 80MHz)
  // true    - Interrupcion periodica, false: interrumpe una sola vez 
  timerAlarmWrite(timer, 1000, true);

  //  Habilitamos el timer que acabamos de crear
  timerAlarmEnable(timer);      
  
  Serial.println("Presiona el Boton A para grabar una secuencia de Movimientos con las flechas");
}

void loop() {
  Kidsy.ArrowForward.read();    // Lee las 4 flechas Touch (importante hacerlo solo una vez por loop)
  Kidsy.ArrowBackward.read();
  Kidsy.ArrowLeft.read();
  Kidsy.ArrowRight.read();

  if(flag_record == true) {     // Si la bandera de grabacion esta activa
    Kidsy.Led1.on();            // Enciende LED1
  }
  else Kidsy.Led1.off();        // Si no, apaga LED1
  
  if(Kidsy.ButtonA.read() == PRESSED) {                     // Si se presiona BotonA
    Kidsy.Buzzer.playTone(200, 100);                        // Emite pitido el Buzzer
    flag_record = !flag_record;                             // cambia el valor de la bandera record
    if(flag_record == true) {                               // Si la bandera record esta activa ...
      Serial.println("Grabando... Presiona las flechas");
      counter = 0;                                          // Comienza la grabacion con el contador a 0
      flag_bufferFull = false;                              // borra la bandera buffer lleno
    }
    else {                                                                          // Si la bandera record esta en 0
      Serial.println("Fin de la grabacion. Presiona el Boton B para reproducir");   // Fin de la grabacion
    }
  }

  if(Kidsy.ButtonB.read() == PRESSED) {     // Se presiona Boton A
    Kidsy.Buzzer.playTone(200, 100);        // Genera pitido el Buzzer
    if(flag_record == true) Serial.println("No se puede reproducir mientras se graba");   // ... si se esta grabando
    else if(counter == 0) Serial.println("No hay ninguna direccion guardada");            // ... si no hay flechas salvadas
    else flag_play = true;    // Si no se graba y si hay flechas, activa reproduccion
  }

  if(flag_record == true) {                       // La bandera de grabacion se activo
    if(Kidsy.ArrowForward.status == PRESSED)  {   // Evalua cada flecha, si se presiona...
      arrowBuffer[counter] = FORWARD;             // ... guarda el valor correspondiente en la posicion actual del buffer ...
      flag_newElement = true;                     // ... y activa bandera de nuevo elemento
    }
    else if(Kidsy.ArrowBackward.status == PRESSED)  {
      arrowBuffer[counter] = BACKWARD; 
      flag_newElement = true;
    }
    else if(Kidsy.ArrowLeft.status == PRESSED)  {
      arrowBuffer[counter] = LEFT; 
      flag_newElement = true;
    }
    if(Kidsy.ArrowRight.status == PRESSED)  {
      arrowBuffer[counter] = RIGHT; 
      flag_newElement = true;
    }
  }

  if(flag_newElement == true && flag_bufferFull == false) {   // Si hay un nuevo elemento y el buffer aun no esta lleno
    Kidsy.Buzzer.playTone(400, 50);                           // Emite un pitido el Buzzer
    flag_newElement = false;                                  // Borra bandera nuevo elemento
    Kidsy.Neopixel.color(GREEN);                              // Destello de Neopixel en Verde
    delay(50);
    Kidsy.Neopixel.off();
    Serial.print("Buffer[");                                  // Imprime la posicion actual del Buffer
    Serial.print(counter);
    Serial.print("] = ");
    if(arrowBuffer[counter] == FORWARD) Serial.println("Forward");
    else if(arrowBuffer[counter] == BACKWARD) Serial.println("Backward");
    else if(arrowBuffer[counter] == LEFT) Serial.println("Left");
    else if(arrowBuffer[counter] == RIGHT) Serial.println("Right");
    if(counter < BUFFER_MAX) counter++;                         // Mientras no llegue al maximo, sigue incrementando el contador
  }

  if(counter == BUFFER_MAX) flag_bufferFull = true;             // Al llegar al maximo, activa bandera de buffer lleno
  
  if(flag_newElement == true && flag_bufferFull == true) {      // Evalua si hay una nueva flecha y el buffer  se lleno
    flag_newElement = false;                                    // resetea bandera de nuevo elemento
    Serial.println("Buffer lleno, presiona A para terminar");   
    Kidsy.Neopixel.heartBeat(RED);                              // El buffer lleno se indica con un latido en rojo
  }

  if(flag_play) {                                               // si se activo la bandera de reproduccion
    Kidsy.Neopixel.color(BLUE);                                 // activa el Neopixel en azul
    delay(1000);                                                // espera 1 segundo a que lo dejes en una superficie
    for(int i=0; i<counter; i++) {                              // comienza el ciclo para revisar el buffer
      Kidsy.Buzzer.playTone(400, 50);                           // Pitido de inicio para cada flecha
      Serial.println("------------------------------------");
      Serial.print("\nBuffer[");                                // Imprime posicion de memoria
      Serial.print(i);
      Serial.print("] = ");
      if(arrowBuffer[i] == FORWARD) {                           // Evalua la direccion para cada posicion, en este caso es Forward
        Serial.println("Forward");                              // por lo que pone ambos valores next_leftSpeed y next_rightspeed
        Serial.println("------------------------------------"); // a la velocidad de la variable movingspeed, con signo positivo
        next_leftSpeed = movingSpeed;                           
        next_rightSpeed = movingSpeed;
        while((next_leftSpeed != set_leftSpeed) || (next_rightSpeed != set_rightSpeed)) {             // Mientras no alcance la velocidad
          Serial.print("Izquierda: (" + String(next_leftSpeed) + ")," + String(set_leftSpeed));       // en ambos motores, seguira 
          Serial.println(" / Derecha: (" + String(next_rightSpeed) + ")," + String(set_rightSpeed));  // actualizando en la interrupcion
        }
        delay(timeBetweenMoves);                                // al final de la aceleracion, espera el valor de esta variable
      }
      else if(arrowBuffer[i] == BACKWARD) {                     // Lo mismo se repite para cada direccion ...
        Serial.println("Backward");
        Serial.println("------------------------------------");
        next_leftSpeed = -movingSpeed;
        next_rightSpeed = -movingSpeed;
        while((next_leftSpeed != set_leftSpeed) || (next_rightSpeed != set_rightSpeed)) {
          Serial.print("Izquierda: (" + String(next_leftSpeed) + ")," + String(set_leftSpeed));
          Serial.println(" / Derecha: (" + String(next_rightSpeed) + ")," + String(set_rightSpeed));
        }
        delay(timeBetweenMoves);
      }
      else if(arrowBuffer[i] == LEFT) {                         // Izquieda ...
        Serial.println("Left");
        Serial.println("------------------------------------");
        next_leftSpeed = -movingSpeed;
        next_rightSpeed = movingSpeed;
        while((next_leftSpeed != set_leftSpeed) || (next_rightSpeed != set_rightSpeed)) {
          Serial.print("Izquierda: (" + String(next_leftSpeed) + ")," + String(set_leftSpeed)); 
          Serial.println(" / Derecha: (" + String(next_rightSpeed) + ")," + String(set_rightSpeed));
        }
        delay(timeBetweenMoves);
      }
      else if(arrowBuffer[i] == RIGHT) {
        Serial.println("Right");                                // Derecha ...
        Serial.println("------------------------------------");
        next_leftSpeed = movingSpeed;
        next_rightSpeed = -movingSpeed;
        while((next_leftSpeed != set_leftSpeed) || (next_rightSpeed != set_rightSpeed))  {
          Serial.print("Izquierda: (" + String(next_leftSpeed) + ")," + String(set_leftSpeed));
          Serial.println(" / Derecha: (" + String(next_rightSpeed) + ")," + String(set_rightSpeed));
        }
        delay(timeBetweenMoves);
      }
    }
    Serial.println("\nFin de la secuencia. Presiona B para repetir o A para grabar");   // Despues de la secuencia...
    Kidsy.Neopixel.off();               // Apaga el Neopixel
    next_leftSpeed = 0;                 // pone en 0 los motores
    next_rightSpeed = 0;
    flag_play = false;                  // Borra la bandera reproduccion
    delay(500);                         // Espera a que los motores se detengan
    for(int i=0; i<3; i++) {            // Emite 3 pitidos para finalizar
      Kidsy.Buzzer.playTone(1000,50);
      delay(25);
      Kidsy.Buzzer.noTone();
      delay(25);
    }
  }
}