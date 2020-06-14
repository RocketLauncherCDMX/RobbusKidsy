// ------------------------------------------------ ROBBUS KIDSY ----------------------------------------------
//
// EJEMPLO DE USO DE LA SENTENCIA CONDICIONAL IF, USADO EN PROGRAMACION EN LENGUAJE C
// Este ejemplo es de uso libre y esta pensado para dar una introduccion al software de Robbus Kidsy.
// Autor: Rocket Launcher
// Fecha: 27 de febrero de 2020
// ------------------------------------------------------------------------------------------------------------
//
// Decisiones... de eso se trata la vida. Los robots tambien las pueden hacer y Robbus no es la excepcion.
// Una decision puede cambiar, detener o ralentizar el flujo de un programa. Imagina que vas en un autobus
// y a lo lejos vez como el semaforo cambia de verde a rojo y el autobus comienza a frenar hasta detenerse,
// entonces piensas... "cambia a verde para que podamos continuar", pues esta decision la puede tomar un robot
// con la ayuda de la condicional if.
//
// if sirve para evaluar una variable, ya sea una cantidad, estado o entrada, como por ejemplo el boton A de 
// tu robot, y su forma seria:
//
// if(Kidsy.ButtonA.read() == HOLD_PRESSED) { Kidsy.Led1.on(); }
// Examinando sus partes:
//
// if(                                    ) {}  <- Cuerpo de la condicion if
//    Kidsy.ButtonA.read()                      <- Variable a evaluar
//                         ==                   <- Tipo de operacion
//                            HOLD_PRESSED      <- Valor con el que sera comparado
//
// De esta forma, para que lo que esta dentro de las llaves puede ejecutarse, el resultado de la
// condicion if debe ser verdadero, es decir, el valor leido del boton debe ser igual a PRESSED, el 
// boton debe presionarse para que el LED encienda.
//
// Los tipos de operacion que pueden ser evaluados en un if son:
// ==  igual que (si, es el signo igual 2 veces, no confundir con un solo signo de igual)
// <   menor que
// >   mayor que
// <=  menor o igual
// >=  mayor o igual
// !=  diferente
//
// Ahora bien, te preguntaras, ¿Y que pasa si quiero que el LED se apague al soltar el boton?
// Pues para eso existe la otra parte de un if, else.
// else es el complemento del if, y la expresion completa seria:
//
// if(Kidsy.ButtonA.read() == HOLD_PRESSED) Kidsy.Led1.on();
// else Kidsy.Led1.off();
//
// "Si el boton esta presionado, entonces enciende el LED, de lo contrario, apaga el LED"
//
// Y con estas dos palabras ya podemos tomar una decision en funcion de una entrada. Veamos como se
// puede sacar provecho de esta condicional en tu Robbus Kidsy para encender o apagar los LEDs
// dependiendo de si los botones se presionan o no.
// ------------------------------------------------------------------------------------------------------------

#include<RobbusKidsy.h>

RobbusKidsy Kidsy;    // Llama a Robus Kidsy

void setup() {
  Kidsy.begin();      // Inicializa el hardware del Robbus Kidsy
}

void loop() {
  if(Kidsy.ButtonA.read() == HOLD_PRESSED) Kidsy.Led1.on();
  else Kidsy.Led1.off();

  if(Kidsy.ButtonB.read() == HOLD_PRESSED) Kidsy.Led2.on();
  else Kidsy.Led2.off();

  if(Kidsy.ButtonC.read() == HOLD_PRESSED) Kidsy.Led3.on();
  else Kidsy.Led3.off();
}