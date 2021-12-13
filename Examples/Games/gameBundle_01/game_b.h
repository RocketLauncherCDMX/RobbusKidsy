// Constantes y variables del juego B
// -------------------------------------------------------------------------------------

bool flag_record = false;       // bandera para grabar las flechas en la memoria
bool flag_configureB = false;   // bandera para configurar el juego B
bool flag_memOk = false;        // bandera de contenido en memoria, comienza vacia
int arrowBuffer[MAX_ARROWS];    // memoria para almacenar hasta 100 direcciones
int arrowCounter = 0;           // contador de direcciones guardadas
// -------------------------------------------------------------------------------------

void call_game_b() {
  if(flag_configureB == false) {          // bandera para acceder a alaconfiguracion del juego B
    flag_configureB = true;               // solo accede una vez al entrar a este juego
    manageLeds(0,1,0,0);                  // LED2 encendido, LED1, LED3, LED4 apagados
    Serial.println("Juguemos: GUIA A KIDSY");
    Serial.println("Dejare que tu me guies por donde tengo que ir.");
    Serial.println("Para hacerlo, deberas grabar una lista de instrucciones presionando las flechas.");
    Serial.println("Si estas listo, presiona A para comenzar a indicarme direcciones");
  }
  
  if(Kidsy.ButtonA.read() == PRESSED) {   // evalua el boton A
    actions(INTERACT_SOUND);                    // reproduce accion interacion
    if(flag_record == false) {            // si al presionar el boton la bandera esta inactiva
      Serial.println("Modo grabacion - Presiona las flechas que quieras guardar");
      arrowCounter = 0;                   // reinicia contador de direcciones
      flag_record = true;                 // se se presiona boton A cambia el estado de bandera
      Kidsy.Led4.set(flag_record);        // refleja el estado grabacion en LED4
    }
  }

  while(flag_record == true) {                    // Mientras el modo grabacion esta activo
    if(Kidsy.ButtonA.read() == PRESSED) {         // Si se presiona de nuevo boton A
      if(arrowCounter > 0) {
        flag_memOk = true;                        // activa bandera de contenido en memoria
        flag_record = false;                      // desactiva bandera de grabacion
        Kidsy.Led4.set(flag_record);              // refleja el estado grabacion en LED4
        Serial.println("Fin de grabacion - Presiona boton B para reproducir");
        actions(CORRECT);                         // reproduce accion correcta
      }
      else {
        Serial.println("No puedes salir del modo de grabacion sin haber guardado ninguna flecha");
        actions(INCORRECT);                       // accion incorrecta
      }
    }
    
    arrowTouched = getArrow();                 // lee las 4 flechas ...
    if(arrowTouched != NONE) {
      if(arrowCounter < MAX_ARROWS) {               // si aun hay memoria disponible
        arrowBuffer[arrowCounter] = arrowTouched;   // guarda la direccion tocada
        Serial.print(arrowCounter);
        Serial.print(": ");
        actions(arrowTouched);                       // ejecuta la accion de la flecha correspondiente
        arrowCounter++;                             // aumenta contador
      }
      else {                                        // si la memoria llego a su limite
        actions(CORRECT);
        Serial.println("Memoria llena, presiona B para reproducir");
        flag_record = false;                        // desactiva bandera de modo grabacion
        flag_memOk = true;                          // activa bandera de contenido en memoria
      }
    }
  }

  if(Kidsy.ButtonB.read() == PRESSED) {
    if(flag_memOk == false) {
      actions(INCORRECT);
      Serial.println("Memoria vacia, primero graba algunas direcciones. Presiona A para grabar.");
    }
    else {
      actions(READY);
      Serial.println("Reproduciendo direcciones...");
      for(int i=0; i<arrowCounter; i++) {
        Serial.print("Paso ");
        Serial.print(i);
        Serial.print(": ");
        makeMove(arrowBuffer[i], SPEED_B, DURATION_B);
        delay(350);
      }
      actions(CORRECT);
      Serial.println("Fin de la secuencia. Presiona B para repetir, o A para grabar una nueva");
    }
  }
}
