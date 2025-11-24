#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <FastLED.h>


#include "index.h"
//Please, change the settings below

#define LED_PIN     6                       // pin for the led
#define NUM_LEDS    150                     // number of led

const char* ssid = "LED_CONTROL";           //the SSID of the access point
const char* password = "Pa$$w0rd";          //the password of the access point

CRGB leds[NUM_LEDS]; 
WebServer server(80);

int on = true;
int r = 0, g = 255, b = 0; 
int effect = 0;
int hue = 0;
int brightness = 100; 

unsigned long lastMixedEffectChange = 0;
const long mixedEffectInterval = 30000; 
int mixed_effect_index = 0; // 0=Rainbow, 1=Disco, 2=Cicle, 3=Sparkling
const int totalMixedEffects = 4; 

bool schedulerEnabled = false;
int schedulerStartHour = 0;
int schedulerStartMin = 0;
int schedulerEndHour = 0;
int schedulerEndMin = 0; 
bool schedulerWasActive = false; 
int manualCurrentHour = 0;    
int manualCurrentMin = 0;     
unsigned long lastMinuteUpdate = 0;

void handleRoot();
void handleOnOff();
void handleSetcolor();
void handleEffect_rainbow(); 
void handleEffect_snow();
void handleEffect_disco();
void handleEffect_cicle();
void handleEffect_sparkling();
void handleEffect_mixed();
void handleSet_scheduler();
void handleSet_bright();
void handleNotFound();

CRGB colorDisco1 = CRGB::Purple; 
CRGB colorDisco2 = CRGB::Cyan;
CRGB colorDisco3 = CRGB::Lime;
CRGB colorSnow = CRGB::SkyBlue; 

void setup() {
  Serial.begin(115200);
  FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(100);
  FastLED.show();

  WiFi.softAP(ssid, password); 
  IPAddress apIP = WiFi.softAPIP(); 

  Serial.print("IP du serveur: ");
  Serial.println(apIP); 
  
  
  fill_solid(leds, NUM_LEDS, CRGB(r, g, b));
  server.on("/", handleRoot); 
  server.on("/onoff", handleOnOff);
  server.on("/Setcolor", handleSetcolor);
  server.on("/setEffect/rainbow", handleEffect_rainbow);
  server.on("/setEffect/snow", handleEffect_snow);
  server.on("/setEffect/disco", handleEffect_disco);
  server.on("/setEffect/cicle", handleEffect_cicle);
  server.on("/setEffect/sparkling", handleEffect_sparkling);
  server.on("/setEffect/mixed", handleEffect_mixed);
  server.on("/SetScheduler", handleSet_scheduler);
  server.on("/Setbright", handleSet_bright); 
  server.onNotFound(handleNotFound);
  
  server.begin();
  Serial.println("HTTP server started");
}

void manageScheduler() {
    if (millis() - lastMinuteUpdate >= 60000) {
        lastMinuteUpdate = millis(); 
        manualCurrentMin++;
        if (manualCurrentMin >= 60) {
            manualCurrentMin = 0;
            manualCurrentHour++;
            if (manualCurrentHour >= 24) {
                manualCurrentHour = 0;
            }
        }
    }

    // 2. Check the scheduler (only if enabled)
    if (schedulerEnabled) {
        int currentMinutes = manualCurrentHour * 60 + manualCurrentMin; 
        int startMinutes = schedulerStartHour * 60 + schedulerStartMin; 
        int endMinutes = schedulerEndHour * 60 + schedulerEndMin; 
        
        bool shouldBeOn = false;
        if (startMinutes < endMinutes) {
            shouldBeOn = (currentMinutes >= startMinutes && currentMinutes < endMinutes); 
        } else {
            shouldBeOn = (currentMinutes >= startMinutes || currentMinutes < endMinutes); 
        }
        
        if (shouldBeOn != on || !schedulerWasActive) {
            on = shouldBeOn;
            Serial.print("Scheduler: LED ");
            Serial.println(on ? "ON" : "OFF");
        }
        schedulerWasActive = true; // Mark scheduler as active

    } else {
        // If the scheduler is disabled, 
        // ensure 'schedulerWasActive' is false
        schedulerWasActive = false;
    }
}

