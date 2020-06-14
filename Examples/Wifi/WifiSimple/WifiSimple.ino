#include<RobbusKidsy.h>
#include<WiFi.h>

const char* ssid     = "ProyectilMx";
const char* password = "PR0Y3CT1L-mx";

WiFiServer server(80);

RobbusKidsy Kidsy;

void setup() {

  Serial.begin(115200);
  Kidsy.begin(BIP);

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

            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            client.print("<a href=\"/F\"> Frente </a> <br> <br>");
            client.print("<a href=\"/S\"> STOP </a> <br> <br>");
            client.print("<a href=\"/B\"> Atras </a> <br>");

            client.println();
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }

        if (currentLine.endsWith("GET /F")) {
          Kidsy.Move.forward(200);
        }
        else if (currentLine.endsWith("GET /S")) {
          Kidsy.Move.stop(); 
        }
        else if (currentLine.endsWith("GET /B")) {
          Kidsy.Move.backward(200);  
        }
      }
    }

    client.stop();
    Serial.println("Client Disconnected.");
  }

}
