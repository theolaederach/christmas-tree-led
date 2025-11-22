#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <FastLED.h>
#include <EEPROM.h>

#include "index.h"
#include "login.h"

//Please, change the settings below

#define LED_PIN     6                       // pin for the led
#define NUM_LEDS    113                     // number of led

const char* ssid = "your_wifi_name";        // your wifi ssid
const char* password = "your_wifi_password";// your wifi password

char password_login[] = "Password";         // your password for login to the control panel




#define EEPROM_SIZE 50

CRGB leds[NUM_LEDS];
WebServer server(80);

int on = true;
int r = 255, g = 0, b = 0;
int effect = 0;
int hue = 0;

// Whitelist EEPROM layout:
// addr 0 = count (uint8_t)
// addr 1.. = up to WHITELIST_MAX IPs, each 4 bytes
#define WHITELIST_MAX 10
IPAddress whitelist[WHITELIST_MAX];
uint8_t whitelistCount = 0;

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
void handleLogin();
void handleNotFound();
void saveState();
void loadState();


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

    // load whitelist from EEPROM
    loadWhitelist();

    server.on("/", handleRoot);
    server.on("/onoff", handleOnOff);
    server.on("/setColor", handleSet_color);
    // keep existing effect endpoints (they may overwrite each other if same path; kept as original)
    server.on("/setEffect", handleEffect_rainbow);
    server.on("/setEffect", handleEffect_snow);
    server.on("/setEffect", handleEffect_disco);
    server.on("/setEffect", handleEffect_cicle);
    server.on("/setEffect", handleEffect_sparkling);
    server.on("/setEffect", handleEffect_mixed);
    server.on("/setScheduler", handleSet_scheduler);
    server.on("/login", handleLogin);
    server.onNotFound(handleNotFound);
    // removed stray "effect" token that caused a compile error
    server.begin();
    Serial.println("HTTP server started"); 

}

void loop() {
    server.handleClient();
    loadState();

    if (on) {
      if (effect == 0) { // Static
        fill_solid(leds, NUM_LEDS, CRGB(r, g, b));
    } else if (effect == 1) { // Rainbow
        static uint8_t hue = 0;
        fill_rainbow(leds, NUM_LEDS, hue, 7);
        hue++;
    } else if (effect == 2) { // Cicle
        static uint8_t hue = 0;
        fadeToBlackBy(leds, NUM_LEDS, 20);
        int pos = beatsin16(10, 0, NUM_LEDS - 1);
        leds[pos] = CRGB(r, g, b);
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
    } else {
        fill_solid(leds, NUM_LEDS, CRGB::Black);
    }
  }  
  FastLED.setBrightness(100);
  FastLED.show();
}

// Updated: check client IP and redirect to login if not whitelisted
void handleRoot() {
    IPAddress clientIp = server.client().remoteIP();
    if (!ipInWhitelist(clientIp)) {
        server.sendHeader("Location", "/login");
        server.send(303);
        return;
    }

    // if allowed, serve the index (assumes index.h provides index_html)
    server.send(200, "text/html", index_html);
}

// Login handler: GET -> show form, POST -> verify password and add IP if correct
void handleLogin() {
    if (server.method() == HTTP_POST) {
        if (server.hasArg("password")) {
            String pw = server.arg("password");
            if (pw == String(password_login)) {
                IPAddress clientIp = server.client().remoteIP();
                addIpToWhitelist(clientIp);
                server.sendHeader("Location", "/");
                server.send(303);
                return;
            } else {
                // wrong password: stay on login
                server.sendHeader("Location", "/login");
                server.send(303);
                return;
            }
        }
    }

    // GET or no password: show login page (assumes login.h provides login_html)
    server.send(200, "text/html", login_html);
}

void handleEffect_sparkling() {
    effect = 4; // Sparkling
    // no persistent effect state needed here; keep consistent with whitelist storage
    saveWhitelist();
    server.sendHeader("Location", "/");
    server.send(303);
}

// Replace old saveState/loadState with whitelist-specific implementations
void saveWhitelist() {
    EEPROM.begin(EEPROM_SIZE);
    if (whitelistCount > WHITELIST_MAX) whitelistCount = WHITELIST_MAX;
    EEPROM.write(0, whitelistCount);
    for (uint8_t i = 0; i < whitelistCount; ++i) {
        uint16_t base = 1 + i * 4;
        IPAddress ip = whitelist[i];
        for (uint8_t b = 0; b < 4; ++b) {
            EEPROM.write(base + b, ip[b]);
        }
    }
    EEPROM.commit();
    EEPROM.end();
}

void loadWhitelist() {
    EEPROM.begin(EEPROM_SIZE);
    whitelistCount = EEPROM.read(0);
    if (whitelistCount > WHITELIST_MAX) whitelistCount = 0;
    for (uint8_t i = 0; i < whitelistCount; ++i) {
        uint16_t base = 1 + i * 4;
        uint8_t b0 = EEPROM.read(base);
        uint8_t b1 = EEPROM.read(base + 1);
        uint8_t b2 = EEPROM.read(base + 2);
        uint8_t b3 = EEPROM.read(base + 3);
        whitelist[i] = IPAddress(b0, b1, b2, b3);
    }
    EEPROM.end();

    // debug print
    Serial.print("Whitelist loaded (count=");
    Serial.print(whitelistCount);
    Serial.println("):");
    for (uint8_t i = 0; i < whitelistCount; ++i) {
        Serial.println(whitelist[i].toString());
    }
}

bool ipInWhitelist(IPAddress ip) {
    for (uint8_t i = 0; i < whitelistCount; ++i) {
        if (whitelist[i] == ip) return true;
    }
    return false;
}

bool addIpToWhitelist(IPAddress ip) {
    if (ipInWhitelist(ip)) return true;
    if (whitelistCount >= WHITELIST_MAX) {
        Serial.println("Whitelist full, cannot add more IPs");
        return false;
    }
    whitelist[whitelistCount++] = ip;
    saveWhitelist();
    Serial.print("Added IP to whitelist: ");
    Serial.println(ip.toString());
    return true;
}