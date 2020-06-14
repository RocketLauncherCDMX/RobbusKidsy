// ------------------------------------------------ ROBBUS KIDSY -------------------------------------------------
//
// EJEMPLO DE SONIDOS CON EL BUZZER POR MEDIO DE LAS FLECHAS DE DIRECCION
// Este ejemplo es de uso libre y esta pensado para dar una introduccion al hardware de Robbus Kidsy.
// Autor: Rocket Launcher
// Fecha: 20 de febrero de 2020
// ---------------------------------------------------------------------------------------------------------------
//
// Robbus Kidsy cuenta con 1 buzzer:
// - Buzzer
//
// El buzzer no es otra cosa que una mini bocina, con la que Robbus es capaza de hacer sonidos
// Utilizar el Buzzer es muy sencillo, basta con emplear la funcion Buzzer.playTone().
//
// Un tono es un sonido que se repite muchas veces en un segundo, y dependiendo de esa cantidad de veces, sera
// como lo escuches, grave o agudo. Para que Kidsy Robbus haga sonidos, basta con poner dentro de la funcion
// playTone() la frecuencia (numero de veces por segundo o Hertz Hz) que tendra tu tono, y, si quieres que el 
// sonido dure cierto tiempo, la cantidad en milisegundos (recuerda que 1 segundo tiene 1000 milisegundos) 
// por ejemplo:
//
// Kisdy.Buzzer.playTone(100, 50);    // Robbus reproduce un sonido grave durante 50 milisegundos
// Kidsy.Buzzer.playTone(1000);       // Robbus reproduce un sonido agudo de forma indefinida
// 
// El odio humano puede escuchar sonidos en el rango de los 20Hz a los 20000Hz
// Los valores cercanos a 20 son sonidos graves, y los valores cercanos a 20000 son sonidos agudos, aunque
// generalmente sonidos mayores a 15000Hz ya son imperceptibles para muchas presonas.
//
// Si usas la segunda forma, Robbus no guardara silencio, a menos que uses la funcion Kidsy.Buzzer.noTone()
//
// Kidsy.Buzzer.playTone(1000);       // Robbus reproduce un sonido agudo de forma indefinida
// if(Kidsy.ButtonA.read() == PRESSED) Kidsy.Buzzer.noTone();   // Robbus guarda silencio al presionar el boton A
// ---------------------------------------------------------------------------------------------------------------

#include<RobbusKidsy.h>

RobbusKidsy Kidsy;        // Llama a tu Robbus Kidsy

void setup() {
  Serial.begin(115200);
  Kidsy.begin();          // Inicializa el hardware del Robbus Kidsy
}

void loop() {
  if(Kidsy.ArrowForward.read() == TOUCHED) {   // tono de 300Hz durante 50ms
    Kidsy.Led1.on();
    Kidsy.Buzzer.playTone(300, 50);
    Serial.println("TOUCHED");
    Kidsy.Led1.off();
  }
  if(Kidsy.ArrowBackward.read() == TOUCHED) {  // tono de 500Hz durante 50ms
    Kidsy.Led2.on();
    Kidsy.Buzzer.playTone(500, 50);
    Serial.println("TOUCHED");
    Kidsy.Led2.off();
  }
  if(Kidsy.ArrowLeft.read() == TOUCHED) {      // tono de 700Hz durante 50ms
    Kidsy.Led3.on();
    Kidsy.Buzzer.playTone(700, 50);
    Serial.println("TOUCHED");
    Kidsy.Led3.off();
  }
  if(Kidsy.ArrowRight.read() == TOUCHED) {     // tono de 900Hz durante 50ms
    Kidsy.Led4.on();
    Kidsy.Buzzer.playTone(900, 50);
    Serial.println("TOUCHED");
    Kidsy.Led4.off();
  }

  // Boton A reproduce un tono de 900Hz y Boton B lo detiene
  if(Kidsy.ButtonA.read() == PRESSED) Kidsy.Buzzer.playTone(900);
  if(Kidsy.ButtonB.read() == PRESSED) Kidsy.Buzzer.noTone();
}