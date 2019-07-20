



//borrowed from https://github.com/adafruit/Light-and-Temp-logger/blob/master/lighttemplogger.ino

#include <Adafruit_Sensor.h>
#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include "RTClib.h"
#include <DHT.h>
#include <DHT_U.h>
#include <LowPower.h>
#include "HX711.h"



// A simple data logger for the Arduino analog pins

//  define time constants
const unsigned long SECOND = 1000;
const unsigned long HOUR = 3600*SECOND;


// how many milliseconds between grabbing data and logging it. 1000 ms is once a second
#define LOG_INTERVAL  1000 // mills between entries (reduce to take more/faster data)
// equivalent to burst/rep delay

int rep_delay= 1*SECOND;
int rep_number = 5;
//int burst_delay = 0;
int burst_delay = 38; //how many 8second sleeps to take between bursts
int burst = 0;




// how many milliseconds before writing the logged data permanently to disk
// set it to the LOG_INTERVAL to write each time (safest)
// set it to 10*LOG_INTERVAL to write all data every 10 datareads, you could lose up to 
// the last 10 reads if power is lost but it uses less power and is much faster!
#define SYNC_INTERVAL 1000 // mills between calls to flush() - to write data to the card
uint32_t syncTime = 0; // time of last sync()
////  rewrite this in terms of bursts/reps?




#define ECHO_TO_SERIAL   1 // echo data to serial port
#define WAIT_TO_START    0 // Wait for serial input in setup()

// the digital pins that connect to the LEDs
#define redLEDpin 2
#define greenLEDpin 3

// The analog pins that connect to the sensors
//optic sensor
int photoCellPin = A2; 
int photoCellReading = 0;


//  //  DHT Humidity + Temp
// See guide for details on sensor wiring and usage:
//   https://learn.adafruit.com/dht/overview
#define DHTPIN_A            6        // Pin which is connected to the DHT sensor.
#define DHTPIN_B            7         // Pin which is connected to the DHT sensor.
#define DHTTYPE           DHT11     // DHT 11 
DHT_Unified dhtA(DHTPIN_A, DHTTYPE);
DHT_Unified dhtB(DHTPIN_B, DHTTYPE);

//	Scale setup

#define calibration_factor  -20970.0 //This value is obtained using the SparkFun_HX711_Calibration sketch
#define DOUT  4
#define CLK  5

HX711 scale;

// define the Real Time Clock object
RTC_PCF8523 RTC;

// for the data logging shield, we use digital pin 10 for the SD cs line
const int chipSelect = 10;

// the logging file
File logfile;


void error(char *str)
{
  Serial.print("error: ");
  Serial.println(str);
  
  // red LED indicates error
  digitalWrite(redLEDpin, HIGH);

  while(1);
}







void setup(void)
{
  Serial.begin(9600);
  Serial.println();
  
  // use debugging LEDs
  pinMode(redLEDpin, OUTPUT);
  pinMode(greenLEDpin, OUTPUT);
  
#if WAIT_TO_START
  Serial.println("Type any character to start");
  while (!Serial.available());
#endif //WAIT_TO_START

  // initialize the SD card
  Serial.print("Initializing SD card...");
  // make sure that the default chip select pin is set to
  // output, even if you don't use it:
  pinMode(10, OUTPUT);
  

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    error("Card failed, or not present");
  }
  Serial.println("card initialized.");
  
  // create a new file
  char filename[] = "LOGGER00.CSV";
  for (uint8_t i = 0; i < 100; i++) {
    filename[6] = i/10 + '0';
    filename[7] = i%10 + '0';
    if (! SD.exists(filename)) {
      // only open a new file if it doesn't exist
      logfile = SD.open(filename, FILE_WRITE); 
      break;  // leave the loop!
    }
  }
  
  if (! logfile) {
    error("couldnt create file");
  }
  Serial.print("Logging to: ");
  Serial.println(filename);

  // connect to RTC
  Wire.begin();  
  if (!RTC.begin()) {
    logfile.println("RTC failed");
#if ECHO_TO_SERIAL
    Serial.println("RTC failed");
#endif  //ECHO_TO_SERIAL
  }
  

  logfile.println("burst\trep\ttimestamp\tlumen\ttempA\trelhumA\ttempB\trelhumB");    
#if ECHO_TO_SERIAL
  Serial.println("burst\trep\ttimestamp\tlumen\ttempA\trelhumA\ttempB\trelhumB");
#endif //ECHO_TO_SERIAL
 
	dhtA.begin();
	dhtB.begin();
 	sensor_t sensor;
    
  //initialize the scale
  scale.begin(DOUT, CLK);
  scale.set_scale(calibration_factor); //This value is obtained by using the SparkFun_HX711_Calibration sketch
  scale.tare();	//Assuming there is no weight on the scale at start up, reset the scale to 0

}







