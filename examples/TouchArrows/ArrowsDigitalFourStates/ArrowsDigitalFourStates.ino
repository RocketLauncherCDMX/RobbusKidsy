// ------------------------------------------------ ROBBUS KIDSY ------------------------------------------------
//
// EJEMPLO DE LECTURA DIGITAL DE LOS 4 ESTADOS POSIBLES DE LAS FLECHAS DE DIRECCION
// Este ejemplo es de uso libre y esta pensado para dar una introduccion al hardware de Robbus Kidsy
// Autor: Rocket Launcher
// Fecha: 31 de mayo de 2020
// --------------------------------------------------------------------------------------------------------------
//
// Si aun no has visto el ejemplo ArrowsSimple, te invitamos a que lo explores antes de este.
//
// Kidsy Robbus cuenta con 4 flechas de direccion:
// - ArrowForward
// - ArrowBackward
// - ArrowLeft
// - ArrowRight
//
// Estas flechas son sensibles al tacto. Para leer su valor basta con usar la funcion read() la cual:
// Leera un 0 o NOTOUCHED cuando se suelta
// Leera un 1 o TOUCHED cuando se toca
// Leera un 2 o HOLD_TOUCHED cuando se mantiene tocando
// Leera un 3 o HOLD_NOTOUCHED cuando se mantiene sin tocar
//
// Y posteriormente usar la variable status para saber su valor, por ejemplo:
//
// Kidsy.ArrowForward.read();                   // Lee la flecha Adelante
// if(Kidsy.ArrowForward.status == TOUCHED) {}  // Evalua si la flecha se presiono
//
// Tambien puedes guardar lo que la funcion leyo en una variable declarada por ti:
// int forward = Kidsy.ArrowForward.read();
//
// Los 2 primeros casos son utiles para muchos programas, pero los siguientes dos, nos daran acceso
// a funcionalidades mas complejas, por ejemplo, hacer que una flecha tenga 2 comportamientos, toque
// corto y toque sostenido (como la tecla de una computadora), repetir mientras no se suelte,
// hacer algo mientras no se toque y un largo etcetera.
//
// En este ejemplo, haremos que Kidsy acelere y desacelere adelante y atras con las flechas correspondientes
// a traves de pulsaciones rapidas, el nivel de velocidad subira 1 unidad, y con la pulsacion mantenida,
// lo hara de 5 en 5 unidades cada 50ms. Puedes monitorizar la velocidad por el serial.
// --------------------------------------------------------------------------------------------------------------

#include<RobbusKidsy.h>

RobbusKidsy Kidsy;        // Llama a tu Robbus Kidsy

long timestamp;   // guarda el valor de la funcion millis() que cuenta el tiempo de ejecucion del programa
int t = 50;    // guarda el valor en milisegundos de la pausa entre incrementos al mantener la flecha presionada
int speed = 0;            // guarda la velocidad de los motores

void setup() {
  Serial.begin(115200);   // Inicializa comunicacion serial a 115200 baudios
  Kidsy.begin();          // Inicializa el hardware del Robbus Kidsy
}

// banderas - Son variables booleanas que guardan estados verdaderos o falsos
bool flag_firstHold_forward = false;    // primera vez que se mantiene presionada la flecha adelante
bool flag_repeat_forward = false;       // activa la repeticion de la accion al presionar flecha adelante
bool flag_firstHold_backward = false;   // primera vez que se mantiene presionada la flecha atras
bool flag_repeat_backward = false;      // activa la repeticion de la accion al presionar flecha atras



void loop() {
  // Lee los estados de las flechas, estos pueden ser 4:
  // 0 - TOUCHED          - La flecha paso de estar tocandose a no tocarse
  // 1 - NOTOUCHED        - La flecha paso de estar sin tocarse a tocarse
  // 2 - HOLD_TOUCHED     - La flecha se mantiene tocandose
  // 3 - HOLD_NOTOUCHED   - La flecha se mantiene sin tocarse
  
  Kidsy.ArrowForward.read();    // Obtiene el estado actual de las flechas adelante y atras
  Kidsy.ArrowBackward.read();

  // REVISION DE FLECHA ADELANTE
  // --------------------------------------------------------------------------------------------------------------------
  if(Kidsy.ArrowForward.status == TOUCHED) {
    speed++;                // si se toca la flecha adelante, la velocidad aumenta en 1
  }
  // Si la flecha se mantiene tocandose, y es la primera vez ...
  else if(Kidsy.ArrowForward.status == HOLD_TOUCHED && flag_firstHold_forward == false) {
    flag_firstHold_forward = true;    // ... activa bandera de flecha tocandose
    timestamp = millis();             // guarda el valor actual del tiempo en ms
  }
  // Si la bandera de flecha tocandose ya esta activa, pero aun no ha pasado el tiempo para la repeticion ...
  else if(Kidsy.ArrowForward.status == HOLD_TOUCHED && flag_firstHold_forward == true && flag_repeat_forward == false) {
    if(millis() > timestamp + 500) flag_repeat_forward = true; 
    // ... revisa si se supero el tiempo para activar repeticion
  }
  
  if(Kidsy.ArrowForward.status == NOTOUCHED) {  // si la flecha de dejo de tocar
    flag_firstHold_forward = false;   // desactiva bandera de boton presionado por primera vez
    flag_repeat_forward = false;      // desactiva bandera de repeticion
  }

  if(flag_repeat_forward == true) {   // si la bandera de repeticion esta activa
    speed += 5;                       // hace incrementos rapidos de 5
    delay(t);
  }
  // --------------------------------------------------------------------------------------------------------------------

  // REVISION DE LA FLECHA ATRAS
  // --------------------------------------------------------------------------------------------------------------------
  if(Kidsy.ArrowBackward.status == PRESSED) {
    speed--;                // si se toca la flecha atras, la velocidad aumenta en -1
  }
  // Si la flecha se mantiene tocandose, y es la primera vez ...
  else if(Kidsy.ArrowBackward.status == HOLD_TOUCHED && flag_firstHold_backward == false) {
    flag_firstHold_backward = true;   // ... activa bandera de flecha tocandose
    timestamp = millis();             // guarda el valor actual del tiempo en ms
  }
  // Si la bandera de flecha tocandose ya esta activa, pero aun no ha pasado el tiempo para la repeticion ...
  else if(Kidsy.ArrowBackward.status == HOLD_TOUCHED && 
    flag_firstHold_backward == true && flag_repeat_backward == false) {
    if(millis() > timestamp + 500) flag_repeat_backward = true; 
    // ... revisa si se supero el tiempo para activar repeticion
  }
  
  if(Kidsy.ArrowBackward.status == NOTOUCHED) {  // si la flecha se deja de tocar
    flag_firstHold_backward = false;   // desactiva bandera de flecha tocandose por primera vez
    flag_repeat_backward = false;      // desactiva bandera de repeticion
  }

  if(flag_repeat_backward == true) {   // si la bandera de repeticion esta activa
    speed -= 5;                        // hace incrementos rapidos de -5
    delay(t);
  }
  // --------------------------------------------------------------------------------------------------------------------

  if(speed > 255) speed = 255;  // si la velocidad supera los 8 bits (+-255), se mantiene
  if(speed < -255) speed = -255;

  // Asigna la velocidad en los motores, si se quiere cambiar el sentido hacia atras, usar BACK
  Kidsy.Move.MotorLeft(speed);    
  Kidsy.Move.MotorRight(speed);

  Serial.print("velocidad: ");    // imprime la velocidad de ambos motores
  Serial.println(speed);
} 