void loop() {
    server.handleClient();
    
    // Manage the clock and scheduler each cycle
    manageScheduler(); 
    
    if (on) {
      if (effect == 0) { // Static
            fill_solid(leds, NUM_LEDS, CRGB(r, g, b)); 
      } else if (effect == 1) { // Rainbow
            static uint8_t hue = 0;
            fill_rainbow(leds, NUM_LEDS, hue, 7); 
            hue++;
      } else if (effect == 2) { // Snow
            fadeToBlackBy(leds, NUM_LEDS, 8);
            int pos = random16(NUM_LEDS);
            leds[pos] = colorSnow;
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
      } else if (effect == 4){ // Cicle
            static uint8_t hue = 0;
            fadeToBlackBy(leds, NUM_LEDS, 20); 
            int pos = beatsin16(10, 0, NUM_LEDS - 1);
            leds[pos] = CRGB(r, g, b); 
      } else if (effect == 5) { // Sparkling
            fadeToBlackBy(leds, NUM_LEDS, 20);
            int pos = random16(NUM_LEDS); 
            leds[pos] += CRGB::White;
      } else if (effect == 6) { // Mixed
            
            // 1. Check if it's time to change the effect
            if (millis() - lastMixedEffectChange > mixedEffectInterval) {
                lastMixedEffectChange = millis();
                mixed_effect_index = (mixed_effect_index + 1) % totalMixedEffects;
            }

            // 2. Execute the current effect in the sequence
            if (mixed_effect_index == 0) {
                //rainbow
                static uint8_t hue = 0;
                fill_rainbow(leds, NUM_LEDS, hue, 7); 
                hue++;

            } else if (mixed_effect_index == 1) {
                //disco
                static uint8_t hue = 0;
                if (hue == 0) {
                    fill_solid(leds, NUM_LEDS, colorDisco1); 
                } else if (hue == 1){
                    fill_solid(leds, NUM_LEDS, colorDisco2); 
                } else if (hue == 2) {
                    fill_solid(leds, NUM_LEDS, colorDisco3); 
                }
                hue = (hue + 1) % 3; 

            } else if (mixed_effect_index == 2) {
                //cicle
                fadeToBlackBy(leds, NUM_LEDS, 20); 
                int pos = beatsin16(10, 0, NUM_LEDS - 1);
                leds[pos] = CRGB(r, g, b); // Use the last chosen static color

            } else if (mixed_effect_index == 3) {
                //sparkling
                fadeToBlackBy(leds, NUM_LEDS, 20);
                int pos = random16(NUM_LEDS); 
                leds[pos] += CRGB::White;
            }
        }
    } else {
        fill_solid(leds, NUM_LEDS, CRGB::Black); 
    }
    
  FastLED.setBrightness(brightness);
  FastLED.show();
  FastLED.delay(50);
}

void handleRoot() {
    server.send(200, "text/html", index_html); 
}

void handleOnOff() {
    on = !on;
    server.sendHeader("Location", "/");
    server.send(303); 
}


void handleSet_scheduler() { // scheduler function
    schedulerEnabled = server.hasArg("enable"); // check if the box is checked

    if (schedulerEnabled) {
        Serial.println("Scheduler ENABLED");
        // Read times only if scheduler is enabled
        if (server.hasArg("start") && server.hasArg("end") && server.hasArg("now")) {
            String start = server.arg("start"); 
            String end = server.arg("end"); 
            String now = server.arg("now"); // Get current time from form
            // Define start/end times
            schedulerStartHour = start.substring(0, 2).toInt();
            schedulerStartMin = start.substring(3, 5).toInt();
            schedulerEndHour = end.substring(0, 2).toInt();
            schedulerEndMin = end.substring(3, 5).toInt(); 

            // --- SET MANUAL CLOCK ---
            manualCurrentHour = now.substring(0, 2).toInt();
            manualCurrentMin = now.substring(3, 5).toInt();
            
            // Reset minute counter
            lastMinuteUpdate = millis(); 

            Serial.print("Heure actuelle réglée sur: ");
            Serial.print(manualCurrentHour);
            Serial.print(":");
            Serial.println(manualCurrentMin);
            Serial.print("Planificateur réglé de ");
            Serial.print(schedulerStartHour);
            Serial.print(":");
            Serial.print(schedulerStartMin);
            Serial.print(" à ");
            Serial.print(schedulerEndHour);
            Serial.print(":");
            Serial.println(schedulerEndMin);

            // Force manageScheduler to re-evaluate the 'on' state
            schedulerWasActive = false; 
        }
    } else {
        // Scheduler is disabled
        Serial.println("Scheduler DISABLED - Manual control enabled"); 
        on = true; // Turn on lights for manual control
        schedulerWasActive = false; // Mark scheduler as inactive
    }
    
    server.sendHeader("Location", "/");
    server.send(303); 
}

void handleSetcolor() {
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
void handleEffect_mixed() { 
    effect = 6; // Mixed
    mixed_effect_index = 0; 
    lastMixedEffectChange = millis(); 
    server.sendHeader("Location", "/");
    server.send(303);
}


void handleSet_bright() { // brightness function
    if (server.hasArg("brightness")) {
        brightness = server.arg("brightness").toInt();
        FastLED.setBrightness(brightness);
    }
    server.sendHeader("Location", "/");
    server.send(303);
}

void handleNotFound() {
  server.send(404, "text/plain", "404: Not found");
}