void loop(void)
{

  
  digitalWrite(greenLEDpin, HIGH);

  DateTime now;

  for (int rep=0; rep <= rep_number; rep++){

    logfile.print(burst, DEC);
    logfile.print("\t");
    #if ECHO_TO_SERIAL
      Serial.print(burst, DEC);
      Serial.print("\t");
    #endif

   
    logfile.print(rep, DEC);
    logfile.print("\t");
    #if ECHO_TO_SERIAL
      Serial.print(rep, DEC);
      Serial.print("\t");
    #endif

   
     // fetch the time
    now = RTC.now();


    // log time
    logfile.print(now.day(), DEC);
    logfile.print("/");
    logfile.print(now.month(), DEC);
    logfile.print("/");
    logfile.print(now.year(), DEC);
    logfile.print("-");
    if(now.hour() < 10){
    logfile.print("0");
    }
    logfile.print(now.hour());
    logfile.print(":");
    if(now.minute() < 10){
    logfile.print("0");
    }
    logfile.print(now.minute());
    logfile.print(":");
    if(now.second() < 10){
    logfile.print("0");
    }
    logfile.print(now.second());
    #if ECHO_TO_SERIAL
      Serial.print(now.day(), DEC);
      Serial.print("/");
      Serial.print(now.month(), DEC);
      Serial.print("/");
      Serial.print(now.year(), DEC);
      Serial.print("-");
    if(now.hour() < 10){
    Serial.print("0");
    }
    Serial.print(now.hour());
    Serial.print(":");
    if(now.minute() < 10){
    Serial.print("0");
    }
    Serial.print(now.minute());
    Serial.print(":");
    if(now.second() < 10){
    Serial.print("0");
    }
    Serial.print(now.second());
    #endif //ECHO_TO_SERIAL

    delay(10); 
    int photoCellReading = analogRead(photoCellPin);  


    delay(10); 
    sensors_event_t event;  
  
    dhtA.temperature().getEvent(&event);

    logfile.print("\t");    
    logfile.print(photoCellReading);
    logfile.print("\t");    
    logfile.print(event.temperature);
  #if ECHO_TO_SERIAL
    Serial.print("\t");   
    Serial.print(photoCellReading);
    Serial.print("\t");    
    Serial.print(event.temperature, 1);
  #endif //ECHO_TO_SERIAL

  delay(10); 
  dhtA.humidity().getEvent(&event);

  logfile.print("\t");    
  logfile.print(event.relative_humidity);
  #if ECHO_TO_SERIAL
    Serial.print("\t");    
    Serial.print(event.relative_humidity, 1);
  #endif //ECHO_TO_SERIAL

  delay(10); 
    dhtB.temperature().getEvent(&event);

    logfile.print("\t");    
    logfile.print(event.temperature);
  #if ECHO_TO_SERIAL
    Serial.print("\t");    
    Serial.print(event.temperature, 1);
  #endif //ECHO_TO_SERIAL

  delay(10); 

  dhtB.humidity().getEvent(&event);

  logfile.print("\t");    
  logfile.print(event.relative_humidity);
  #if ECHO_TO_SERIAL
    Serial.print("\t");    
    Serial.print(event.relative_humidity, 1);
  #endif //ECHO_TO_SERIAL

  logfile.print("\t");  
  logfile.print(scale.get_units(), 1); //scale.get_units() returns a float
  
  #if ECHO_TO_SERIAL
    Serial.print("\t");    

    Serial.print(scale.get_units(), 1); //scale.get_units() returns a float
  #endif

  logfile.println();
  #if ECHO_TO_SERIAL
    Serial.println();
  #endif // ECHO_TO_SERIAL

  delay(rep_delay);
  }
  
  
  
    digitalWrite(greenLEDpin, LOW);

  // Now we write data to disk! Don't sync too often - requires 2048 bytes of I/O to SD card
  // which uses a bunch of power and takes time
  if ((millis() - syncTime) < SYNC_INTERVAL) return;
  syncTime = millis();
  
  // blink LED to show we are syncing data to the card & updating FAT!
  digitalWrite(redLEDpin, HIGH);
  logfile.flush();
  digitalWrite(redLEDpin, LOW);
  
  burst +=1;

  // delay for the amount of time we want between readings
  //delay((LOG_INTERVAL -1) - (millis() % LOG_INTERVAL));
  for (int j = 0; j<burst_delay; j+=1){
    //df LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
    //LowPower.idle(SLEEP_8S, ADC_OFF, TIMER5_OFF, TIMER4_OFF, TIMER3_OFF, TIMER2_OFF, TIMER1_OFF, TIMER0_OFF, SPI_OFF, USART3_OFF, USART2_OFF, USART1_OFF, USART0_OFF, TWI_OFF);
  LowPower.idle(SLEEP_8S, ADC_OFF, TIMER2_OFF, TIMER1_OFF, TIMER0_OFF, SPI_OFF, USART0_OFF, TWI_OFF);
  }



}
