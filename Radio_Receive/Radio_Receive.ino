/* Radio Reception Code for 9-DOF Sensor
 * Joshua Harrell (UNF SAE AERO)
 * Modified: 12/18/20
 */
 /*                   Adafruit Notes                */
// Feather9x_RX
// -*- mode: C++ -*-
// Example sketch showing how to create a simple messaging client (receiver)
// with the RH_RF95 class. RH_RF95 class does not provide for addressing or
// reliability, so you should only use RH_RF95 if you do not need the higher
// level messaging abilities.
// It is designed to work with the other example Feather9x_TX
 
#include <SPI.h>
#include <RH_RF95.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SPITFT.h>
#include <Adafruit_SPITFT_Macros.h>
#include <gfxfont.h>
#include <Adafruit_ST7735.h>

 
/* for Feather32u4 RFM9x
#define RFM95_CS 8
#define RFM95_RST 4
#define RFM95_INT 7
*/
 
/*for feather m0 RFM9x
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
 
//  Feather m0 w/wing 
#define RFM95_RST     11   // "A"
#define RFM95_CS      10   // "B"
#define RFM95_INT     6    // "D"

 
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

// These are 'hard wired' for Onboard TFT LCD
#define TFT_CS     39
#define TFT_RST    37
#define TFT_DC     38
#define TFT_BACKLIGHT 7
 
// Change to 434.0 or other frequency, must match RX's freq!
#define RF95_FREQ 868.1
 
// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);

// Create TFT object named tft
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
 
// Blinky on receipt
#define LED 13

//Create sensor variables
char xaccel[5];
char yaccel[5];
char zaccel[5];
char xgyro[5];
char ygyro[5];
char zgyro[5];
char xmag[2];
char ymag[2];
char zmag[2];


void setup()
{
  pinMode(LED, OUTPUT);
  pinMode(RFM95_RST, OUTPUT);
  pinMode(RFM95_CS, OUTPUT);
  tft.initR(INITR_HALLOWING);        // Initialize HalloWing-oriented screen
  pinMode(TFT_BACKLIGHT, OUTPUT);
  pinMode(TFT_CS, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);
  analogWrite(TFT_BACKLIGHT, 255);
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
  tft.setTextSize(1);
  tft.setCursor(0, 0);
  tft.setTextWrap(true);
  tft.println("Here is your data:");
  digitalWrite(TFT_CS, HIGH);
 
  Serial.begin(9600);
//  while (!Serial) {
//    delay(1);
//  }
  delay(100);
 
  Serial.println("Feather LoRa RX Test!");
 
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
}
 
void loop()
{
  //rf95.waitAvailable();
  if (rf95.available())
  {
    // Should be a message for us now
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);
 
    if (rf95.recv(buf, &len))
    {
      digitalWrite(LED, HIGH);
     // RH_RF95::printBuffer("Received: ", buf, len);// Prints Hex code to Serial Monitor
      Serial.print("Got: ");
      Serial.println((char*)buf);
      Serial.print("RSSI: ");
      Serial.println(rf95.lastRssi(), DEC);
      char first = buf[0];
      digitalWrite(RFM95_CS, HIGH);
      delay(10);
      digitalWrite(TFT_CS, LOW);
      delay(10);
      
      switch(first){
        case('G'):
        {
         tft.setCursor(0, 20);
         tft.print((char*)buf);          
         tft.println(" rad/s");
         digitalWrite(TFT_CS, HIGH);
         delay(10);
         digitalWrite(RFM95_CS, LOW);
         break; 
        }
        
        case('A'):
        {
          tft.setCursor(0, 40);
          tft.print((char*)buf);
          tft.println(" m/s^2");
          digitalWrite(TFT_CS, HIGH);
          delay(10);
          digitalWrite(RFM95_CS, LOW);
          break; 
        }
        
        case('M'):
        {
          tft.setCursor(0, 60);
          tft.print((char*)buf);
          tft.println(" uT");
          digitalWrite(TFT_CS, HIGH);
          delay(10);
          digitalWrite(RFM95_CS, LOW);
          break; 
        } 
        default:
        {
          Serial.println("Unknown");
          digitalWrite(TFT_CS, HIGH);
          delay(10);
          digitalWrite(RFM95_CS, LOW);
          break;
        }
      }
      // manual reset
       digitalWrite(RFM95_RST, LOW);
       delay(10);
       digitalWrite(RFM95_RST, HIGH);
       delay(10);
      rf95.init();
      delay(5);
      rf95.setFrequency(RF95_FREQ);
      delay(5);
      // Send a reply
      uint8_t data[] = "Got it";
      rf95.send(data, sizeof(data));
      rf95.waitPacketSent();
      Serial.println("Sent a reply");
      digitalWrite(LED, LOW);
    }
    else
    {
      Serial.println("Receive failed");
    }
  }
  else
  {
//    Serial.println("I received nothing");
//    delay(1);
  }
  //delay(400);
}
