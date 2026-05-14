#include <Arduino.h>
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include "DHT.h"
#include <ESP32Servo.h>

// 🔑 Credenciales Blynk
#define BLYNK_TEMPLATE_ID "TMPL2NOKkI6wZ"
#define BLYNK_TEMPLATE_NAME "Embebidos"
#define BLYNK_AUTH_TOKEN "GnAZfXTzJ9WAmYJFV6w6d64OOpRki0C3"


char ssid[] = "";
char pass[] = "";

// DHT11
#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Servo
Servo servoVentilacion;
#define SERVO_PIN 15

// Variables
int botonManual = 0;
int anguloServo = 0;

BlynkTimer timer;

// Control desde Blynk (botón)
BLYNK_WRITE(V2) {
  botonManual = param.asInt();
}

void enviarDatos() {
  float temperatura = dht.readTemperature();

  if (isnan(temperatura)) {
    Serial.println("Error DHT11");
    return;
  }

  Serial.print("Temperatura: ");
  Serial.println(temperatura);

  // Modo automático
  if (botonManual == 0) {
    if (temperatura > 30) {
      anguloServo = 90;
    } else {
      anguloServo = 0;
    }
  }
  // Modo manual
  else {
    anguloServo = 90;
  }

  servoVentilacion.write(anguloServo);

  // 📲 Enviar datos a Blynk
  Blynk.virtualWrite(V0, temperatura);
  Blynk.virtualWrite(V1, anguloServo);
}

void setup() {
  Serial.begin(115200);

  dht.begin();

  servoVentilacion.attach(SERVO_PIN);
  servoVentilacion.write(0);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  timer.setInterval(2000L, enviarDatos);
}

void loop() {
  Blynk.run();
  timer.run();
}