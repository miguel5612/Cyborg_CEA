/*
 * Cyborg CEA datalogger V1.0
 * Created for the robotic people competition
 * By Juanes / Lina / Santiago
 * Coach: IE.Miguel Califa
 * Hardware: Wemos D1 R1 & DHT & BMP180 & SD CARD
 * 
 */

 // Libraries
#include <SD.h>
#include <DHT.h>
#include <SPI.h>
#include <Wire.h>
#include <DHT_U.h>
#include <Arduino.h>
#include <SFE_BMP180.h>
#include <Adafruit_Sensor.h>

 // Definitions
#define DHTPIN D4
#define DHTTYPE    DHT11
#define I2C_ADDRESS 0x77
#define Po 1017
#define chipSelect D8

 // Object creations
SFE_BMP180 bmp180;

DHT_Unified dht(DHTPIN, DHTTYPE);

void setup() {
  // Serial Comm with PC
  Serial.begin(9600);
  
  // BMP180 Inits
  Wire.begin();
  
  //begin() initializes the interface, checks the sensor ID and reads the calibration parameters.  
  if (!bmp180.begin())
  {
    Serial.println("begin() failed. check your BMP180 Interface and I2C Address.");
    while (1);
  }

  // DHT Begin
  dht.begin();

  // SD Card Init
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
}

void loop() {
  // prints data
  float temp, hum, alt;
  temp = getTemp();
  hum = getHum();
  alt = getAltitude();

  Serial.println("Data: ");
  Serial.print(temp); Serial.println(" degC");
  Serial.print(hum); Serial.println(" % RH");
  Serial.print(alt); Serial.println(" Meters");

  File dataFile = SD.open("datalog.txt", FILE_WRITE);
  String dataString = "";
  dataString += String(temp) + ",";
  dataString += String(hum) + ",";
  dataString += String(alt);
  dataFile.println(dataString);
  dataFile.close();
    
  delay(200);
}

int getTemp()
{
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println(F("Error reading temperature!"));
  }
  else return event.temperature;
}

int getHum()
{
  sensors_event_t event;
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println(F("Error reading humidity!"));
  }
  else return event.relative_humidity;
}

float getAltitude()
{
  char status;
  double T, P, alt;
  bool success = false;

  status = bmp180.startTemperature();

  if (status != 0) {
    delay(1000);
    status = bmp180.getTemperature(T);

    if (status != 0) {
      status = bmp180.startPressure(3);

      if (status != 0) {
        delay(status);
        status = bmp180.getPressure(P, T);

        if (status != 0) {
          alt = bmp180.altitude(P, Po);

          //Serial.print("Altitude: ");
          //Serial.print(alt);
          //Serial.println(" Meters");
          return abs(alt);
        }
      }
    }
  }
}
