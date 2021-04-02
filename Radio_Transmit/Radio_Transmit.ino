/* Radio Transmit Code for LoRa 915MHz Module
 * Ported and edited for Feather 32u4 board
 * Created by: Joshua Harrell (UNF SAE AERO)
 * Modified: 11/26/20
 */
 
// --------------------------Adafruit Notes -------------------------------
/* Feather9x_TX
 -*- mode: C++ -*-
 Example sketch showing how to create a simple messaging client (transmitter)
 with the RH_RF95 class. RH_RF95 class does not provide for addressing or
 reliability, so you should only use RH_RF95 if you do not need the higher
 level messaging abilities.
 It is designed to work with the other example Feather9x_RX */

#include <SPI.h>
#include <RH_RF95.h>              //Includes Radio Head LoRa 915MHz library
#include <Adafruit_FXAS21002C.h>  //Includes Gyroscope Library
#include <Adafruit_FXOS8700.h>    //Includes Accelerometer & Magnetometer Library
#include <Adafruit_Sensor.h>
#include <Wire.h>

// for feather32u4 
#define RFM95_CS 8
#define RFM95_RST 4
#define RFM95_INT 7


/* for feather m0  
#define RFM95_CS 8
#define RFM95_RST 4
#define RFM95_INT 3
*/

/* for shield 
#define RFM95_CS 10
#define RFM95_RST 9
#define RFM95_INT 7
*/

/* Feather 32u4 w/wing
#define RFM95_RST     11   // "A"
#define RFM95_CS      10   // "B"
#define RFM95_INT     2    // "SDA" (only SDA/SCL/RX/TX have IRQ!)
*/

/* Feather m0 w/wing 
#define RFM95_RST     11   // "A"
#define RFM95_CS      10   // "B"
#define RFM95_INT     6    // "D"
*/

#if defined(ESP8266)
  /* for ESP w/featherwing */ 
  #define RFM95_CS  2    // "E"
  #define RFM95_RST 16   // "D"
  #define RFM95_INT 15   // "B"

#elif defined(ESP32)  
  /* ESP32 feather w/wing */
  #define RFM95_RST     27   // "A"
  #define RFM95_CS      33   // "B"
  #define RFM95_INT     12   //  next to A

#elif defined(NRF52)  
  /* nRF52832 feather w/wing */
  #define RFM95_RST     7   // "A"
  #define RFM95_CS      11   // "B"
  #define RFM95_INT     31   // "C"
  
#elif defined(TEENSYDUINO)
  /* Teensy 3.x w/wing */
  #define RFM95_RST     9   // "A"
  #define RFM95_CS      10   // "B"
  #define RFM95_INT     4    // "C"
#endif

// Change to 434.0 or other frequency, must match RX's freq!
#define RF95_FREQ 868.1

// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);

/* Assign a unique ID to this GYRO sensor at the same time */
Adafruit_FXAS21002C gyro = Adafruit_FXAS21002C(0x0021002C);

/* Assign a unique ID to this sensor at the same time */
Adafruit_FXOS8700 accelmag = Adafruit_FXOS8700(0x8700A, 0x8700B);

/* Displays All Sensor Details
 * Writes each sensor name, version, ID, and Resolution to Serial monitor
 * Returns nothing
 */
