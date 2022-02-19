#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
const char* ssid = "Dafid@INETMEDIA";
const char* password = "rahasialah";
int sensorPin = A0; 
WiFiServer server(80);

void setup() {
  Serial.begin(9600);
  delay(10);
 
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
  Serial.print("Subnetmask : ");
  Serial.println(WiFi.subnetMask());
  Serial.print("MAC Address : ");
  Serial.println(WiFi.softAPmacAddress());
  
  server.begin();
  Serial.println("Server Started");
 
  Serial.print("Gunakan browser untuk mengakses : ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");

}

void loop() {
  
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
 
  Serial.println("Client Baru");
  while(!client.available()){
    delay(1);
  }

  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
 
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("");// harus ditambahkan -> untuk menganti baris
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<head><title>Membaca Sensor LDR Dengan ESP8266</title></head>");
  client.print("IP Address : ");
  client.println(WiFi.localIP());
  client.println("<br>");
  client.print("Subnetmask : ");
  client.println(WiFi.subnetMask());
  client.println("<br>");
  client.print("MAC Address : ");
  client.println(WiFi.softAPmacAddress());
  client.println("<br>");

  int value = analogRead(sensorPin);
  Serial.print("Nilai Sensor : ");
  Serial.println(value);
  
  client.print("Nilai Sensor : ");
  client.print(value);
  client.println("<br><br>");
  client.println("<a href=\"/\"\"><button>Reset Sensor </button></a>");
  client.println("</html>");
  delay(10);
  Serial.println("Client Terputus");
  Serial.println("");

}
