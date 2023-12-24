/*
  This example reads data from the additional AHT10 connected to Nano 33 BLE Lite and on-board LPS22HB sensor of the
  Nano 33 BLE Sense Lite and prints the temperature, pressure and humidity
  value to the Serial Monitor once a second.

  The circuit:
  - Arduino Nano 33 BLE Sense Lite
  - AHT10 Sensor

  This example code is not in the public domain.
*/

#include <Arduino_LPS22HB.h>
#include <Adafruit_AHTX0.h>
#include <weatherCalcLibraryForBLE33.h>

Adafruit_AHTX0 aht;
Adafruit_Sensor *aht_humidity;
weatherCalcForBLE33 weather;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  if (!BARO.begin()) {
    Serial.println("Failed to initialize pressure sensor!");
    while (1);
  }

  if (!aht.begin()) {
    Serial.println("Failed to find AHT10 chip");
    while (1) {
      delay(10);
    }
  }

  Serial.println("AHT10 Found!");

  aht_humidity = aht.getHumiditySensor();
  aht_humidity->printSensorDetails();
}

void loop() {
  // read the sensor value
  /* Display the result Pressure & Temperature (Temperature is measured in degree Celcius (C)  & Pressure is measured in Pascal (Pa)) */
  float pressure = BARO.readPressure();
  float temperature = BARO.readTemperature();
  Serial.println();
  // wait 1 second to print again

  /* Display the result Humidity (Humidity is measured in % relative humidity (% rH) */
  sensors_event_t humidity;
  aht_humidity->getEvent(&humidity);

  Serial.print("\t\tHumidity: ");
  float rel_humidity=humidity.relative_humidity;
  Serial.print(rel_humidity);
  Serial.println(" % rH");
  Serial.print("\t\tTemperature: ");
  Serial.print(temp.temperature);
  Serial.println(" degrees C");

  weather.setTemperature(temperature);
  weather.setHumidity(rel_humidity);
  weather.setPressure(pressure); 
  weather.setUserAltitude(500);

  delay(1000);
}
