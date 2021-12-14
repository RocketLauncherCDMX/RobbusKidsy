// ------------------------------------------------ ROBBUS KIDSY ------------------------------------------------
//
// EJEMPLO DE CONTROL DE ROBBUS KIDSY DESDE EL SMARTPHONE POR MEDIO DE LA APLICACION DABBLE
// Este ejemplo es de uso libre y esta pensado para dar una introduccion al hardware de Robbus Kidsy.
// Autor: Rocket Launcher
// Fecha: 12 de diciembre de 2021
// Visita www.robbus.mx/kidsy para mas informacion
// --------------------------------------------------------------------------------------------------------------
//
//   Este ejemplo crea una conexion basica entre Robbus Kidsy y un telefono, por bluetooth 4.2.
//   El presente ejemplo funciona hasta el día 12/12/2021 con la aplicación Dabble para telefonos Android. 
//   Android:   https://play.google.com/store/apps/details?id=io.dabbleapp&hl=es_MX
//   iOS:       https://apps.apple.com/mx/app/dabble-bluetooth-controller/id1472734455
//
//   NOTA: Esta aplicacion es de un tercero y su uso puede variar, no obstante, cualquier cambio se vera reflejado
//         en futuras actualizaciones. Rocket Launcher trabajara arduamente para tener su aplicacion propia en un
//         futuro cercano. Apoyanos comprando nuestros productos :)
//   
//   Pasos para probar este ejemplo:
//
//   1.- Compilar y subir el ejemplo a tu Robbus Kidsy.
//   2.- Serciorarse que tu Robbus Kidsy tenga el interruptor en posicion ON.
//   3.- Instalar la aplicacion Dabble (enlace arriba).
//   4.- Abrir la aplicación. En la pantalla principal encontraras diversas funciones, selecciona Gamepad.
//   5.- En la pantalla del Gamepad, en la parte superior derecha encontraras 2 iconos, el primero es para
//       conectar a tu dispositivo, el segundo es para cambiar de tipo de gamepad. Toca el primero (Conectar).
//   6.- La siguiente pantalla es Seleccione el Dispositivo, donde, si Kidsy ya tiene el programa y se encuentra
//       encendido, debera aparecer en esta lista. Seleccionalo. Si ya tiene el programa, esta encendido, y aun
//       asi no aparece en la lista, presiona el boton RESET de Kidsy o vuelve a seleccionar Conectar en la app.
//   7.- Una vez conectado, ya podras manejar a Kidsy con las flechas y botones. Tambien podras ocupar el pad
//       analogo o el acelerometro, tocando el icono de la derecha.
//   8.- Puedes ver la funcionalidad de cada boton, por medio del puerto Serial de Arduino. (115200 bauds).
// ------------------------------------------------------------------------------------------------------------------

#define CUSTOM_SETTINGS
#define INCLUDE_GAMEPAD_MODULE
#include <DabbleESP32.h>
#include <RobbusKidsy.h>
#include "KidsyDabbleJoystick.h"

RobbusKidsy Kidsy;    // Llama a Robbus Kidsy

char ble_name[20] = "ROBBUS KIDSY";     // Este es el nombre del dispositvo para su conexion

#define FLAT_SPEED            90        // velocidad de una llanta ponchada (magenta)
#define NOMINAL_SPEED         140       // velocidad nominal (negro o rojo)
#define BOOST_SPEED           255       // velocidad con boost (amarillo)
#define GRASS_SPEED           100       // velocidad en el pasto (verde)
#define WATER_SPEED           80        // velocidad en el agua (cyan)
#define EFFECT_TIME           1000      
#define BOOST_TIME            1000      // tiempo que dura el turbo en milisegundos
#define BOOST_COLLECT_DELAY   3000      // retardo para poder agarrar otro boost
#define GRASS_TONE            5000
#define WATER_TONE            7000
#define STOP_TONE             200
#define MAGENTA_TONE          6000
#define STOP                  0         // Velocidad de alto
#define NONE                  0         // Sin elejir
#define LEFT                  1         // llanta izquierda
#define RIGHT                 2         // llanta derecha

