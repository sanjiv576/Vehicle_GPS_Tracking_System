
// import libraries
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

// i2c lcd, SDA = A4, SCL = A5;
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

// pin declaration
const int RxPin = 7, TxPin = 8;

// create TinyGPS object
TinyGPSPlus gps;


// store latitude and longitude
double latitude;
double longitude;
// establish for serial communication with GPS module
SoftwareSerial ss(TxPin, RxPin);

// pin declaration for NodeMCU
//const int TxNode = 5, RxNode = 6;

//SoftwareSerial esp(TxNode, RxNode);

void setup() {
  //   for arduino baudrate
  Serial.begin(9600);

  // for GPS baudrate
  ss.begin(9600);

  //  for lcd
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.cursor();
  lcd.print("Connecting...");
  lcd.blink();

  // for NodeMCU baudrate
  //esp.begin(115200);


  Serial.println("Searching the signal");
}


void loop() {

  while (ss.available() > 0) {
    byte gpsData = ss.read();

    //    since it is NMEA (National Marine Electronics Association) data format so changing it into human readable
    gps.encode(gpsData);

    if (gps.location.isUpdated()) {

      latitude  = gps.location.lat(), 7;
      longitude = gps.location.lng(), 7;

      //      latitude and longitude display in serial monitor
      Serial.print("Latitude: ");
      Serial.print(latitude);

      Serial.print("  Longitude: ");
      Serial.println(longitude);

      // shows latitude and longitude in I2C lcd
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("lat : ");
      lcd.print(latitude);

      lcd.setCursor(0, 1);
      lcd.print("lon : ");
      lcd.print(longitude);


    }
    // Serial.write(gpsData);

  }

}
