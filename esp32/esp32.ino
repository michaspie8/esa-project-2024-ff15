// ESP 32 DEV BOARD
// 0,5 Wh/ imp
#include <WiFi.h>
#include <HTTPClient.h>  

// Konfiguracja WiFi
const char* ssid = "mangusty";
const char* password = ""; //wypełnić hasłem

// Adres serwera 
const char* serverIP = "http://192.168.2.20";

volatile bool pulseIncoming = false;
int pulseCount = 0;

void IRAM_ATTR handlePulse() {
    pulseIncoming = true;
}

void setup() {
  Serial.begin(115200);
  pinMode(35, INPUT); 
  pinMode(5, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(5), handlePulse, FALLING); // Zliczanie impulsów

  // Połączenie z WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
}

void loop() {
  
  if (pulseIncoming && analogRead(35) < 300) {
    pulseCount++;
    sendImpulseData(pulseCount);
    pulseIncoming = false; // Resetujemy flagę po wysłaniu danych
  }
  delay(20);
}

void sendImpulseData(int pulseCount) {
  if (WiFi.status() == WL_CONNECTED) {  // Sprzawdzenie połączenia
    HTTPClient http;
    String url = String(serverIP) + "/tick/" + String(pulseCount);
    
    http.begin(url);
    int httpResponseCode = http.PUT("");  // just PUT
    http.end(); // Koniec sesji HTTP
  } else {
    Serial.println("WiFi not connected, cannot send data.");
  }
}

