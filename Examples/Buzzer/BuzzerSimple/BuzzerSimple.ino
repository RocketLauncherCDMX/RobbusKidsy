// ------------------------------------------------ ROBBUS KIDSY ----------------------------------------------
//
// EJEMPLO SIMPLE DE USO DEL BUZZER
// Este ejemplo es de uso libre y esta pensado para dar una introduccion al hardware del robot Robbus Kidsy.
// Autor: Rocket Launcher
// Fecha: 03 de marzo de 2020
// ------------------------------------------------------------------------------------------------------------
//
// Robbus Kidsy cuenta con 1 buzzer:
// - Buzzer
//
// El buzzer no es otra cosa que una mini bocina, con la que Robbus puede hacer sonidos
// Utilizar el Buzzer es muy sencillo, basta con emplear la funcion Buzzer.playTone().
//
// Kisdy.Buzzer.playTone(100, 50);    // Robbus reproduce un sonido grave durante 50 milisegundos
// Kidsy.Buzzer.playTone(1000);       // Robbus reproduce un sonido agudo de forma indefinida
// 
// El odio humano puede escuchar sonidos en el rango de los 20Hz a los 20000Hz
// Los valores cercanos a 20 son sonidos graves, y los valores cercanos a 20000 son sonidos agudos, aunque
// generalmente sonidos mayores a 15000Hz ya son imperceptibles para muchas personas.
//
// Si usas la segunda forma, Robbus no guardara silencio, a menos que uses la funcion Kidsy.Buzzer.noTone()
//
// Kidsy.Buzzer.playTone(1000);                               // reproduce un sonido agudo de forma indefinida
// if(Kidsy.ButtonA.read() == PRESSED) Kidsy.Buzzer.noTone(); // guarda silencio al presionar el boton A
// ------------------------------------------------------------------------------------------------------------

#include<RobbusKidsy.h>

RobbusKidsy Kidsy;      // Llama a Robbus Kidsy

void setup() {
  Kidsy.begin();        // Inicializa el hardware del Robbus Kidsy

  Kidsy.Buzzer.playTone(500);         // se mantiene sonando
  delay(1000);                        // espera 1 segundo
  Kidsy.Buzzer.playTone(1000);        // se mantiene sonando
  delay(1000);                        // espera 1 segundo
  Kidsy.Buzzer.playTone(1500);        // se mantiene sonando
  delay(1000);                        // espera 1 segundo
  Kidsy.Buzzer.playTone(2000, 1000);  // suena durante 1 segundo
  Kidsy.Buzzer.playTone(2500, 1000);  // suena durante 1 segundo
  Kidsy.Buzzer.noTone();              // deja de sonar
}

void loop() {
  /* Para que no aturdirte, el sonido solo se reproduce una vez en el setup, si quieres volver
     a escucharlo, basta que presiones el boton RESET.*/
}