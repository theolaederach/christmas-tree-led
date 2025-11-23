
# Christmas tree guarland with an esp32 and neopixel

Hello, i made a code for an esp32 to configure your guarland for your Christmas tree.

It use an web interface connected to your wifi for configure the led. you just have to scan your wifi and connect with the ip to the web site.

## Installation

Clone the project on your computer :

```bash
  git clone https://github.com/theolaederach/christmas-tree-led.git
```
Before uploading the code into the esp32, change the variable below.

After the uploading, you can see the ip adress on the serial monitor or you can scan your wifi with an app like [Fing](https://www.fing.com/)


## Environment Variables

You need to change this variables to connect the esp to your wi-fi.


### Into the file `Christmas-tree-led.ino`

10 : `#define LED_PIN 6` change the 6 by your led pin.

11 : `#define NUM_LED` change by the number of led you have.

13 :`const char* ssid = "your_wifi_name";` change by the ssid of your wifi / the name.

14 : `const char* password = "Password";` change by the password of your wifi.

## Authors

- [@theolaederach](https://www.github.com/theolaederach)