void displaySensorDetails(void) {
  sensor_t Gyro, accel, mag;    // Defines the sensor instances for each sensor
  gyro.getSensor(&Gyro);         // Sends GYRO data to address of "sensor"
  accelmag.getSensor(&accel, &mag); // Sends Accelerometer data to the address of "accel" and Magnetometer data to address of "mag"
  Serial.println("------------------------------------");
  Serial.print("Sensor:       ");
  Serial.println(Gyro.name);
  Serial.print("Driver Ver:   ");
  Serial.println(Gyro.version);
  Serial.print("Unique ID:    0x");
  Serial.println(Gyro.sensor_id, HEX);
  Serial.print("Max Value:    ");                //Prints Gyroscope info to Serial Monitor on Startup
  Serial.print(Gyro.max_value);                  //Waits 1 second to post next info
  Serial.println(" rad/s");
  Serial.print("Min Value:    ");
  Serial.print(Gyro.min_value);
  Serial.println(" rad/s");
  Serial.print("Resolution:   ");
  Serial.print(Gyro.resolution);
  Serial.println(" rad/s");
  Serial.println("------------------------------------");
  Serial.println("");
  delay(1000);

  Serial.println("------------------------------------");
  Serial.println("ACCELEROMETER");
  Serial.println("------------------------------------");
  Serial.print("Sensor:       ");
  Serial.println(accel.name);
  Serial.print("Driver Ver:   ");
  Serial.println(accel.version);
  Serial.print("Unique ID:    0x");              //Prints Accelerometer info to Serial Monitor on startup
  Serial.println(accel.sensor_id, HEX);          //Waits 1 second to post next info
  Serial.print("Min Delay:    ");
  Serial.print(accel.min_delay);
  Serial.println(" s");
  Serial.print("Max Value:    ");
  Serial.print(accel.max_value, 4);
  Serial.println(" m/s^2");
  Serial.print("Min Value:    ");
  Serial.print(accel.min_value, 4);
  Serial.println(" m/s^2");
  Serial.print("Resolution:   ");
  Serial.print(accel.resolution, 8);
  Serial.println(" m/s^2");
  Serial.println("------------------------------------");
  Serial.println("");
  delay(1000);

  Serial.println("------------------------------------");
  Serial.println("MAGNETOMETER");
  Serial.println("------------------------------------");
  Serial.print("Sensor:       ");
  Serial.println(mag.name);
  Serial.print("Driver Ver:   ");
  Serial.println(mag.version);
  Serial.print("Unique ID:    0x");
  Serial.println(mag.sensor_id, HEX);
  Serial.print("Min Delay:    ");                   //Prints Magnetometer Data to Serial Monitor on startup
  Serial.print(accel.min_delay);                    //Waits 1 second to start transmitting
  Serial.println(" s");
  Serial.print("Max Value:    ");
  Serial.print(mag.max_value);
  Serial.println(" uT");
  Serial.print("Min Value:    ");
  Serial.print(mag.min_value);
  Serial.println(" uT");
  Serial.print("Resolution:   ");
  Serial.print(mag.resolution);
  Serial.println(" uT");
  Serial.println("------------------------------------");
  Serial.println("");
  delay(1000);
}

/* This function verifies that the Receiver received the sensor data
 * Returns nothing but prints verfication to serial monitor
 */
void Acknowledgement(void)
{
  /* Wait for a reply */
  uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);

  Serial.println("Waiting for reply...");
  //rf95.waitAvailable();
  if (rf95.available())
  { 
    // Should be a reply message for us now   
    if (rf95.recv(buf, &len))
   {
      Serial.print("Got reply: ");
      Serial.println((char*)buf);
      Serial.print("RSSI: ");
      Serial.println(rf95.lastRssi(), DEC);    
    }
    else
    {
      Serial.println("Receive failed");
    }
  }
  else
  {
    Serial.println("No reply, is there a listener around?");
  }
}

/* This function turns any float value to 3 digit precision float
 * Returns new 3 digit precision float
 */
float PrecisionModifier(float data){
  int temp = data*100;
  float newData = temp/100;
  return newData;
}

void setup() 
{
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

  Serial.begin(9600);
//  while (!Serial) {
//    delay(1);
//  }

  delay(100);

  Serial.println("Feather LoRa TX Test!");

  // manual reset
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);

  while (!rf95.init()) {
    Serial.println("LoRa radio init failed");
    Serial.println("Uncomment '#define SERIAL_DEBUG' in RH_RF95.cpp for detailed debug info");
    while (1);
  }
  Serial.println("LoRa radio init OK!");

  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM
  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1);
  }
  Serial.print("Set Freq to: "); Serial.println(RF95_FREQ);
  
  // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on

  // The default transmitter power is 13dBm, using PA_BOOST.
  // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then 
  // you can set transmitter powers from 5 to 23 dBm:
  rf95.setTxPower(5, false);

Serial.println("");
Serial.println("Gyroscope Test");
Serial.println("");

  /* Initialise the GYRO sensor */
  if (!gyro.begin()) {
    /* There was a problem detecting the FXAS21002C ... check your connections
     */
    Serial.println("Ooops, no FXAS21002C detected ... Check your wiring!");
    while (1)
      ;
  }
  
Serial.println("");
Serial.println("FXOS8700 Test");
Serial.println("");

  /* Initialise the sensor */
  if (!accelmag.begin(ACCEL_RANGE_4G)) {
    /* There was a problem detecting the FXOS8700 ... check your connections */
    Serial.println("Ooops, no FXOS8700 detected ... Check your wiring!");
    while (1)
      ;
  }
  /* Display some basic information on the GYRO sensor */
  displaySensorDetails();
}

int16_t packetnum = 0;  // packet counter, we increment per xmission


