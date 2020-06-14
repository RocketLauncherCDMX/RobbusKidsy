// ------------------------------------------------ ROBBUS KIDSY ----------------------------------------------
//
// EJEMPLO SIMPLE DE BUZZER CON MELODIA
// Este ejemplo es de uso libre y esta pensado para dar una introduccion al hardware del robot Robbus Kidsy.
// Autor: Rocket Launcher
// Fecha: 03 de marzo de 2020
// ------------------------------------------------------------------------------------------------------------
//
// Antes de continuar, te recomendamos que primero pruebes el programa BuzzerSimple.
//
// Robbus Kidsy cuenta con 1 buzzer:
// - Buzzer
//
// Para hacer sonidos de un tono, la funcion playTone es suficiente, sin embargo existe un metodo
// mas eficiente, y es utilizando notas.
//
// Cada nota tiene una frecuencia distinta, y en el archivo piches.h se encuentra la tabla de
// las notas mas comunes.
//
// Este ejemplo esta basado en el ejemplo original toneMelody de la biblioteca de Arduino, y consiste
// en asignar un arreglo con las notas y uno de igual tamaño con las duraciones, para luego reproducir
// uno por una.
//
// El arreglo melody sera el que contiene el nombre de las notas (sacadas de pitches.h)
// El arreglo duration contiene la duracion de cada uno en funcion de milisegundos sobre duracion. Es decir
// cuartas, octavas, diesciseisavas, etc.
// ------------------------------------------------------------------------------------------------------------

#include <RobbusKidsy.h>

RobbusKidsy Kidsy;    // Llama a Robbus Kidsy

// Arrelgo con las notas de la melodia
int melody[] = {
  NOTE_AS4, NOTE_AS4, NOTE_AS4, NOTE_AS4, NOTE_AS4, NOTE_AS4, NOTE_AS4, NOTE_AS4,
  NOTE_AS4, NOTE_AS4, NOTE_AS4, NOTE_AS4, NOTE_AS4, NOTE_AS4, NOTE_AS4, NOTE_AS4,
  NOTE_AS4, NOTE_AS4, NOTE_AS4, NOTE_AS4, NOTE_D5, NOTE_D5, NOTE_D5, NOTE_D5,
  NOTE_C5, NOTE_C5, NOTE_C5, NOTE_C5, NOTE_F5, NOTE_F5, NOTE_F5, NOTE_F5, 
  NOTE_G5, NOTE_G5, NOTE_G5, NOTE_G5, NOTE_G5, NOTE_G5, NOTE_G5, NOTE_G5, 
  NOTE_G5, NOTE_G5, NOTE_G5, NOTE_G5, NOTE_C5, NOTE_AS4, NOTE_A4, NOTE_F4,
  NOTE_G4, 0, NOTE_G4, NOTE_D5, NOTE_C5, 0, NOTE_AS4, 0, NOTE_A4, 0, NOTE_A4, 
  NOTE_A4, NOTE_C5, 0, NOTE_AS4, NOTE_A4, NOTE_G4,0, NOTE_G4, NOTE_AS5, NOTE_A5, NOTE_AS5, 
  NOTE_A5, NOTE_AS5, NOTE_G4,0, NOTE_G4, NOTE_AS5, NOTE_A5, NOTE_AS5, NOTE_A5, NOTE_AS5, 
  NOTE_G4, 0, NOTE_G4, NOTE_D5, NOTE_C5, 0, NOTE_AS4, 0, NOTE_A4, 0, NOTE_A4, NOTE_A4, 
  NOTE_C5, 0, NOTE_AS4, NOTE_A4, NOTE_G4,0, NOTE_G4, NOTE_AS5, NOTE_A5, NOTE_AS5, NOTE_A5, 
  NOTE_AS5, NOTE_G4,0, NOTE_G4, NOTE_AS5, NOTE_A5, NOTE_AS5, NOTE_A5, NOTE_AS5
 };

// duracion de las notas 1/4, 1/8, 1/16, etc.
int noteDurations[] = {
  8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
  8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
  8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
  8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8
};

void setup() {
  Kidsy.begin();    // Inicializa el hardware del Robbus Kidsy
  
  // Recorre el arreglo de las notas
  for (int thisNote = 0; thisNote < 112; thisNote++) {
    // determina la duracion con la division de 1 segundo / nota.
    int noteDuration = 800 / noteDurations[thisNote];
    Kidsy.Buzzer.playTone(melody[thisNote], noteDuration);

    // para distinguir cada nota, se calcula un tiempo minimo entre ellas.
    // la duracion de la nota + 30% suena bien.
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    Kidsy.Buzzer.noTone();    // Al final, detiene la reproduccion de sonido.
  }
}

void loop() {
  /* Para que no te aturdas, el sonido solo se reproduce 1 vez en el setup, si quieres volver
     a escucharlo, basta que presiones el boton RESET.*/
}