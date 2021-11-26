# SAE-Aerodesign-LoRA-9dof
Long Range Radio communication system to transmit SAE plane flight data to an MCU w/ readable UI

![System Overview](https://github.com/Engineering-Applied/SAE-Aerodesign-LoRA-9dof/tree/main/media/images/System_Overview.HEIC)

## Objective
To allow a user to monitor pitch, yaw, and roll of a model RC plane real-time midflight

## Materials
* Adafruit Feather 32u4 LoRa enabled @ 915Mhz
* Adafruit Hallowing M0
* Adafruit LoRA Featherwing enabled @ 915Mhz
* Adafruit I2C 9dof sensor (FXOS8700 & FXAS21002)
* 2 Li-Po 3.7V 500mAh Rechargeable Batteries
* Soldering Iron *(optional, reflow oven preferred)*
* Lead-free Solder *(optional, solder paste preferred)*
* uFL SMT antenna connector
* RP-SMA to uFL RF adapter cable
* Edge Launch SMA connector for 1.6mm PCBs
* RP-SMA Jack to SMA Plug RF Adapter
* Two 915MHz LoRa Antenna RP-SMA - 1/2 Wave 2dBi
* Hot Glue Gun *(optional)*
* Electrical Tape *(optional)*

## Setup

#### For Receiver Module (Hallowing M0 w/ Featherwing)
1. Solder male header pins to LoRa Featherwing to match the Feather footprint
2. Solder jumper wire from pin "A" to pin "RST" in the middle of the board
3. Solder jumper wire from pin "B" to pin "CS" in the middle of the board
4. Solder jumper wire from pin "D" to pin "IRQ" in the middle of the board
5. Solder edge launch SMA connector to the RF copper surface pads on the edge of the board
6. Finally attach the featherwing to the Hallowing M0 Board
7. Screw on the RP-SMA Jack to SMA Plug RF Adapter to the SMA Edge Launch
8. Screw on one of the 915Mhz LoRa Antennas to the adapter

![Receiver Module Setup](https://github.com/Engineering-Applied/SAE-Aerodesign-LoRA-9dof/tree/main/media/images/Featherwing_JumperWires.HEIC)
![Receiver Module Schematic](https://github.com/Engineering-Applied/SAE-Aerodesign-LoRA-9dof/blob/main/media/images/Radio_Receive.png)

#### For Transmitting Module (Feather 32u4)
1. Carefully solder (reflow oven w/ Solder paste method preferred) uFL SMT antenna connector to the uFL copper surface pads
2. Solder I2C wires (3V, GND, SDA, & SCL) of desired length from the feather 32u4 board to their respective counterparts on the I2C 9dof sensor board
3. Attach RP-SMA to uFL RF adapter cable to the uFL SMT antenna connector
4. Hot glue the cable to the SMT antenna connector for secure connection (**To make sure the uFL connector does not get ripped off the pads**)
5. Tape the cable to the board for extra support (**Also to make sure the uFL Connector does not get ripped off the pads**)
6. Screw on the other 915Mhz LoRA Antenna to the other end of the RP-SMA to uFL cable

![Transmitter Module Setup](https://github.com/Engineering-Applied/SAE-Aerodesign-LoRA-9dof/tree/main/media/images/Fastened_RF_Cable.HEIC)
![Transmitter Module Schematic](https://github.com/Engineering-Applied/SAE-Aerodesign-LoRA-9dof/blob/main/media/images/Radio_Transmit.png)

## Programming
* Upload the Radio_Transmit code to the Feather 32u4 board
* Upload the Radio_Receive code to the Hallowing M0 board
* Verify functionality

*Must have SPI.h, Wire.h, RH_RF95.h, Adafruit_FXAS21002C.h, Adafruit_FXOS8700.h, Adafruit_Sensor.h, Adafruit_GFX.h, Adafruit_SPITFT.h, Adafruit_SPITFT_Macros.h, gfxfont.h, Adafruit_ST7735.h libraries installed*

## Testing
* Attach fully charged Li-Po batteries to each of the boards
* 9dof Data should appear on the Hallowing M0 TFT in almost real-time
