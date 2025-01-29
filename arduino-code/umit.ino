#include <CodeCell.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <ArduinoJson.h>

// CodeCell instance
CodeCell myCodeCell;

// Sensor variables
float accX, accY, accZ;  // Accelerometer data
float magX, magY, magZ;  // Magnetometer data
float gyroX, gyroY, gyroZ; // Gyroscope data

// Wi-Fi settings
const char* ssid = "HUAWEI-B311-205C";  // Replace with your Wi-Fi SSID
const char* password = "20D5TH0N98R";   // Replace with your Wi-Fi password

// UDP settings
WiFiUDP udp;
const char* udpAddress = "192.168.8.103";  // Replace with PC's IP address
const int udpPort = 7771;  // Port to send data to

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to Wi-Fi");

  // Print ESP32 IP Address
  Serial.print("ESP32 IP Address: ");
  Serial.println(WiFi.localIP());

  // Initialize CodeCell
  myCodeCell.Init(MOTION_ACCELEROMETER + MOTION_GYRO + MOTION_MAGNETOMETER);
}

void loop() {
  // Read sensor data
  myCodeCell.Motion_AccelerometerRead(accX, accY, accZ);
  myCodeCell.Motion_GyroRead(gyroX, gyroY, gyroZ);
  myCodeCell.Motion_MagnetometerRead(magX, magY, magZ);
   
  // Create a JSON object
  StaticJsonDocument<200> doc;
  doc["accX"] = accX;
  doc["accY"] = accY;
  doc["accZ"] = accZ;
  doc["gyroX"] = gyroX;
  doc["gyroY"] = gyroY;
  doc["gyroZ"] = gyroZ;
  doc["magX"] = magX;
  doc["magY"] = magY;
  doc["magZ"] = magZ;

  // Serialize JSON
  String jsonMessage;
  serializeJson(doc, jsonMessage);

  // Send data over UDP
  udp.beginPacket(udpAddress, udpPort);
  udp.write(reinterpret_cast<const uint8_t*>(jsonMessage.c_str()), jsonMessage.length());
  udp.endPacket();

  // Debugging output
  Serial.println("Sent data:");
  Serial.println(jsonMessage);

  delay(10);  // Adjust delay if needed

  // Reconnect Wi-Fi if disconnected
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Wi-Fi disconnected, reconnecting...");
    WiFi.begin(ssid, password);
  }
}
