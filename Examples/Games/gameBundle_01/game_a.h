// Constantes y variables del juego A
// -------------------------------------------------------------------------------------

bool flag_waitTouch = false;    // bandera para esperar el toque de una flecha
bool timeOut = false;           // bandera de limite de tiempo
int moves[MAX_MOVES];           // guarda hasta MAX_MOVE movimientos diferentes
int nextMove = 0;               // lista de movimientos
int movesNumber = 0;            // numero de movimientos por ronda
// -------------------------------------------------------------------------------------

void call_game_a() {
  manageLeds(1,0,0,0);                    // LED1 ON, LED2-LED4 OFF
  Serial.println("Juguemos: KIDSY DICE");
  Serial.println("Me movere " + String(MAX_MOVES) + " veces, y tienes que recordar mis movimientos, luego, presionar las flechas");
  Serial.println("conforme los fui haciendo. ¡Ganaras si recuerdas los " + String(MAX_MOVES) + " movimientos");
  Serial.println();
  Serial.print("¿Estas list@?, comenzaremos en ...");

  timeOut = false;                      // resetea bandera de limite de tiempo
  randomSeed(analogRead(0));            // se usaran valores aleatorios entre 0 y 4
  for(int i=0; i<MAX_MOVES; i++) moves[i] = random(1,5);  // genera los movimientos aleatorioamente
  actions(READY);                       // reproduce tonos y luces
  movesNumber = 1;                      // cantidad de movimientos en cada ronda, comienza en 1
  nextMove = 0;                         // contador de movimientos, comienza en 0
  while(kidsyStatus != GAMEOVER) {      // Se queda aqui hasta que se finalice el juego
    Serial.println();
    Serial.print("Ronda ");
    Serial.print(movesNumber);
    Serial.println(": ");
    for(int i=0; i<movesNumber; i++) {  // ciclo de movimientos por ronda, si aciertas, aumenta en 1
      makeMove(moves[i], SPEED_A, DURATION_A);      // llama la funcion para moverse, con el valor del siguiente mov.
      delay(PAUSE_TIME);
    }
    Serial.println();
    Serial.println("Tu turno...");
    for(int i=0; i<movesNumber; i++) {
      timeStamp = millis();             // guarda una marca de tiempo
      timeOut = false;                  // cada que espera un toque flecha, reinicia la bandera
      do {                              // Al menos 1 vez lee si hay toque de una flecha...
        arrowTouched = getArrow();      // lee las 4 flechas
        if(millis() > timeStamp+5000) { // si pasa el limite de tiempo sin tocas flechas (5 seg)
          timeOut = true;               // se activa bandera limite de tiempo
          Serial.println("¡Tiempo!, debes ser mas rapido.");
        } 
      }
      while(arrowTouched == NONE && timeOut == false);     // ... mientras no se toque ninguna
      if(arrowTouched == moves[i]) {                  // si es la flecha correcta ...
        switch(arrowTouched) {
          case FORWARD:   actions(FORWARD);   break;  // determina las acciones de ese toque
          case BACKWARD:  actions(BACKWARD);  break;
          case LEFT:      actions(LEFT);      break;
          case RIGHT:     actions(RIGHT);     break;
        }
      }
      else {    // ... si no corresponde al movimiento de Kidsy
        if(timeOut == false) Serial.println("¡Incorrecto!");
        actions(INCORRECT);
        Serial.println("¡Te gane! Intentalo de nuevo presionando el BOTON A o cambia de juego");
        Serial.println();
        kidsyStatus = GAMEOVER;     // pasa al estado Fin del juego
        i=movesNumber;              // obliga al contador de movimientos a terminar
      }
    }
    if(kidsyStatus == GAME_A) {     // Si continua en modo de Juego A, quiere decir que fue correcto
      Serial.println("¡Correcto!");
      delay(500);
      movesNumber++;                // incrementa en 1 el numero de movimientos a repetir
      actions(CORRECT);             // reproduce accion correcta
      if(movesNumber > MAX_MOVES) {       // Si se llega al fin de los movimientos
        Serial.println("¡Me has derrotado!, Ganaste :)");
        actions(VICTORY);           // reproduce la accion VICTORIA
        Serial.println("Si quieres intentarlo de nuevo, presiona el BOTON A.");
        Serial.println();
        setSpeedLeft = 255;
        setSpeedRight = -255;
        delay(1000);
        setSpeedLeft = 0;
        setSpeedRight = 0;
        delay(500);
        setSpeedLeft = Kidsy.Move.top_leftSpeed;
        setSpeedRight = Kidsy.Move.top_rightSpeed;
        delay(1000);
        setSpeedLeft = 0;
        setSpeedRight = 0;
        kidsyStatus = GAMEOVER;     // Termina el juego.
      }
    }
  }
  kidsyStatus = IDDLE;    // Pasa al estado de inactivadad para estar listo para otro juego.
}
