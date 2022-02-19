#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
const char* ssid = "Dafid@INETMEDIA";
const char* password = "passwordeopo";
int ledPin = 13; // GPIO13
WiFiServer server(80);

void setup() {
  Serial.begin(9600);
  delay(10);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  // Terhubung dengan jaringan WiFi
  Serial.println();
  Serial.println();
  Serial.print("Terhubung : ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  } 
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println(ssid);
  Serial.print("IPaddress: ");
  Serial.println(WiFi.localIP());
  // server diaktifkan
  server.begin();
  Serial.println("Server Started");
  // Print the IP address
  Serial.print("Gunakan browser untuk mengakses : ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");

}

void loop() {
  // Pemerikasan, jika client telah terhubung
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  // Menunggu samapi client mengirimkan beberapa data
  Serial.println("client baru");
  while(!client.available()){
    delay(1);
  }
  // membaca baris pertama untuk melakukan permintaan

  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
  // membanginkan permintan dari browser
  int value = LOW;
  if (request.indexOf("/LED=ON") != -1) {
      digitalWrite(ledPin, HIGH);
      value = HIGH;
  }
  if (request.indexOf("/LED=OFF") != -1) {
      digitalWrite(ledPin, LOW);
      value = LOW;
  }
  // Set ledPin untuk mengatur permintaan
  digitalWrite(ledPin, value);
  // mengembalikan tanggapan
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("");// harus ditambahkan -> untuk menganti baris
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.print("Kondisi Led pin saat ini : ");
  if(value == HIGH) {
    client.print("On");
  } else {
    client.print("Off");
  }
  client.println("<br><br>");
  client.println("<a href=\"/LED=ON\"\"><button>Turn On </button></a>");
  client.println("<a href=\"/LED=OFF\"\"><button>Turn Off </button></a><br />");
  client.println("</html>");
  delay(10);
  Serial.println("Client Terputus");
  Serial.println("");

}
