#include <RadioHead.h>

#include <RH_ASK.h>
#include <SPI.h>

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735

#define TFT_CS        9
#define TFT_RST       -1 // Or set to -1 and connect to Arduino RESET pin
#define TFT_DC         6

RH_ASK driver(2000, 4);

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);


void setup() {
  Serial.begin(9600);
  while (!Serial);
  tft.initR(INITR_MINI160x80); 
  if(!driver.init()) {
    Serial.println("Radio init failed");
  }
  tft.invertDisplay(true);
  tft.fillScreen(0x0000);
  tft.setRotation(1);
  Serial.println("X");
}
 

void loop() {
  uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
  uint8_t buflen = sizeof(buf);

  if (driver.recv(buf, &buflen)) {
    Serial.println("X");
    float pressure, bmp_t, t, h;
    uint8_t *ptr;
    ptr = buf;
    memcpy(&pressure, ptr, 4);
    ptr += 4;
    memcpy(&bmp_t, ptr, 4);
    ptr += 4;
    memcpy(&t, ptr, 4);
    ptr += 4;
    memcpy(&h, ptr, 4);
    
    Serial.println(pressure);
    Serial.println(bmp_t);
    Serial.println(t);
    Serial.println(h);
    tft.fillScreen(0x0000);
    char text[32];
    char text2[32];
    dtostrf(pressure, 5, 3, text2);
    sprintf(text, "Pressure: %s", text2); 
    tft.setCursor(10, 10);
    testdrawtext(text, 0xffff);
    dtostrf(bmp_t, 5, 3, text2);
    sprintf(text, "Temperature: %s", text2); 
    tft.setCursor(10, 20);
    testdrawtext(text, 0xffff);
    dtostrf(h, 5, 3, text2);
    sprintf(text, "Humidity: %s", text2); 
    tft.setCursor(10, 30);
    testdrawtext(text, 0xffff);
  }
}
void testdrawtext(char *text, uint16_t color) {
  tft.setTextSize(1);
  tft.setTextColor(color);
  tft.setTextWrap(true);
  tft.print(text);
}
