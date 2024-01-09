// ------------------------------------------------ ROBBUS KIDSY ----------------------------------------------
//
// EJEMPLO DE USO DE LA SENTENCIA CONDICIONAL SWITCH, USADO EN PROGRAMACION EN LENGUAJE C
// Este ejemplo es de uso libre y esta pensado para dar una introduccion al software de Robbus Kidsy.
// Autor: Rocket Launcher
// Fecha: 19 de febrero de 2020
// ------------------------------------------------------------------------------------------------------------
//
// Tu Robbus Kidsy es capaz de tomar desiciones. Asi como tu eliges si quieres mirar a la izquierda, derecha,
// arriba o abajo cuando alguien te llama, Robbus es capaz de decidir si quiere ir a la izquierda, derecha, 
// adelante o atras, dependiendo del color que vea.
// La sentencia switch sirve justo para eso, DECIDIR. Si quieres que Robbus tome esa desicion tan dificil, lo
// necesitaras. La sentencia Switch del lenguaje C, sirve para dirigir el programa por un lado u otro, 
// dependiendo de una variable. El programa de tu Robbus (y cualquier computadora o robot) siempre se ejecuta 
// de arriba hacia abajo, pero el flujo se puede cambiar como si de abrir y cerrar llaves se tratara, por 
// medio de un switch. 
// 
// Con este ejemplo aprenderas a utilizar una sentencia switch para hacer que Robbus decida una direccion
// posible entre cuatro opciones distintas. 
// Robbus puede ver 7 colores diferentes:
//
// 0 - BLACK    (NEGRO)
// 1 - RED      (ROJO)
// 2 - GREEN    (VERDE)
// 3 - BLUE     (AZUL)
// 4 - YELLOW   (AMARILLO)
// 5 - CYAN     (CIAN)
// 6 - MAGENTA  (MAGENTA)
// 7 - WHITE    (BLANCO)
//
// Y al programar puedes usar el numero o su "alias", es decir, BLACK en vez de 0, o MAGENTA en vez de 6.
// Ojo, no estas usando directamente sus nombres, cuando usas sus alias, el programa reemplaza esa etiqueta
// con su valor numerico.
//
// Su forma es la siguiente:
// switch() {   <- Cuerpo del switch con llave de inicio
//   case A:    <- Opcion A
//     -----    <- Codigo a ejecutar
//     break;   <- Si entro en la opcion A esto le impide seguir con las siguientes.
//   case B:    <- Opcion B
//     -----    <- Codigo a ejecutar
//     break;   <- Si entro en la opcion B esto le impide seguir con las siguientes.
//
//      .
//      .
//      .
//
//   case N:    <- Opcion N. No hay limite de opciones, mientras no se repitan
//     break;

//   default:   <- Opcion por defecto, si no entro a ninguna opcion, puede ejecutar esta por defecto
//     break;   <- Opcional, ya que esta deberia ser la ultima opcion.
// }            <- Final de la sentencia switch con llave de cierre
//
// Lo mejor de todo es que la sentencia switch no es exclusiva de Robbus, se ocupa en cualquier programa 
// escrito en lenguaje C, por lo que ¡podras programar futuros Robbus y otros robots de la misma forma!
// ------------------------------------------------------------------------------------------------------------

#include<RobbusKidsy.h>

RobbusKidsy Kidsy;              // Llama a Robbus Kidsy

int kidsyColor;                 // Esta variable guardara el color que Robbus vio

void setup() {
  Kidsy.begin();                // Inicializa el hardware del Robbus Kidsy
  Kidsy.ColorSensor.enable();   // Habilita el sensor de color y enciende su led blanco
}

void loop() {
  Kidsy.ColorSensor.read();              // Lee el color actual
  kidsyColor = Kidsy.ColorSensor.value;  // guarda el valor del color en la variable color (0 - 7)
  Kidsy.Neopixel.color(kidsyColor);      // muestra el color en el Neopixel

  // Se escribe la palabra switch, y entre parentesis lo que quieres evaluar, seguido de {} llaves
  switch(kidsyColor) {
    // estamos entre las llaves, el cuerpo del switch y aqui es donde se hara la evaluacion.
    
    case RED:                    // "case" sirve para nombrar un posible caso, termina con :
      Kidsy.Move.forward(100);   // Robbus se mueve al frente
      delay(500);                // pequeña pausa
      Kidsy.Move.stop();         // Robbus se detiene
      delay(500);                // pequeña pausa
      break;                     // si este fue el caso, "break;" rompe el ciclo, y evita que entre a otro caso.
    
    case GREEN:                  // Evalua si el color es verde ...
      Kidsy.Move.backward(100); // ... Robbus se mueve al frente
      delay(500);
      Kidsy.Move.stop();
      delay(500);
      break;                    
    
    case YELLOW:                 // Evalua si el color es azul ...
      Kidsy.Move.turnLeft(100); // ... Robbus se mueve a la izquierda
      delay(500);
      Kidsy.Move.stop();
      delay(500);
      break;
      
    case MAGENTA:                // el ultimo caso es MAGENTA, si llego aqui, significa que no era ninguno de los 3 anteriores...
      Kidsy.Move.turnRight(100);// ... Robbus se mueve a la derecha
      delay(500);
      Kidsy.Move.stop();
      delay(500);
      break;

    default:                     // Si no fue ninguna opcion, siempre podemos dejar un caso auxiliar llamado default
      Kidsy.Move.stop();         // y si llego hasta aqui, es por que no se presiona ninguna flecha y Robbus se detiene.
      break;
  }                              // Este es el termino del ciclo, lo que suceda a continuacion ya no formara parte de el.
}