// DHT Temperature & Humidity Sensor
// Unified Sensor Library Example
// Written by Tony DiCola for Adafruit Industries
// Released under an MIT license.

// Depends on the following Arduino libraries:
// - Adafruit Unified Sensor Library: https://github.com/adafruit/Adafruit_Sensor
// - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

//#define DHTPIN            2         // Pin which is connected to the DHT sensor.
#define DHTPIN_A            2         // Pin which is connected to the DHT sensor.
#define DHTPIN_B            4         // Pin which is connected to the DHT sensor.

#define DHTTYPE           DHT11     // DHT 11 

// See guide for details on sensor wiring and usage:
//   https://learn.adafruit.com/dht/overview

//DHT_Unified dht(DHTPIN, DHTTYPE);

DHT_Unified dhtA(DHTPIN_A, DHTTYPE);
DHT_Unified dhtB(DHTPIN_B, DHTTYPE);

uint32_t delayMS;
uint32_t delayMS_A;
uint32_t delayMS_B;
int burst = 0;
int replicate_number = 5; // how many replicates per burst
int rep_delay = 2; // how many delayMS units to wait between replicates
int burst_delay = 10; // how many seconds between measurements


void setup() {
  Serial.begin(9600); 
  Serial.println("#DHTxx Unified Sensor Example");

  dhtA.begin();
  // Initialize device.
  sensor_t sensor;
  // Print temperature sensor details.
  Serial.println("#DHT11 Sensor A");
  dhtA.temperature().getSensor(&sensor);
  Serial.println("#------------------------------------");
  Serial.println("#Temperature");
  Serial.print  ("#Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("#Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("#Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("#Max Value:    "); Serial.print(sensor.max_value); Serial.println(" *C");
  Serial.print  ("#Min Value:    "); Serial.print(sensor.min_value); Serial.println(" *C");
  Serial.print  ("#Resolution:   "); Serial.print(sensor.resolution); Serial.println(" *C");  
  Serial.println("#------------------------------------");
  // Print humidity sensor details.
  dhtA.humidity().getSensor(&sensor);
  Serial.println("#------------------------------------");
  Serial.println("#Humidity");
  Serial.print  ("#Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("#Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("#Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("#Max Value:    "); Serial.print(sensor.max_value); Serial.println("%");
  Serial.print  ("#Min Value:    "); Serial.print(sensor.min_value); Serial.println("%");
  Serial.print  ("#Resolution:   "); Serial.print(sensor.resolution); Serial.println("%");  
  Serial.println("#------------------------------------");
  // Set delay between sensor readings based on sensor details.
  Serial.println("#------------------------------------");
  Serial.println("#sensor  burst_number replicate_number temp_C humidity_percent");
  Serial.println("#------------------------------------");
  delayMS_A = sensor.min_delay / 1000;

  // Initialize device.
  dhtB.begin();
  Serial.println("#DHT11 Sensor B");
  // Print temperature sensor details.
//  sensor_t sensor;
  dhtB.temperature().getSensor(&sensor);
  Serial.println("#------------------------------------");
  Serial.println("#Temperature");
  Serial.print  ("#Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("#Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("#Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("#Max Value:    "); Serial.print(sensor.max_value); Serial.println(" *C");
  Serial.print  ("#Min Value:    "); Serial.print(sensor.min_value); Serial.println(" *C");
  Serial.print  ("#Resolution:   "); Serial.print(sensor.resolution); Serial.println(" *C");  
  Serial.println("#------------------------------------");
  // Print humidity sensor details.
  dhtB.humidity().getSensor(&sensor);
  Serial.println("#------------------------------------");
  Serial.println("#Humidity");
  Serial.print  ("#Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("#Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("#Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("#Max Value:    "); Serial.print(sensor.max_value); Serial.println("%");
  Serial.print  ("#Min Value:    "); Serial.print(sensor.min_value); Serial.println("%");
  Serial.print  ("#Resolution:   "); Serial.print(sensor.resolution); Serial.println("%");  
  Serial.println("#------------------------------------");
  // Set delay between sensor readings based on sensor details.
  Serial.println("#------------------------------------");
  Serial.print("#sensor  burst_number replicate_number temp_C humidity_percent\n");
  Serial.println("#------------------------------------");
  delayMS_B = sensor.min_delay / 1000;

  delayMS=max(delayMS_A,delayMS_B);
}

void loop() {

for (int counter = 0; counter < replicate_number; counter++){
  // Delay between replicates
  delay(delayMS*rep_delay);
  // Get temperature event and print its value.
    sensors_event_t event;  
    dhtA.temperature().getEvent(&event);
    Serial.print("\t");
    Serial.print("A");
    Serial.print("\t");
    Serial.print(burst);
    Serial.print("\t");
    Serial.print(counter);
    Serial.print("\t");
    Serial.print(event.temperature);
    Serial.print("\t");
    dhtA.humidity().getEvent(&event);
    Serial.println(event.relative_humidity);

    dhtB.temperature().getEvent(&event);
    Serial.print("\t");
    Serial.print("B");
    Serial.print("\t");
    Serial.print(burst);
    Serial.print("\t");
    Serial.print(counter);
    Serial.print("\t");
    Serial.print(event.temperature);
    Serial.print("\t");
    dhtB.humidity().getEvent(&event);
    Serial.println(event.relative_humidity);
   }

  burst += 1;
  delay(burst_delay*1000);

 }

