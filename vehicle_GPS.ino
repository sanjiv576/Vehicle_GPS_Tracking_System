// import libraries
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

// pin declaration
const int RxPin = 7, TxPin = 8;

// create TinyGPS object
TinyGPSPlus gps;

// establish for serial communication with GPS module
SoftwareSerial ss(TxPin, RxPin);

// pin declaration for NodeMCU
const int TxNode = 5, RxNode = 6;

SoftwareSerial esp(TxNode, RxNode);

void setup() {
  //   for arduino baudrate
  Serial.begin(9600);

// for NodeMCU baudrate 
esp.begin(115200);
  //  for GPS baudrate
  ss.begin(9600);

  Serial.println("Searching the signal");
}


void loop() {

  while (ss.available() > 0) {
    byte gpsData = ss.read();

    //    since it is NMEA (National Marine Electronics Association) data format so changing it into human readable
    gps.encode(gpsData);

    if (gps.location.isUpdated()) {
      Serial.print("Latitude: ");
      Serial.print(gps.location.lat(), 7);

      Serial.print("  Longitude: ");
      Serial.println(gps.location.lng(), 7);
    }
    // Serial.write(gpsData);

  }

}
