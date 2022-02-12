#include <RadioHead.h>

/***************************************************************************
  This is a library for the BMP3XX temperature & pressure sensor

  Designed specifically to work with the Adafruit BMP388 Breakout
  ----> http://www.adafruit.com/products/3966

  These sensors use I2C or SPI to communicate, 2 or 4 pins are required
  to interface.

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing products
  from Adafruit!

  Written by Limor Fried & Kevin Townsend for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ***************************************************************************/

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BMP3XX.h"
#include "DHT.h"

const byte address[6] = "00001";

#include <RH_ASK.h>

#define DHTPIN 2     
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321


#define BMP_SCK 13
#define BMP_MISO 12
#define BMP_MOSI 11
#define BMP_CS 10

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BMP3XX bmp;
DHT dht(DHTPIN, DHTTYPE);
RH_ASK driver(2000, 5, 4);


void setup() {
  Serial.begin(9600);
  while (!Serial);
  if (!driver.init())
         Serial.println("init failed");

  dht.begin();

  if (!bmp.begin_SPI(BMP_CS)) {  // hardware SPI mode  
    Serial.println("Could not find a valid BMP3 sensor, check wiring!");
    while (1);
  }

  // Set up oversampling and filter initialization
  bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
  bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X);
  bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
  bmp.setOutputDataRate(BMP3_ODR_50_HZ);
}

void loop() {
  if (!bmp.performReading()) {
    Serial.println("Failed to perform reading :(");
    return;
  }

  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  float pressure = bmp.pressure / 100.0;
  float bmp_t = bmp.temperature;
  
  Serial.print(bmp.temperature);
  Serial.print(" ");
  Serial.print(bmp.pressure / 100.0);
  Serial.print(" ");
  Serial.print(h);
  Serial.println();
  
  char buf[16] = "\0";
  char *ptr;
  ptr = buf;
  memcpy(ptr, &pressure, 4);
  ptr += 4;
  memcpy(ptr, &bmp_t, 4);
  ptr += 4;
  memcpy(ptr, &t, 4);
  ptr += 4;
  memcpy(ptr, &h, 4);

  Serial.println(driver.send((uint8_t *)buf, sizeof(buf)));
  Serial.println(driver.waitPacketSent());
  delay(2000);
}
