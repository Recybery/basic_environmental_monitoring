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

#define DHTPIN            2         // Pin which is connected to the DHT sensor.
//#define DHTPIN_A            2         // Pin which is connected to the DHT sensor.
//#define DHTPIN_B            4         // Pin which is connected to the DHT sensor.

#define DHTTYPE           DHT11     // DHT 11 

// See guide for details on sensor wiring and usage:
//   https://learn.adafruit.com/dht/overview

DHT_Unified dht(DHTPIN, DHTTYPE);

//DHT_Unified dht(DHTPIN_A, DHTTYPE);
//DHT_Unified dht(DHTPIN_B, DHTTYPE);

uint32_t delayMS;
int burst = 0;
int replicate_number = 5; // how many replicates per burst
int rep_delay = 2; // how many delayMS units to wait between replicates
int burst_delay = 60; // how many seconds between measurements

int photoCellPinL = 0; //100kOhm 
int photoCellReadingL;

int photoCellPinR = 1; //330Ohm
int photoCellReadingR;

int photoCellPinM = 0; //mixed signal
int photoCellReadingM;


void setup() {
  Serial.begin(9600); 
  // Initialize device.
  dht.begin();
  Serial.println("DHTxx Unified Sensor Example");
  // Print temperature sensor details.
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
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
  dht.humidity().getSensor(&sensor);
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
  Serial.println("#burst_number replicate_number temp_C humidity_percent lumin_L lumin_R lumin_M");
  Serial.println("#------------------------------------");

  delayMS = sensor.min_delay / 1000;
}

void loop() {


for (int counter = 0; counter < replicate_number; counter++){
  
  
  // Delay between replicates
  delay(delayMS*rep_delay);
  
  // Get temperature event and print its value.
  sensors_event_t event;  
  dht.temperature().getEvent(&event);
    Serial.print(burst);
    Serial.print("  ");

    Serial.print(counter);
    Serial.print("  ");
    
    Serial.print(event.temperature);
    Serial.print("  ");

    dht.humidity().getEvent(&event);
    Serial.print(event.relative_humidity);
    Serial.print("  ");
   
    photoCellReadingL = analogRead(photoCellPinL);
    Serial.print(photoCellReadingL);
    Serial.print("  ");
  
    photoCellReadingR = analogRead(photoCellPinR);
    Serial.print(photoCellReadingR);
    Serial.print("  ");

    photoCellReadingM = analogRead(photoCellPinM);
    Serial.print(photoCellReadingM);
    Serial.print("  ");


    Serial.println(" ");
    }
  burst += 1;
  delay(burst_delay*1000);
 }

