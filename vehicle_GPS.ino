#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>

TinyGPSPlus gps;
SoftwareSerial SerialGPS(4, 5);

const char* ssid     = "SS_wlink";
const char* password = "@74jApAn#64!";

String status;

float Latitude , Longitude, speedVeh;
int year , month , date, hour , minute , second;
String DateString , TimeString , LatitudeString , LongitudeString, SpeedString;


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
  Serial.println("Server started");
  Serial.println(WiFi.localIP());
}

void loop()
{
  while (SerialGPS.available() > 0)
    if (gps.encode(SerialGPS.read()))
    {
      if (gps.location.isValid())
      {
        status = "On";
        Latitude = gps.location.lat();
        LatitudeString = String(Latitude , 6);
        Longitude = gps.location.lng();
        LongitudeString = String(Longitude , 6);

        speedVeh = gps.speed.value();
        SpeedString = String(speedVeh);
      }

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

      if (gps.time.isValid())
      {
        TimeString = "";
        hour = gps.time.hour() + 5; //adjust UTC
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
  WiFiClient client = server.available();
  if (!client)
  {
    return;
  }

  //Response
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n <!DOCTYPE html><html lang='en'><head><meta charset='UTF-8'><meta http-equiv='X-UA-Compatible' content='IE=edge'> <meta name='viewport' content='width=device-width, initial-scale=1.0'><title>Vehicle GPS Tracking System</title><link rel='icon' type='image/x-icon' href='https://cdn-icons-png.flaticon.com/512/3927/3927464.png'><!-- CSS only --><link href='https://cdn.jsdelivr.net/npm/bootstrap@5.2.2/dist/css/bootstrap.min.css' rel='stylesheet' integrity='sha384-Zenh87qX5JnK2Jl0vWa8Ck2rdkQ2Bzep5IDxbcnCeuOxjzrPF/et3URy9Bv1WTRi' crossorigin='anonymous'> <style>* {color: white;}h1,h3 {text-align: center;}body {background-color: rgb(57, 43, 43);}ul {list-style-type: none;} </style></head><body> <div class='containter'> <h1>Vehicle GPS Tracking System </h1> <br><div class='text-center'><img class='img-fluid' style='width: 40%;'src='https://www.istartek.com/wp-content/uploads/2019/06/img_2019-06-04_04-09-49.jpg' alt=''></div>  <br>  <div class='table-responsive'><table class='table table-stripped'><thead class='table-dark'><th>S.N</th><th>Vehicle Name</th><th>Status</th><th>Latitude</th> <th>Longitude</th><th>Speed</th><th>Date</th><th>Time</th><th>Google Map</th> </thead><tbody><tr><td>1</td><td>Veh-A</td><td>";
  s += status + "</td><td>" + LatitudeString + "</td> <td>" + LongitudeString + "</td><td>" + SpeedString + "</td><td>" + DateString + "</td><td>" + TimeString;
  s += "</td><td><a href='http://maps.google.com/maps?&z=15&mrt=yp&t=k&q='" + LatitudeString + "+" + LongitudeString + """ target=""_top"">Find Me</a></td></tr><tr><td>2</td><td>Veh-B</td><td>Off</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td></tr></tbody></table></div><br> <h3> Note: Click on <b>'Google Map'</b> to view in Google Map</h3></div><br> <footer class='text-center'> Powered by Sanjiv Shrestha</footer></body></html> \n";

  client.print(s);
  delay(100);

}
