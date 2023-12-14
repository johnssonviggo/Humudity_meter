#include "DHT.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display bredd, i pixlar
#define SCREEN_HEIGHT 64 // OLED display höjd, i pixlar

#define OLED_RESET     -1 // Reset pin # (eller -1 ifall den delas med arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define LOGO_HEIGHT   16 //hur stor loggan är
#define LOGO_WIDTH    16 //och hur bred den är

#define DHTPIN 12   //definera att DHTPIN = 12
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

int ledPins[] = { 2, 3, 4,
                  5, 6, 7,
                  8, 9, 10 };   // alla pins som jag använder för lamporna

int delaytime = 100;    

void setup() {
  Serial.begin(9600);
  Serial.println(F("DHTxx test!"));
    dht.begin();
  for(int i = 0; i < 9; i++) {
    pinMode(ledPins[i], OUTPUT);
  }
  Serial.begin(115200);
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); 
  }
  display.display();
  delay(1000); // Pause for 2 seconds

}

void loop() {
  
  // Till nästa gång:
  // Säg att din fukthetsmätare ger ett värde mellan 0 och 100 och du vill tända rätt antal lampor
  // Använd map()
  // pinCount = map(humidity, 0, 100, 0, 9); (inte testat)
  // // Elias
  drawSrting();
  delay(1000);
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);
    if (isnan(h) || isnan(t) || isnan(f)) {
      Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  float hif = dht.computeHeatIndex(f, h);
  float hic = dht.computeHeatIndex(t, h, false);
    Serial.print(F("%  Temperature: "));
    Serial.print(t);
    Serial.print(F("°C "));
    Serial.print(f);
    Serial.print(F("°F  Heat index: "));
    Serial.print(hic);
    Serial.print(F("°C "));
    Serial.print(hif);
    int pinCount = 9; // mellan 0-9
    int humidity = 100;
    int antalLamporOn = map(h, 0, 90, 0, pinCount);
 
  for(int i = 0; i < pinCount; i++) {
    if(i < antalLamporOn) {
      digitalWrite(ledPins[i], HIGH);
    }else {
      digitalWrite(ledPins[i], LOW);
    }
  }
}

void drawSrting(void) {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);
  float hif = dht.computeHeatIndex(f, h);
  float hic = dht.computeHeatIndex(t, h, false);
  display.clearDisplay();

  display.setTextSize(2);      // Normal 1:1 pixel scale
  display.setTextColor(WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.print(F("Fuktighet: "));
  display.print(h);
  display.print("%    ");
  display.print(F("Temp:     "));
  display.print(t);
  display.print(F(" C "));

  display.display();
  delay(500);
}
