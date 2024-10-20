#include <WiFi.h>
#include <HTTPClient.h>

// Konfiguracja WiFi
const char* ssid = "a";
const char* password = "b";

// Adres serwera
const char* serverIP = "http://192.168.2.50/tick/";

// Pin, do którego podłączony jest sygnał impulsowy
const int pulsePin = 4; // Zmień na odpowiedni pin

volatile int pulseCount = 0; // Licznik impulsów
unsigned long lastSendTime = 0; // Ostatni czas wysyłania danych
const unsigned long interval = 10 * 60 * 1000; // 10 minut w milisekundach

void IRAM_ATTR handlePulse() {
  pulseCount++; // Zlicza impulsy
}

void setup() {
  Serial.begin(115200);

  // Konfiguracja pinu impulsowego
  pinMode(pulsePin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(pulsePin), handlePulse, FALLING);

  // Połączenie z WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
}

void loop() {
  unsigned long currentMillis = millis();

  // Sprawdzanie, czy minęło 10 minut od ostatniego wysłania danych
  if (currentMillis - lastSendTime >= interval) {
    lastSendTime = currentMillis; // Aktualizowanie czasu ostatniego wysłania

    // Wysyłanie danych do serwera
    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      String url = String(serverIP) + String(pulseCount);
      http.begin(url);

      // Ustawienie metody PUT
      http.addHeader("Content-Type", "text/plain");
      int httpCode = http.PUT(""); // Pusta treść, wysyła tylko URL

      if (httpCode > 0) {
        Serial.printf("PUT request sent, code: %d\n", httpCode);
      } else {
        Serial.printf("PUT request failed, error: %s\n", http.errorToString(httpCode).c_str());
      }

      http.end();
      pulseCount = 0; // Resetowanie licznika impulsów
    } else {
      Serial.println("WiFi not connected");
    }
  }
}