#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <SPI.h>
#include <Wire.h>
#include <SFE_BMP180.h>
#include <Adafruit_Sensor.h>


#define DHTPIN 2 
#define DHTTYPE    DHT11
#define I2C_ADDRESS 0x77
#define Po 1017

DHT_Unified dht(DHTPIN, DHTTYPE);
SFE_BMP180 bmp180;

uint32_t delayMS;


void setup() {
  Serial.begin(9600);
  //Inicializar DHT11.
  dht.begin();
  sensor_t sensor;
  Wire.begin();
  
  //begin() initializes the interface, checks the sensor ID and reads the calibration parameters.  
  if (!bmp180.begin())
  {
    Serial.println("begin() failed. check your BMP180 Interface and I2C Address.");
    while (1);
  }
  delayMS = sensor.min_delay / 1000;

}

void loop() {
  Serial.println (obtener_lecturas());
  delay(500);

}
String obtener_lecturas () {
  double temperatura=obtener_temperatura ();
  double humedad=obtener_humedad ();
  double altitud=obtener_altitud ();

  return String (temperatura) + "," + String (humedad) + "," + String (altitud);
  
}

double obtener_temperatura(){ 
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println(F("Error reading temperature!"));
    return 9999;
  }
  else {
    Serial.print(F("Temperature: "));
    Serial.print(event.temperature);
    Serial.println(F("°C"));
    return event.temperature;
  }
}
double obtener_humedad (){ 
  sensors_event_t event;
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println(F("Error reading humidity!"));
    return 9999;
  }
  else {
    Serial.print(F("Humidity: "));
    Serial.print(event.relative_humidity);
    Serial.println(F("%"));
    return event.relative_humidity; 
  }
}
double obtener_altitud()
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

          Serial.print("Altitude: ");
          Serial.print(alt);
          Serial.println(" Meters");
          return abs(alt);
        }
      }
    }
  }
}
