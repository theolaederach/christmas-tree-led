#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <FastLED.h>
#include <EEPROM.h>

#include "index.h"
//Please, change the settings below

#define LED_PIN     6                       // pin for the led
#define NUM_LEDS    150                     // number of led

const char* ssid = "your_wifi_name";        // your wifi ssid
const char* password = "Password";          // your wifi password





CRGB leds[NUM_LEDS];
WebServer server(80);

int on = true;
int r = 255, g = 0, b = 0;
int effect = 0;
int hue = 0;

void handleRoot();
void handleOnOff();
void handleSet_color();
void handleEffect_rainbow();
void handleEffect_snow();
void handleEffect_disco();
void handleEffect_cicle();
void handleEffect_sparkling();
void handleEffect_mixed();
void handleSet_scheduler();
void handleNotFound();

CRGB colorDisco1 = CRGB::Purple;
CRGB colorDisco2 = CRGB::Cyan;
CRGB colorDisco3 = CRGB::Lime;
CRGB colorSnow = CRGB::SkyBlue;



void setup() {

  Serial.begin(115200);
  FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(100);

  fill_solid(leds, NUM_LEDS, CRGB(r, g, b));
  FastLED.show();
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
        delay(500);
       Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected !");
    Serial.print("IP Address: ");

    Serial.println(WiFi.localIP()); //show the IP to access the control panel


    server.on("/", handleRoot);
    server.on("/onoff", handleOnOff);
    server.on("/setColor", handleSet_color);
    server.on("/setEffect/rainbow", handleEffect_rainbow);
    server.on("/setEffect/snow", handleEffect_snow);
    server.on("/setEffect/disco", handleEffect_disco);
    server.on("/setEffect/cicle", handleEffect_cicle);
    server.on("/setEffect/sparkling", handleEffect_sparkling);
    server.on("/setEffect/mixed", handleEffect_mixed);
    server.on("/setScheduler", handleSet_scheduler);
    server.onNotFound(handleNotFound);
    // removed stray "effect" token that caused a compile error
    server.begin();
    Serial.println("HTTP server started"); 

}

void loop() {
    server.handleClient();

    if (on) {
      if (effect == 0) { // Static
            fill_solid(leds, NUM_LEDS, CRGB(r, g, b));
        } else if (effect == 1) { // Rainbow
            static uint8_t hue = 0;
            fill_rainbow(leds, NUM_LEDS, hue, 7);
            hue++;
        } else if (effect == 2) { // Snow
            
        } else if (effect == 3) { // Disco
            static uint8_t hue = 0;
            if (hue == 0) {
                fill_solid(leds, NUM_LEDS, colorDisco1);
            } else if (hue == 1){
                fill_solid(leds, NUM_LEDS, colorDisco2);
            } else if (hue == 2) {
                fill_solid(leds, NUM_LEDS, colorDisco3);
            }
            hue = (hue + 1) % 3;
        } else if (effect == 4){
            static uint8_t hue = 0;
            fadeToBlackBy(leds, NUM_LEDS, 20);
            int pos = beatsin16(10, 0, NUM_LEDS - 1);
            leds[pos] = CRGB(r, g, b);
        } else if (effect == 5) { // Sparkling
            fadeToBlackBy(leds, NUM_LEDS, 20);
            int pos = random16(NUM_LEDS);
            leds[pos] += CRGB::White;
        } else if (effect == 6) { // Mixed
            // Combine rainbow and sparkling
            static uint8_t hue = 0;
            fill_rainbow(leds, NUM_LEDS, hue, 7);
            hue++;
            int pos = random16(NUM_LEDS);
            leds[pos] += CRGB::White;
        } 
    } else {
        fill_solid(leds, NUM_LEDS, CRGB::Black);
    }
    
  FastLED.setBrightness(100);
  FastLED.show();
}


void handleRoot() {
    server.send(200, "text/html", index_html);
}

void handleOnOff() {
    on = !on;
    server.sendHeader("Location", "/");
    server.send(303);
}


void handleScheduler() {
    // Implementation for scheduler handling
    server.sendHeader("Location", "/");
    server.send(303);
}
void handleMixed() {
    effect = 6; // Mixed
    server.sendHeader("Location", "/");
    server.send(303);
}
void handleSet_color() {
    if (server.hasArg("color")) {
        String colorHex = server.arg("color").substring(1);
        r = strtoul(colorHex.substring(0, 2).c_str(), NULL, 16);
        g = strtoul(colorHex.substring(2, 4).c_str(), NULL, 16);
        b = strtoul(colorHex.substring(4, 6).c_str(), NULL, 16);
        
        effect = 0;
        fill_solid(leds, NUM_LEDS, CRGB(r, g, b));
        FastLED.show();
    }
    server.sendHeader("Location", "/");
    server.send(303);
}


void handleEffect_rainbow() {
    effect = 1; // Rainbow
    server.sendHeader("Location", "/");
    server.send(303);
}
void handleEffect_snow() {
    effect = 2; // Snow
    server.sendHeader("Location", "/");
    server.send(303);
}
void handleEffect_disco() {
    effect = 3; // Disco
    server.sendHeader("Location", "/");
    server.send(303);
}
void handleEffect_cicle() {
    effect = 4; // Cicle
    server.sendHeader("Location", "/");
    server.send(303);
}
void handleEffect_sparkling() {
    effect = 5; // Sparkling
    server.sendHeader("Location", "/");
    server.send(303);
}


// wrapper so server.on("/setEffect", handleEffect_mixed) links correctly
void handleEffect_mixed() {
    effect = 6; // Mixed
    server.sendHeader("Location", "/");
    server.send(303);

}

// wrapper so server.on("/setScheduler", handleSet_scheduler) links correctly
void handleSet_scheduler() {
    handleScheduler();
}

// simple Not Found handler (was declared but missing)
void handleNotFound() {
    String message = "404: Not Found\n\n";
    message += "URI: ";
    message += server.uri();
    message += "\nMethod: ";
    message += (server.method() == HTTP_GET) ? "GET" : "OTHER";
    server.send(404, "text/plain", message);
}
