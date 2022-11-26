// import libraries
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>

// create object of TinyGPS++
TinyGPSPlus gps;

// serial communication Tx=D2=4=SDA , Rx=D1=5=SCL
SoftwareSerial SerialGPS(4, 5);  // Tx, Rx


const char* ssid     = "SS_wlink";
const char* password = "@74jApAn#64!";

// declaration variables for GPS details

String status;
float Latitude , Longitude, speedVeh;
int year , month , date, hour , minute , second;
String DateString , TimeString , LatitudeString , LongitudeString, SpeedString, notAvailable;
bool show = false;
int waitTime = 3000;

 

// web server port
WiFiServer server(80);

void setup()
{
  Serial.begin(9600);
  SerialGPS.begin(9600);
  Serial.println();
  Serial.print("Connecting");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  server.begin();
  Serial.print("Server started at : ");

  // IP address of Node MCU
  Serial.println(WiFi.localIP());
}

void loop()
{


  //  if gps is detected
  while (SerialGPS.available() > 0) {


    if (gps.encode(SerialGPS.read()))
    {
      show = false;
      //  get latitude, longitude and speed
      getVehLocation();

      // invoke the function to get date of the vehicle
      getVehDate();

      // invoke the function to get time of the vehicle
      getVehTime();

    }
  }

  //  if gps is not detected then again try to connect 

  if (millis() > waitTime && gps.charsProcessed() < 10) {
    Serial.println("No GPS detected");
    show = true;
    notAvailable = "<h3><br>Sorry, GPS signal is not found</h3>";
    while (true);
  }

  WiFiClient client = server.available();
  if (!client)
  {
    return;
  }

  //Response and show in local host
  String gui = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n <!DOCTYPE html><html lang='en'><head><meta charset='UTF-8'><meta http-equiv='X-UA-Compatible' content='IE=edge'> <meta name='viewport' content='width=device-width, initial-scale=1.0'><title>Vehicle GPS Tracking System</title><link rel='icon' type='image/x-icon' href='https://cdn-icons-png.flaticon.com/512/3927/3927464.png'><!-- CSS only --><link href='https://cdn.jsdelivr.net/npm/bootstrap@5.2.2/dist/css/bootstrap.min.css' rel='stylesheet' integrity='sha384-Zenh87qX5JnK2Jl0vWa8Ck2rdkQ2Bzep5IDxbcnCeuOxjzrPF/et3URy9Bv1WTRi' crossorigin='anonymous'> <style>* {color: white;}h1,h3 {text-align: center;}body {background-color: rgb(57, 43, 43);}ul {list-style-type: none;} </style></head><body> <div class='containter'> <h1>Vehicle GPS Tracking System </h1> <br><div class='text-center'><img class='img-fluid' style='width: 40%;'src='https://www.istartek.com/wp-content/uploads/2019/06/img_2019-06-04_04-09-49.jpg' alt=''></div>  <br>  <div class='table-responsive'><table class='table table-stripped'><thead class='table-dark'><th>S.N</th><th>Vehicle Name</th><th>Status</th><th>Latitude</th> <th>Longitude</th><th>Speed (km/hr)</th><th>Date</th><th>Time</th><th>Google Map</th> </thead><tbody><tr><td>1</td><td>Veh-A</td><td>";
  gui += status + "</td><td>" + LatitudeString + "</td> <td>" + LongitudeString + "</td><td>" + SpeedString + "</td><td>" + DateString + "</td><td>" + TimeString;
  gui += "</td><td><a href='http://maps.google.com/maps?&z=15&mrt=yp&t=k&q=" + LatitudeString + "+" + LongitudeString + "'' target=''_top''>Find Me</a></td></tr><tr><td>2</td><td>Veh-B</td><td>Off</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td></tr><tr><td>3</td><td>Veh-C</td><td>Off</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td></tr><tr><td>4</td><td>Veh-D</td><td>Off</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td></tr><tr><td>5</td><td>Veh-E</td><td>Off</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td></tr><tr><td>6</td><td>Veh-F</td><td>Off</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td></tr></tbody></table></div><br> <h3> Note: Click on <b>'Find Me'</b> to view in Google Map</h3></div><br>";

  if (show) {
    gui += notAvailable;
  }
  gui += "<footer class='text-center'> Powered by Sanjiv Shrestha</footer></body></html> \n";

  client.print(gui);
  delay(100);

}



// --------------------------- FUNCTIONS HERE --------------------------------------------

// for current date

void getVehDate() {

  if (gps.date.isValid())
  {
    DateString = "";
    date = gps.date.day();
    month = gps.date.month();
    year = gps.date.year();

    if (date < 10)
      DateString = '0';
    DateString += String(date);

    DateString += " / ";

    if (month < 10)
      DateString += '0';
    DateString += String(month);
    DateString += " / ";

    if (year < 10)
      DateString += '0';
    DateString += String(year);
  }
}

// for current time

void getVehTime() {

  if (gps.time.isValid())
  {
    TimeString = "";
    hour = gps.time.hour() + 5;
    minute = gps.time.minute();
    second = gps.time.second();

    if (hour < 10)
      TimeString = '0';
    TimeString += String(hour);
    TimeString += " : ";

    if (minute < 10)
      TimeString += '0';
    TimeString += String(minute);
    TimeString += " : ";

    if (second < 10)
      TimeString += '0';
    TimeString += String(second);
  }
}


// for current latitude and longitude

void getVehLocation() {
  if (gps.location.isValid())
  {
    status = "On";
    Latitude = gps.location.lat();
    LatitudeString = String(Latitude , 7);
    Longitude = gps.location.lng();
    LongitudeString = String(Longitude , 7);

    // speed in km/hr
    speedVeh = gps.speed.kmph();
    SpeedString = String(speedVeh);
  }
  //  else {
  //    show = true;
  //    notAvailable = "<h3><br>Sorry, GPS signal is not found</h3>";
  //  }
}
