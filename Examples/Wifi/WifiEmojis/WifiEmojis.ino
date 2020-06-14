#include<RobbusKidsy.h>
#include <WiFi.h>

///// CAMBIAR A TU RED

const char* ssid     = "ProyectilMx";
const char* password = "PR0Y3CT1L-mx";


WiFiServer server(80);

Robbus Kidsy;

void setup() {

  Serial.begin(115200);
  Kidsy.begin();

  Serial.println();
  Serial.print("Conectandose a ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop() {
  WiFiClient client = server.available();

  if (client) {
    Serial.println("New Client.");
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        if (c == '\n') {

          if (currentLine.length() == 0) {



            //////////////HTML

            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
            client.println("");



            //////////////ESTILO

            client.println("<head><style>A {text-decoration: none;} h1 { font-family: Helvetica; font-size : 1000%; text-decoration: none; display : block;  margin: 0px auto; text-align: center;}</style></head>");



            //////////////CONTENIDO

            client.print("<body>");

            //Emojis para mejor abstracción, se usa el código Unicode en decimal
            client.print("<h1><a href=\"/Front\"> &#11014 </a> <br>");

            client.print("<h1><a href=\"/Left\"> &#11013 </a>         <a href=\"/Stop\"> &#9899 </a>         <a href=\"/Right\"> &#10145 </a> <br>");

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

        if (currentLine.endsWith("GET /Front")) {

          Kidsy.Move.forward(100);
          delay(200);
          Kidsy.Move.stop();

        }
        if (currentLine.endsWith("GET /Left")) {

          Kidsy.Move.turnLeft(100);
          delay(200);
          Kidsy.Move.stop();

        }
        else if (currentLine.endsWith("GET /Stop")) {

          Kidsy.Move.stop();

        }
        else if (currentLine.endsWith("GET /Right")) {

          Kidsy.Move.turnRight(100);
          delay(200);
          Kidsy.Move.stop();

        }
        else if (currentLine.endsWith("GET /Back")) {

          Kidsy.Move.backward(100);
          delay(200);
          Kidsy.Move.stop();

        }

        else if (currentLine.endsWith("GET /Claxon")) {

          Kidsy.Buzzer.playTone(880, 50);
          delay(50);
          Kidsy.Buzzer.playTone(880, 500);

        }
      }
    }

    client.stop();
    Serial.println("Desconectado.");
  }

}
