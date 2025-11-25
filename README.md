# ESP32 NeoPixel Christmas Tree

This project uses an ESP32 to control a NeoPixel (WS2812B) LED strip for your Christmas tree.

It works by creating its own Wi-Fi Access Point (AP). You connect your phone or computer directly to the ESP32's Wi-Fi, and then use a web interface to control the lights.

## 🎄 Features

* Turn LEDs On/Off
* Static Color Picker
* Multiple Effects (Rainbow, Disco, Cicle, Sparkling, and a "Mixed" sequence)
* Brightness Control
* Manual Time-Based Scheduler (Set a "current time", "start time", and "end time")

## 🚀 Getting Started

### 1. Installation

Clone the project to your computer:
``` bash
git clone https://github.com/theolaederach/christmas-tree-led.git
```
Open the `christmas-tree-led.ino`file in your Arduino IDE.

### 2. Configuration

Before uploading, you must change the following variables at the top of the `christmas-tree-led.ino` file to match your hardware:
``` C++
#define LED_PIN     6       // The ESP32 pin connected to your LED strip's Data-In
#define NUM_LEDS    150     // The total number of LEDs on your strip

const char* ssid = "LED_CONTROL";           //the SSID of the access point
const char* password = "Pa$$w0rd";          //the password of the access point
```
#### 3. Upload
1. Make sure you have the FastLED library installed in your Arduino IDE.

2. Select your ESP32 board (e.g., "ESP32C3 Dev Module").

3. Upload the code to your ESP32.

## 💡 How to Use
1. After the ESP32 has booted, open the Wi-Fi settings on your phone or computer.

2. Scan for new networks and connect to the one named "LED_CONTROL".

3. Use the password "mot2passe" to connect.

4. Once connected, open a web browser and go to the following address: [http://192.168.4.1](http://192.168.4.1)

You should now see the web interface and can start controlling your Christmas tree lights!

## 🧑‍💻 Author
[@theolaederach](https://github.com/theolaederach/)