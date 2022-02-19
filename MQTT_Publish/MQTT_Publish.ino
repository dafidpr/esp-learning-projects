#include <MQTTClient.h>
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <WiFiManager.h>

// Configuration
const char deviceName[] = "PUBLISHER SENSOR";
const char* mqtt_server_ip = "192.168.10.23";
const int mqtt_port = 1883;

bool ledsOn = true;
bool loggingOn = true;

WiFiClient wifiClient;
MQTTClient mqttClient;

unsigned long lastUpdate = 0;
const int updateInterval = 5000;
int lastLdrReading = 0;

// Sensor readings
#define led 16
String ledStatus = "OFF";
int ldr = 0;

const int buffersize = 16;

void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.print("\Could not connect to Wi-Fi. Device is in configuration mode.");
  Serial.print("\nUser must connect to device \"");
  Serial.print(myWiFiManager->getConfigPortalSSID());
  Serial.print("\" as a WiFi access point,");
  Serial.print("\nand open the configuration page at ");
  Serial.print(WiFi.softAPIP());
  Serial.print(" to configure Wi-Fi.\n");
}

// Publish sensor values to MQTT broker
void publishMessages() {
  
  // Assemble the sensor data into a text string (char array)
  char alldata[buffersize];
  memset(alldata, 0, buffersize);
  sprintf(alldata, "%0d %0s", ldr, ledStatus);

  if(loggingOn) {
   Serial.printf("\nOUTBOUND SENSOR DATA:    [%s]", alldata); 
  }

//  Serial.printf("\nOUTBOUND SENSOR DATA:    [%s]", alldata);

  // Publish to the message broker
  mqttClient.publish("building/alldata", alldata);
  mqttClient.publish("building/ldr", String(ldr));
  mqttClient.publish("building/led", ledStatus);
  
}

//char* intToCharArray(int dataValue) {
//  char message[11];
//  sprintf(message, "%0d", dataValue);
//  return message;
//}

// Initialize the device
void setup() {
 
  Serial.begin(9600);
  while (!Serial) {
    ; // Wait for serial port to connect.
  }
  delay(1000);
  Serial.println();
  Serial.println("IoT DEVICE STARTED");
  Serial.printf("Previously used SSID: %s\n", WiFi.SSID().c_str());

  // Use WiFiManager to connect to network
  WiFiManager wifiManager;
  wifiManager.setAPCallback(configModeCallback);
  wifiManager.setDebugOutput(false);
  
  // Attach using SSID and password saved on the device.
  // If can't connect, start an access point.
  wifiManager.autoConnect(deviceName,"password");
 
  // Print the IP address
  Serial.print("\nDevice is on the network at ");
  Serial.print(WiFi.localIP());
  
  // Set up MQTT client
  mqttClient.begin(mqtt_server_ip, mqtt_port, wifiClient);
  
  // Set pins for output to LEDs
  pinMode(led, OUTPUT);

  // Make sure no LED is on initially
  setLEDs(false);

  // Start timing for sensor updates
  lastUpdate = millis();
}

// Main loop
void loop() {
  
  // If we're not connected to MQTT broker, then connect
  if (!mqttClient.connected()) connect();

  // Check for callbacks from MQTT broker
  mqttClient.loop();  
  delay(10);

  // Publish sensor updates if they are due
//  updateSensorValues();
//  updateLEDSettings();
//  publishMessages();
//  delay(1000);
    if(millis() - lastUpdate > updateInterval) {
      lastUpdate = millis();
      updateSensorValues();
      // logSensorValues();
      publishMessages();
      updateLEDSettings();
    }
  
}

// Establish or reestablish connection to WiFi and the MQTT broker
void connect() {
  
  while (WiFi.status() != WL_CONNECTED) {
      Serial.print(".");
    delay(1000);
  }
  Serial.println();
  Serial.print("Device's IP address is ");
  Serial.println(WiFi.localIP());
  Serial.print("WiFi signal strength (RSSI) is ");
  Serial.print(WiFi.RSSI());
  Serial.println(" dBm");

  Serial.print("Connecting to MQTT broker...");
  while (!mqttClient.connect("IOT-SENSOR-01", "try", "try")) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println();
  Serial.println("connected.");
  
}

// Read latest values from sensors
void updateSensorValues() {
  ldr = analogRead(A0);
}

void logSensorValues() {
  if(loggingOn) {
    Serial.print(ldr, 1);
  }
}

// Update LED display on device
void updateLEDSettings() {
  if(ldr > 100){
    setLEDs(false);
  } else {
    setLEDs(true);
  }
//    if(lastLdrReading < ldr) {
//      setLEDs(true);
//    } else if(lastLdrReading == ldr) {
//      setLEDs(false);
//    } else if(lastLdrReading > ldr) {
//      setLEDs(false);
//    }
//    lastLdrReading = ldr;
}

// Set LEDs on or off individually, or all off if LEDs are turned off for display
void setLEDs(bool showLED) {
  digitalWrite(led, (showLED) ? HIGH : LOW);
  ledStatus = (showLED) ? "ON" : "OFF";
}
