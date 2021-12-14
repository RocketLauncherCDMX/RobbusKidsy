// ------------------------------------------------ ROBBUS KIDSY ----------------------------------------------
//
// EJEMPLO DE USO DE COMUNICACION POR WIFI USANDO A KIDSY COMO SERVIDOR WEB CON INTERFAZ GRAFICA
// Este ejemplo es de uso libre y esta pensado para dar una introduccion al hardware de Robbus Kidsy.
// Autor: Rocket Launcher, Agradecimientos especiales a Rolando Gonzalez, por su importante colaboracion
//        en este ejemplo.
// Fecha: 27 de mayo de 2020
// ------------------------------------------------------------------------------------------------------------
// Si aun no has probado el ejemplo WifiSimple, te recomendamos que lo cargues primero.
//
// Robbus Kidsy cuenta con un Modulo de comunicacion Wifi, por medio de el, es posible conectarlo a
// internet, para lo cual solo es necesario contar con un router inalambrico.
//
// A continuacion, se cargara una pequeña pagina web en la memoria de Kidsy, la cual se podra descargar
// desde cualquier dispositivo conectado al mismo router. Este ejemplo solamente funciona con redes privadas
// por lo que no es posible conectarse con Kidsy desde otra IP. En ejemplos mas avanzados se conectara a
// internet para poder acceder a el desde cualquier parte.
//
// A diferencia del ejemplo WifiSimple, aqui se interactua por medio de emojis, para las direcciones y para
// sonar el buzzer.
//
// Importante:  Para que la conexion funcione, es importante que tanto Kidsy como el dispositivo desde el
//              que lo controlaras, esten conectados a la misma red Wifi.
// 
// Para mas informacion sobre el Hardware, puedes visitar la pagina oficial https://robbus.mx/kidsy
// ------------------------------------------------------------------------------------------------------------

#include<RobbusKidsy.h>
#include <WiFi.h>

///// CAMBIAR A TU RED

// Configura el acceso a tu red Wifi cambiandolo en los siguientes Strings
const char* ssid     = "ssid";
const char* password = "password";

WiFiServer server(80);    // Crea una instancia de servidor Web

RobbusKidsy Kidsy;        // Llama a Robbus Kidsy

void setup() {
  Serial.begin(115200);   // Comunicacion serial
  Kidsy.begin();          // Inicializa el hardware de Kidsy

  if(ssid == "ssid") {
    Serial.println("Por favor pon el nombre de tu red en ssid, y la contraseña en password");
    while(1);
  }

  Serial.println();
  Serial.print("Conectandose a ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);               // proporciona credenciales para la conexion

  while (WiFi.status() != WL_CONNECTED) {   // permanece aqui hasta conectarse
    delay(500);
    Serial.print(".");
  }

  Kidsy.Led1.on();
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

            //HTML
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
            client.println("");

            //ESTILO
            client.println("<head><style>A {text-decoration: none;} h1 { font-family: Helvetica; font-size : 1000%; text-decoration: none; display : block;  margin: 0px auto; text-align: center;}</style></head>");

            //CONTENIDO
            client.print("<body>");

            //Emojis para mejor abstracción, se usa el código Unicode en decimal
            client.print("<h1><a href=\"/Front\"> &#11014 </a> <br>");
            client.print("<h1><a href=\"/Left\"> &#11013 </a>  <a href=\"/Stop\"> &#9899 </a> <a href=\"/Right\"> &#10145 </a> <br>");
            client.print("<h1><a href=\"/Back\"> &#11015 </a> <br> <br>");
            client.print("<h1><a href=\"/Claxon\"> &#128266 </a> <br>");

            client.print("</body>");

            client.println();
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }

        if (currentLine.endsWith("GET /Front")) {         // si termina con Front
          Kidsy.Move.forward(100);                        // mueve hacia el frente
          delay(200);
          Kidsy.Move.stop();
        }
        else if (currentLine.endsWith("GET /Left")) {     // si termina con Left
          Kidsy.Move.turnLeft(100);                       // gira a la izquierda
          delay(200);
          Kidsy.Move.stop();
        }
        else if (currentLine.endsWith("GET /Stop")) {     // si termina en Stop
          Kidsy.Move.stop();                              // se detiene
        }
        else if (currentLine.endsWith("GET /Right")) {    // si termina en Right
          Kidsy.Move.turnRight(100);                      // gira a la dercha
          delay(200);
          Kidsy.Move.stop();
        }
        else if (currentLine.endsWith("GET /Back")) {     // si termina en Back
          Kidsy.Move.backward(100);                       // mueve hacia atras
          delay(200);
          Kidsy.Move.stop();
        }
        else if (currentLine.endsWith("GET /Claxon")) {   // si termina en Claxon
          Kidsy.Buzzer.playTone(880, 50);                 // reproduce sonido
          delay(50);
          Kidsy.Buzzer.playTone(880, 500);
        }
      }
    }

    client.stop();                                        // al termino, desconecta al cliente
    Serial.println("Desconectado.");
  }

}
