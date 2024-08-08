#define BLYNK_TEMPLATE_ID "YourTemplateID"  // Replace with your Blynk Template ID
#define BLYNK_DEVICE_NAME "YourDeviceName"  // Replace with your Blynk Device Name
#define BLYNK_AUTH_TOKEN "YourAuthToken"    // Replace with your Blynk Auth Token

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = BLYNK_AUTH_TOKEN;  // Your Blynk Auth Token
char ssid[] = "YourNetworkName"; // Your Wi-Fi Network Name
char pass[] = "YourNetworkPass"; // Your Wi-Fi Password

// Define Virtual Pins
#define V0 V0 // Virtual Pin for Motor Speed
#define V1 V1 // Virtual Pin for Motor Direction
#define V2 V2 // Virtual Pin for Motor Start/Stop

// UART Serial Communication with ATmega32P
#define RX_PIN D2 // Define the pin for receiving data from ATmega32P
#define TX_PIN D1 // Define the pin for sending data to ATmega32P

BlynkTimer timer;

void setup()
{
  Serial.begin(9600);       // Set baud rate for UART communication
  Blynk.begin(auth, ssid, pass);
}

void loop()
{
  Blynk.run();
  timer.run();              // Run Blynk timer
}

// Function to send motor speed to ATmega32P
BLYNK_WRITE(V0)
{
  int motorSpeed = param.asInt();   // Get motor speed from Blynk app
  Serial.write(motorSpeed);         // Send motor speed to ATmega32P via UART
}

// Function to send motor direction to ATmega32P
BLYNK_WRITE(V1)
{
  int motorDirection = param.asInt();   // Get motor direction from Blynk app
  Serial.write(motorDirection);         // Send motor direction to ATmega32P via UART
}

// Function to send motor start/stop command to ATmega32P
BLYNK_WRITE(V2)
{
  int motorStatus = param.asInt();   // Get motor start/stop status from Blynk app
  Serial.write(motorStatus);         // Send motor status to ATmega32P via UART
}