void loop()
{
  /* Get the new GYRO, ACCEL, and MAG sensor events */
  sensors_event_t gevent, aevent, mevent;
  gyro.getEvent(&gevent);
  accelmag.getEvent(&aevent, &mevent);

  /* Save the measured values to variables with %0.3f precision */
  float xgyro = gevent.gyro.x;//PrecisionModifier(gevent.gyro.x);
  float ygyro = gevent.gyro.y;//PrecisionModifier(gevent.gyro.y);
  float zgyro = gevent.gyro.z;//PrecisionModifier(gevent.gyro.z);
  float xaccel = aevent.acceleration.x;//PrecisionModifier(aevent.acceleration.x);
  float yaccel = aevent.acceleration.y;//PrecisionModifier(aevent.acceleration.y);
  float zaccel = aevent.acceleration.z;//PrecisionModifier(aevent.acceleration.z);
  float xmag = mevent.magnetic.x;//PrecisionModifier(mevent.magnetic.x);
  float ymag = mevent.magnetic.y;//PrecisionModifier(mevent.magnetic.y);
  float zmag = mevent.magnetic.z;//PrecisionModifier(mevent.magnetic.z);
  
  /* Display the GYRO results (speed is measured in rad/s) */
  Serial.print("X: ");
  Serial.print(gevent.gyro.x);
  Serial.print("  ");
  Serial.print("Y: ");
  Serial.print(gevent.gyro.y);
  Serial.print("  ");
  Serial.print("Z: ");
  Serial.print(gevent.gyro.z);
  Serial.print("  ");
  Serial.println("rad/s ");

/* Display the ACCEL results (acceleration is measured in m/s^2) */
  Serial.print("A ");
  Serial.print("X: ");
  Serial.print(aevent.acceleration.x, 4);
  Serial.print("  ");
  Serial.print("Y: ");
  Serial.print(aevent.acceleration.y, 4);
  Serial.print("  ");
  Serial.print("Z: ");
  Serial.print(aevent.acceleration.z, 4);
  Serial.print("  ");
  Serial.println("m/s^2");

  /* Display the MAG results (mag data is in uTesla) */
  Serial.print("M ");
  Serial.print("X: ");
  Serial.print(mevent.magnetic.x, 1);
  Serial.print("  ");
  Serial.print("Y: ");
  Serial.print(mevent.magnetic.y, 1);
  Serial.print("  ");
  Serial.print("Z: ");
  Serial.print(mevent.magnetic.z, 1);
  Serial.print("  ");
  Serial.println("uT");

  Serial.println("");
  
  //delay(300); // Wait 1 second between transmits, could also 'sleep' here!
  Serial.println("Transmitting..."); // Send a message to rf95_server

  /* Create a character array of format X,###.###,###.###,###.###   e.g A,xgyro,ygyro,zgyro
   * On receiving end, make array of parsed strings
   */
  String temp1;
  temp1 += F("G: ");
  temp1 += String(xgyro, 2); //The second parameter shows 2 decimal points (4 characters)
  temp1 += F(",");
  temp1 += String(ygyro, 2);
  temp1 += F(",");
  temp1 += String(zgyro, 2);
  int temp1_len = temp1.length()+1;
  char radiopacket1[temp1_len];
  temp1.toCharArray(radiopacket1, temp1_len);

  String temp2;
  temp2 += F("A: ");
  temp2 += String(xaccel, 2); //The second parameter shows 2 decimal points (4 or 5 characters)
  temp2 += F(",");
  temp2 += String(yaccel, 2);
  temp2 += F(",");
  temp2 += String(zaccel, 2);
  int temp2_len = temp2.length()+1;
  char radiopacket2[temp2_len];
  temp2.toCharArray(radiopacket2, temp2_len);

  String temp3;
  temp3 += F("M: ");
  temp3 += String(xmag, 1);  //The second parameter shows 1 decimal points (4 characters)
  temp3 += F(",");
  temp3 += String(ymag, 1);
  temp3 += F(",");
  temp3 += String(zmag, 1);
  int temp3_len = temp3.length()+1;
  char radiopacket3[temp3_len];
  temp3.toCharArray(radiopacket3, temp3_len);

 /* Send the Message 1 */         
  Serial.print("Sending "); Serial.println(radiopacket1);
  Serial.println("Sending...");
  delay(10);
  rf95.send((uint8_t *)radiopacket1, sizeof(radiopacket1)); 
  rf95.waitPacketSent();
  Serial.println("Waiting for packet to complete...");
  Acknowledgement();
  delay(300);       //Wait 400 msecond before transmitting
  
  /* Send the Message 2 */         
  Serial.print("Sending "); Serial.println(radiopacket2);
  Serial.println("Sending...");
  delay(10);
  rf95.send((uint8_t *)radiopacket2, sizeof(radiopacket2)); 
  rf95.waitPacketSent();
  Serial.println("Waiting for packet to complete...");
  Acknowledgement();
  delay(300);       //Wait 400 msecond before transmitting
  
  /* Send the Message 3 */         
  Serial.print("Sending "); Serial.println(radiopacket3);
  Serial.println("Sending...");
  delay(10);
  rf95.send((uint8_t *)radiopacket3, sizeof(radiopacket3)); 
  rf95.waitPacketSent();
  Serial.println("Waiting for packet to complete...");
  Acknowledgement();
  delay(300);      //Wait 400 msecond before transmitting
}
