/*
  fil: humidity_meter_v2
  skapare: Viggo Johnsson
  datum: 2023- 12 - 14
  beskrivning: mäter hur varmt och fuktigt det är i ett rum
 */

//det vi använder från library
#include "DHT.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

      /*---skärmen---*/
      
#define SCREEN_WIDTH 128 // OLED display bredd, i pixlar
#define SCREEN_HEIGHT 64 // OLED display höjd, i pixlar

  //resettar skärmen
#define OLED_RESET     -1 // Reset pin # (eller -1 ifall den delas med arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define LOGO_HEIGHT   16 //hur stor loggan är
#define LOGO_WIDTH    16 //och hur bred den är

      /*---mätaren---*/
#define DHTPIN 12   //definera att DHTPIN = 12
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

  //vilka pins som används för lamporna, samt delay för dem
int ledPins[] = { 2, 3, 4,
                  5, 6, 7,
                  8, 9, 10 };   // alla pins som jag använder för lamporna

int delaytime = 100;    

     /*---Setup---*/
     /*
    startup sekvensen för allt som behövs, det inkluderar: skärmen och
    mätaren samt testar om det funkar med "DHTxx test!" i monitorn
    och sätter pinMode till OUTPUT så att lamporna tänds. 
      */
void setup() {
  Serial.begin(9600);
  Serial.println(F("DHTxx test!"));
    dht.begin();
  for(int i = 0; i < 9; i++) {
    pinMode(ledPins[i], OUTPUT);
  }
  Serial.begin(115200);
  // SSD1306_SWITCHCAPVCC = generera display voltage från 3.3V innifrån
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); 
  }
  display.display();
  delay(1000); // Pause for 2 seconds

}
  /*
  hämtar fuktighet och temperatur från mätaren och sätter det till variablerna h och t.
  printar i seriell monitorn temperaturen och fuktigheten eller "Failed to read from DHT sensor!" om det inte fungerar.
   */
void loop() {
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
    int antalLamporOn = map(h, 0, 90, 0, pinCount);  //mappar för vilka lampor som tänds beroende på variabeln h (fuktigheten)

      /*
       sätter på och av lampor
        sammarbetar med mappen, 
       där lampor tänds beroende på hur fuktigt det är.
       
      */
      for(int i = 0; i < pinCount; i++) {
    if(i < antalLamporOn) {
      digitalWrite(ledPins[i], HIGH); //vilken lampa som ska tändas
    }else {
      digitalWrite(ledPins[i], LOW); //vilka som ska vara släckta
    }
  }
}

  /*
   hämtar som innan fuktigheten och temperaturen från mätaren och sätter det till samma variabler.
   men skriver ut det på en display istället för serial monitorn.
   */
void drawSrting(void) {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);
  float hif = dht.computeHeatIndex(f, h);
  float hic = dht.computeHeatIndex(t, h, false);
  display.clearDisplay();

  display.setTextSize(2);      // Normal 1:1 pixel scale
  display.setTextColor(WHITE); // använd vit text
  display.setCursor(0, 0);     // Starta i top-vänster hörna
  /*
  det den printar på displayen
   */
  display.print(F("Fuktighet: "));
  display.print(h);
  display.print("%    ");
  display.print(F("Temp:     "));
  display.print(t);
  display.print(F(" C "));

  display.display();
  delay(500);
}
