// Define Blynk template ID, name, and authentication token
#define BLYNK_TEMPLATE_ID ""
#define BLYNK_TEMPLATE_NAME "IoT"
#define BLYNK_AUTH_TOKEN ""

// Include necessary libraries
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

// Define Blynk-related variables
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "letsbuildit";  // Type your WiFi name
char pass[] = "letsbuildit";  // Type your WiFi password
BlynkTimer timer;

// Define pin connections
int gas = A0;
int sensorThreshold = 1;

#define DHTPIN D4          // Connect the out pin to D2 on NODE MCU
#define DHTTYPE DHT11      // Define the DHT sensor type
DHT dht(DHTPIN, DHTTYPE);

// Function to send sensor data to Blynk
void sendSensor()
{
  // Read temperature and humidity from DHT sensor
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  // Check if the DHT sensor reading is valid
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Read analog sensor value
  int analogSensor = analogRead(gas);

  // Send gas value to Blynk
  Blynk.virtualWrite(V2, analogSensor);
  Serial.print("Gas Value: ");
  Serial.println(analogSensor);

  // Send temperature and humidity values to Blynk
  Blynk.virtualWrite(V1, t);
  Blynk.virtualWrite(V0, h);

  // Print temperature and humidity to Serial Monitor
  Serial.print("Temperature : ");
  Serial.print(t);
  Serial.print("    Humidity : ");
  Serial.println(h);
}

void setup()
{
  Serial.begin(115200);

  // Initialize Blynk and DHT sensor
  Blynk.begin(auth, ssid, pass);
  dht.begin();

  // Set up a timer to periodically send sensor data
  timer.setInterval(100L, sendSensor);
}

void loop()
{
  // Run Blynk and timer functions
  Blynk.run();
  timer.run();

  // Read temperature, humidity, and gas value
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  int gasValue = analogRead(gas);

  // Print sensor readings to Serial Monitor
  Serial.print("h:");
  Serial.print(h);
  Serial.print("  t:");
  Serial.print(t);
  Serial.print("  g:");
  Serial.println(gasValue);

  // Check for air quality and print corresponding message
  if (gasValue < 600)
  {
    Serial.println("Fresh Air");
  }
  else if (gasValue > 600)
  {
    Serial.println("Bad Air");
  }

  // Send Blynk notification for bad air quality
  if (gasValue > 600)
  {
    Blynk.logEvent("pollution_alert", "Bad Air");
  }
}