bool flag_speedMod = false;           // bandera de modificador de velocidad al pasar por un color
bool flag_boost = false;              // bandera de duracion de un boost
bool flag_boostCollect = false;       // bandera para colectar un boost, 5 segundos en poder agarrar otro
bool flag_flatTire = false;           // bandera de llanta ponchada
bool flag_direction_buttons = false;  // bandera para saber si algun boton de direccion se pulso o no

int leftTireStatus = STOP;            // control de llanta izquierda
int rightTireStatus = STOP;           // control de llanta derecha
int max_speed_left = NOMINAL_SPEED;   // velocidad llanta izquierda
int max_speed_right = NOMINAL_SPEED;  // velocidad llanta derecha
int boost_counter = 0;                // almacena los turbos que vas agarrando
int tireFlated = NONE;
int fixTireCounter = 0;               // contador para arreglar la llanta ponchada
double boost_delayTimer = 0;          // contador del retardo para agarrar un nuevo boost
double boost_timer = 0;               // contador de duracion del boost
double speedMod_timer = 0;            // contador de la duracion de un modificador de velocidad

void setup() {
  Serial.begin(115200);             // make sure your Serial Monitor is also set at this baud rate.
  Dabble.begin(ble_name);      // Enter baudrate of your bluetooth.Connect bluetooth on Bluetooth port present on evive.
  Kidsy.begin();                     // Inicializa el hardware del Robbus Kidsy
  Kidsy.ColorSensor.enable();
}

