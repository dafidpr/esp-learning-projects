#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
const char* ssid = "Dafid@INETMEDIA";
const char* password = "rahasialah";
const char* host = "192.168.100.25";
int sensorPin = A0; 

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(10);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

}

void loop() {
  
  // put your main code here, to run repeatedly:
  Serial.println("Koneksi ke SERVER dimulai ");
  Serial.print("IP Server : ");
  Serial.println(host);
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("Koneksi GAGAL");
    return;
  }

  int sensorValue = analogRead(sensorPin);
  Serial.print("Nilai Sensor :");
  Serial.println(sensorValue);
  delay(5000);

  String url = "/esp8266-database/index.php?";
  url += "value="; 
  url += sensorValue;
  Serial.print("URL: ");
  Serial.println(url);
  
  client.print(String("GET ") + url + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" +"Connection: close\r\n\r\n");
 
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Batas waktu koneksi HABIS ! <<<<");
      Serial.println(">>> koneksi dihentikan ! <<<<");
      client.stop();
      return;
    }
  }
  while (client.available()) {
    String line = client.readStringUntil('\r');
  
    if (line.indexOf("sukses") != -1) {
      Serial.println();
      Serial.println("INSERT data berhasil");
    } else if (line.indexOf("gagal") != -1) {
      Serial.println();
      Serial.println("Maaf, INSERT data berhasil");
    }
  }
  Serial.println();
  Serial.println("Koneksi database dihentikan");
  delay(50000);


}
