// ------------------------------------------------ ROBBUS KIDSY ----------------------------------------------
//
// EJEMPLO DE USO DE COMUNICACION POR WIFI USANDO A KIDSY COMO SERVIDOR WEB
// Este ejemplo es de uso libre y esta pensado para dar una introduccion al hardware de Robbus Kidsy.
// Autor: Rocket Launcher, Agradecimientos especiales a Rolando Gonzalez, por su importante colaboracion
//        en este ejemplo.
// Fecha: 27 de mayo de 2020
// ------------------------------------------------------------------------------------------------------------
// Robbus Kidsy cuenta con un Modulo de comunicacion Wifi, por medio de el, es posible conectarlo a
// internet, para lo cual solo es necesario contar con un router inalambrico.
//
// En este ejemplo, se cargara una pequeña pagina web en la memoria de Kidsy, la cual se podra descargar
// desde cualquier dispositivo conectado al mismo router. Este ejemplo solamente funciona con redes privadas
// por lo que no es posible conectarse con Kidsy desde otra IP. En ejemplos mas avanzados se conectara a
// internet para poder acceder a el desde cualquier parte.
//
// Importante:  Para que la conexion funcione, es importante que tanto Kidsy como el dispositivo desde el
//              que lo controlaras, esten conectados a la misma red Wifi.
// 
// Para mas informacion sobre el Hardware, puedes visitar la pagina oficial https://robbus.mx/kidsy
// ------------------------------------------------------------------------------------------------------------

#include<RobbusKidsy.h>
#include<WiFi.h>

// Configura el acceso a tu red Wifi cambiandolo en los siguientes Strings
const char* ssid     = "ssid";
const char* password = "password";

WiFiServer server(80);    // Crea una instancia de servidor Web

RobbusKidsy Kidsy;        // Llama a Robbus Kidsy

void setup() {
  Serial.begin(115200);   // Comunicacion serial
  Kidsy.begin();          // Inicializa el hardware

  if(ssid == "ssid") {
    Serial.println("Por favor pon el nombre de tu red en ssid, y la contraseña en password");
    while(1);
  }

  Serial.println();
  Serial.print("Conectandose a ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);     // proporciona credenciales para la conexion

  while (WiFi.status() != WL_CONNECTED) {   // permanece aqui hasta conectarse
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());           // IP local, esta sera la IP que busques en tu navegador
  Serial.println("Introduce esta direccion en el navegador de cualquier dispositivo conectado a la misma red");

  server.begin();                           // inicializa el servidor de Kidsy
}

void loop() {
  WiFiClient client = server.available();   // Servidor listo para atender peticiones

  if (client) {                             // Si hay una nueva peticion
    Serial.println("New Client.");
    String currentLine = "";
    while (client.connected()) {            // Mientras el cliente este conectado...
      if (client.available()) {             // y disponible
        char c = client.read();
        Serial.write(c); 
        if (c == '\n') {

          if (currentLine.length() == 0) {  // manda la pagina web

            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            client.print("<h2>");
            client.print("<a href=\"forward\">FORWARD</a><br>");
            client.print("<a href=\"backward\">BACKWARD</a><br>");
            client.print("<a href=\"left\">LEFT</a><br>");
            client.print("<a href=\"right\">RIGHT</a><br>");
            client.print("<a href=\"stop\">STOP</a><br><br>");
            
            client.print("<a href=\"led1\">LED1</a><br>");
            client.print("<a href=\"led2\">LED2</a><br>");
            client.print("<a href=\"led3\">LED3</a><br>");
            client.print("<a href=\"led4\">LED4</a><br><br>");

            client.print("<a href=\"red\">NEOPIXEL RED</a><br>");
            client.print("<a href=\"green\">NEOPIXEL GREEN</a><br>");
            client.print("<a href=\"blue\">NEOPIXEL BLUE</a><br><br>");

            client.print("<a href=\"tone1\">BUZZER TONE1</a><br>");
            client.print("<a href=\"tone2\">BUZZER TONE2</a><br>");
            client.print("<a href=\"tone3\">BUZZER TONE3</a><br>");
            client.print("<a href=\"mute\">BUZZER MUTE</a><br>");
            client.print("</h2>");
            
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }

        if (currentLine.endsWith("GET /forward")) {         // si termina con forward
          Kidsy.Move.forward(200);                          // mueve hacia al frente
        }
        else if (currentLine.endsWith("GET /backward")) {   // si termina con backward
          Kidsy.Move.backward(200);                         // mueve hacia atras
        }
        else if (currentLine.endsWith("GET /left")) {       // si termina con left
          Kidsy.Move.turnLeft(200);                         // giro a la izquierda
        }
        else if (currentLine.endsWith("GET /right")) {      // si termina con right
          Kidsy.Move.turnRight(200);                        // giro a la derecha
        }
        else if (currentLine.endsWith("GET /stop")) {       // si termina con stop
          Kidsy.Move.stop();                                // detiene a Kidsy
        }
        else if (currentLine.endsWith("GET /led1")) {       // si termina con led1
          Kidsy.Led1.toggle();                              // cambia estado de Led1
        }
        else if (currentLine.endsWith("GET /led2")) {       // si termina con stop
          Kidsy.Led2.toggle();                              // cambia estado de Led2
        }
        else if (currentLine.endsWith("GET /led3")) {       // si termina con stop
          Kidsy.Led3.toggle();                              // cambia estado de Led3
        }
        else if (currentLine.endsWith("GET /led4")) {       // si termina con stop
          Kidsy.Led4.toggle();                              // cambia estado de Led4
        }
        else if (currentLine.endsWith("GET /red")) {        // si termina con red
          Kidsy.Neopixel.color(RED);                        // Neopixel en rojo
        }
        else if (currentLine.endsWith("GET /green")) {      // si termina con green
          Kidsy.Neopixel.color(GREEN);                      // Neopixel en verde
        }
        else if (currentLine.endsWith("GET /blue")) {       // si termina con blue
          Kidsy.Neopixel.color(BLUE);                       // Neopixel en azul
        }
        else if (currentLine.endsWith("GET /tone1")) {      // si termina con tone1
          Kidsy.Buzzer.playTone(500);                       // Buzzer suena a 500Hz
        }
        else if (currentLine.endsWith("GET /tone2")) {      // si termina con tone2
          Kidsy.Buzzer.playTone(1000);                      // Buzzer suena a 1000Hz
        }
        else if (currentLine.endsWith("GET /tone3")) {      // si termina con tone3
          Kidsy.Buzzer.playTone(1500);                      // Buzzer suena a 1500Hz
        }
        else if (currentLine.endsWith("GET /mute")) {       // si termina con mute
          Kidsy.Buzzer.noTone();                            // Buzzer en silencio
        }
      }
    }
    client.stop();                                          // al termino, desconecta al cliente
    Serial.println("Client Disconnected.");
  }
}
