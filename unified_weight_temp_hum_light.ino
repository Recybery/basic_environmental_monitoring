#include <Adafruit_Sensor.h>

// DHT Temperature & Humidity Sensor
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN            4         // Pin which is connected to the DHT sensor.
#define DHTTYPE           DHT11     // DHT 11 

DHT_Unified dht(DHTPIN, DHTTYPE);


//optic sensor
int photoCellPinL = A2; //1.09MOhm 
int photoCellReadingL;

int photoCellPinR = A1; //330Ohm
int photoCellReadingR;
int photoCellPinM = A0; //mixed signal
int photoCellReadingM;

//HX711 Load Cell
#include "HX711.h"

#define calibration_factor -21320.0 //This value is obtained using the SparkFun_HX711_Calibration sketch
#define DOUT  3
#define CLK  2

HX711 scale(DOUT, CLK);


//sampling parameters
uint32_t delayMS;
int burst = 0;
int replicate_number = 5; // how many replicates per burst
int rep_delay = 2; // how many delayMS units to wait between replicates
int burst_delay = 60; // how many seconds between measurements


void setup() {
  Serial.begin(9600); 
  // Initialize device.
  dht.begin();
  Serial.println("DHTxx Unified Sensor ");
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


  //initialize the scale
  scale.set_scale(calibration_factor); //This value is obtained by using the SparkFun_HX711_Calibration sketch
  scale.tare();	//Assuming there is no weight on the scale at start up, reset the scale to 0
  Serial.println("#------------------------------------");
  Serial.println("#Weight");
  Serial.print  ("#Sensor:       "); Serial.println("HX711");
  Serial.print  ("#Calibration factor:       "); Serial.println(calibration_factor);




  // Set delay between sensor readings based on sensor details.
  delayMS = sensor.min_delay / 1000;

  Serial.println("#------------------------------------");
  Serial.print("#burst_number replicate_number temp_C humidity_percent  L_photocell  R_photocell  M_photocell  mass_kg");
  Serial.println("#------------------------------------");
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

    Serial.print(scale.get_units(), 1); //scale.get_units() returns a float
    Serial.print("  ");


    Serial.println(" ");
    }
  burst += 1;
  delay(burst_delay);
 }