void loop() {
  Dabble.processInput();             //this function is used to refresh data obtained from smartphone.Hence calling this function is mandatory in order to get data properly from your mobile.
  buttonsProcess();
  
  int new_angle = GamePad.getAngle();
  int b = GamePad.getRadius();
  
  // Process the arrows in the dabble app
  // --------------------------------------------------------------------------------
  if(button_triangle == PRESSED) {
    Serial.println("Triangle");
    Kidsy.Led1.toggle();
  }
  if(button_cross == PRESSED) {
    Serial.println("Cross");
    Kidsy.Led2.toggle();
  }
  if(button_square == PRESSED) {
    Serial.println("Square");
    Kidsy.Led3.toggle();
  }
  if(button_circle == PRESSED) {
    Serial.println("Circle");
    Kidsy.Led4.toggle();
  }
  if(button_select == PRESSED) {
    Serial.println("Select - Sensor de color desabilitado");
    Kidsy.ColorSensor.disable();
  }
  if(button_start == PRESSED) {
    Serial.println("Start - Sensor de color habilitado");
    Kidsy.ColorSensor.enable();
  }
  // --------------------------------------------------------------------------------

  // Process the direction buttons or the angle, not both
  // --------------------------------------------------------------------------------

  if(button_forward == PRESSED) {
    Serial.println("Up");
    flag_direction_buttons = true;
    Kidsy.Move.MotorLeft(max_speed_left);
    Kidsy.Move.MotorRight(max_speed_right);
  }
  else if(button_forward == RELEASED) {
    flag_direction_buttons = false;
    Kidsy.Move.MotorLeft(0);
    Kidsy.Move.MotorRight(0);
  }
  else if(button_backward == PRESSED) {
    Serial.println("Down");
    flag_direction_buttons = true;
    Kidsy.Move.MotorLeft(-max_speed_left);
    Kidsy.Move.MotorRight(-max_speed_right);
  }
  else if(button_backward == RELEASED) {
    flag_direction_buttons = false;
    Kidsy.Move.MotorLeft(0);
    Kidsy.Move.MotorRight(0);
  }
  else if(button_left == PRESSED) {
    Serial.println("Left");
    flag_direction_buttons = true;
    Kidsy.Move.MotorLeft(-max_speed_left);
    Kidsy.Move.MotorRight(max_speed_right);
  }
  else if(button_left == RELEASED) {
    flag_direction_buttons = false;
    Kidsy.Move.MotorLeft(0);
    Kidsy.Move.MotorRight(0);
  }
  else if(button_right == PRESSED) {
    Serial.println("Right");
    flag_direction_buttons = true;
    Kidsy.Move.MotorLeft(max_speed_left);
    Kidsy.Move.MotorRight(-max_speed_right);
  }
  else if(button_right == RELEASED) {
    flag_direction_buttons = false;
    Kidsy.Move.MotorLeft(0);
    Kidsy.Move.MotorRight(0);
  }
  else if(b != 0 && new_angle != 0) {
    if(new_angle != old_angle) {
      Serial.print("Angulo: " + String(new_angle) + "°, Radio: " + String(b));
      if(new_angle >= 60 && new_angle < 120) {
        Serial.println(" - Frente");
        Kidsy.Move.MotorLeft(max_speed_left/SPEED_STEP_NUMBER*b);
        Kidsy.Move.MotorRight(max_speed_right/SPEED_STEP_NUMBER*b);
      }
      else if(new_angle >= 120 && new_angle < 150) {
        Serial.println(" - Vuelta ligera a la Izquierda");
        Kidsy.Move.MotorLeft(max_speed_left/(SPEED_STEP_NUMBER*2)*b);
        Kidsy.Move.MotorRight(max_speed_right/SPEED_STEP_NUMBER*b);
      }
      else if(new_angle <= 60 && new_angle > 30) {
        Serial.println(" - Vuelta ligera a la Derecha");
        Kidsy.Move.MotorLeft(max_speed_left/SPEED_STEP_NUMBER*b);
        Kidsy.Move.MotorRight(max_speed_right/(SPEED_STEP_NUMBER*2)*b);
      }
      else if(new_angle >= 150 && new_angle < 210) {
        Serial.println(" - Vuelta pronunciada a la Izquierda");
        Kidsy.Move.MotorLeft(-max_speed_left/SPEED_STEP_NUMBER*b);
        Kidsy.Move.MotorRight(max_speed_right/SPEED_STEP_NUMBER*b);
      }
      else if(new_angle <= 30 || new_angle > 330) {
        Serial.println(" - Vuelta pronunciada a la Derecha");
        Kidsy.Move.MotorLeft(max_speed_left/SPEED_STEP_NUMBER*b);
        Kidsy.Move.MotorRight(-max_speed_right/SPEED_STEP_NUMBER*b);
      }
      else if(new_angle >= 210 && new_angle < 240) {
        Serial.println(" - Reversa a la Izquierda");
        Kidsy.Move.MotorLeft(-max_speed_left/(SPEED_STEP_NUMBER*2)*b);
        Kidsy.Move.MotorRight(-max_speed_right/SPEED_STEP_NUMBER*b);
      }
      else if(new_angle <= 330 && new_angle > 300) {
        Serial.println(" - Reversa a la Derecha");
        Kidsy.Move.MotorLeft(-max_speed_left/SPEED_STEP_NUMBER*b);
        Kidsy.Move.MotorRight(-max_speed_right/(SPEED_STEP_NUMBER*2)*b);
      }
      else if(new_angle >= 240 && new_angle < 300) {
        Serial.println(" - Reversa");
        Kidsy.Move.MotorLeft(-max_speed_left/SPEED_STEP_NUMBER*b);
        Kidsy.Move.MotorRight(-max_speed_right/SPEED_STEP_NUMBER*b);
      }
    }  
    else if(b == 0) {
      Kidsy.Move.MotorLeft(0);
      Kidsy.Move.MotorRight(0);
    }
    old_angle = new_angle;
  }
  else if(flag_direction_buttons == false) {
    Kidsy.Move.MotorLeft(0);
    Kidsy.Move.MotorRight(0);
  }
  // --------------------------------------------------------------------------------

  Kidsy.ColorSensor.read();
  Kidsy.Neopixel.color(Kidsy.ColorSensor.value, 50);
  switch(Kidsy.ColorSensor.value) {
    case RED:
      max_speed_left = 0;
      max_speed_right = 0;
      break;
    case GREEN:
      max_speed_left = GRASS_SPEED;
      max_speed_right = GRASS_SPEED;
      break;
    case BLUE:
      max_speed_left = WATER_SPEED;
      max_speed_right = WATER_SPEED;
      break;
    case YELLOW:
      max_speed_left = BOOST_SPEED;
      max_speed_right = BOOST_SPEED;
      break;
    case MAGENTA:
      //if(flag_flatTire == false) {
        //flag_flat Tire = true;
        Kidsy.Neopixel.color(MAGENTA);
      //}
      break;
    default:
      max_speed_left = NOMINAL_SPEED;
      max_speed_right = NOMINAL_SPEED;
      break;
  }
}