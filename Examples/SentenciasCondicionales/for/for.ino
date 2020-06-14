// ------------------------------------------------ ROBBUS KIDSY ----------------------------------------------
//
// EJEMPLO DE USO DEL CICLO FOR, USADO EN PROGRAMACION EN LENGUAJE C
// Este ejemplo es de uso libre y esta pensado para dar una introduccion al software del robot Robbus Kidsy.
// Autor: Rocket Launcher
// Fecha: 22 de febrero de 2020
// ------------------------------------------------------------------------------------------------------------
//
// Tu Robbus Kidsy puede ejecutar ciclos, sin la necesidad de que tu intervengas mientras lo hace. Imagina que
// en la escuela te piden realizar la tabla del 6, mentalmenta diras 6x1=6, 6x2=12, 6x3=18, etc. Pues este mismo
// principio lo puede llevar a cabo tu robot.
// Una de las formas en las que Robbus puede realizar un ciclo es mediante un for, y si quieres que sea capaz
// de ejecutar una serie de acciones con incrementos o decrementos te sera muy util.
// El ciclo for del lenguaje C, sirve para hacer un conteo ordenado incrementando o decrementando. 
// Por ejemplo, supongamos que quieres acelerar las llantas de tu robot desde la posicion de reposo, hasta la 
// velocidad maxima suavemente, lo podriamos hacer de la siguiente forma:
//
// Kidsy.Move.MototLeft(1);
// delay(10);
// Kidsy.Move.MototLeft(2);
// delay(10);
// Kidsy.Move.MototLeft(3);
// delay(10);
// y asi hasta llegar a 255. ¿Ves lo tedioso que resulta?, pero si en su lugar usamos un ciclo for:
//
// for(i=0; i<256; i++) {
//   Kidsy.Move.MotorLeft(i);
//   delay(10);
// }
//
// Obtenemos exactamente el mismo resultado, pero con solo 3 líneas de codigo. conveniente ¿No?
// Examinemos la forma que tiene un for:
//
// for(     ;       ;      ) {} <- Este es el cuerpo, consta de la palabra for y un par de ; dentro de parentesis
//      i=0                     <- Este es el valor inicial que tendra tu variable a utilizar.
//            i<255             <- Este es el valor final que alcanzara tu variable
//                    i++       <- Este es el incremento que hara cada que avance. Un ++ indica un incremento de 1.

// Es decir: INICIO, FIN e INCREMENTO.
//
// Como pudiste ver, para realizar la cuenta usamos una variable, la cual requiere ser entera (int) y puedes
// usar una que ya hayas usado amtes, ya que dentro del for se inicializa al valor que es requerido. Tambien podriamos
// definir una variable nueva dentro del mismo ciclo, la cual solamente existiria durante la cuenta:
// for(int i=0; i<255; i++)     <- Aqui la variable i es declara e inicializada, al salir del for, dejara de existir.
//
// Un ciclo for tambien puede dar saltos mayores a 1:
// for(int i=0; i<255; i+=5)    <- Aqui el salto entre pasos es de 5 unidades.
//
// E incluso puede ir de mas a menos:
// for(int i=255; i>=0; i-=10)  <- Se inicia desde 255, hasta 0, en saltos de -10 unidades.
//
// Por ultimo, ¿Notaste el < > y >= de los ejemplos?, es sencillo, significan:
// <  menor que
// >  mayor que
// <= menor o igual que
// >= mayor o igual que
//
// Basta saber que si tu segundo parametro del for es:
// i<10, los valores incrementaran desde el numero inicial hasta 9.
// i<=10, los valores incrementaran desde el numero inicial hasta 10.
// i>10, los valores decrementaran desde el numero mayor hasta 11.
// i>=10, los valores decrementaran desde el numero mayor hasta el 10.
// 
// Y con esto en mente ya podemos hacer un ciclo de un numero a otro con los saltos que querramos. Apliquemos
// este ciclo para acelerar y desacelerar la llanta derecha de tu Robbus Kidsy.
// ------------------------------------------------------------------------------------------------------------

#include<RobbusKidsy.h>

RobbusKidsy Kidsy;        // Llama a Robbus Kidsy

int counter;              // Contador para el ciclo for

void setup() {
  Serial.begin(115200);   // Inicializa la comunicacion serial por USB a 115200 baudios   
  Kidsy.begin();          // Inicializa el hardware del Robbus Kidsy
}

void loop() {
  Serial.println("\nInicio del ciclo for de 0 a 255");
  delay(3000);
  for(counter=0; counter<=255; counter++) {   // ciclo for de 0 a 255 en incrementos de 1
    Kidsy.Move.MotorRight(counter);
    Serial.print("Velocidad: ");
    Serial.println(counter);
    delay(50);    // pausa de 50 milisegundos
  }
  Serial.println("\nInicio del ciclo for de 255 a 0");
  delay(3000);
  for(counter=255; counter>=0; counter--) {   // ciclo for de 255 a 0 en decrementos de 1
    Kidsy.Move.MotorRight(counter);
    Serial.print("Velocidad: ");
    Serial.println(counter);
    delay(50);    // pausa de 50 milisegundos
  }
} 