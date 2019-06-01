
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

TinyGPSPlus gps;         // The TinyGPS++ object
SoftwareSerial ss(4, 5); // The serial connection to the GPS device

// JSON
#include <ArduinoJson.h>

// SD Karte
#include <SPI.h>
#include <SD.h>

// change this to match your SD shield or module;
// WeMos Micro SD Shield V1.0.0: D8
// LOLIN Micro SD Shield V1.2.0: D4 (Default)
const int chipSelect = D8;


float latitude , longitude, hoehe, geschwindigkeit;
int year , month , date, hour , minute , second;
String date_str , time_str , lat_str , lng_str, alt_str, speed_str;
String time_prt, dsn_str;

void setup()
{
  Serial.begin(115200);
  ss.begin(9600);
  time_prt = "XX:XX:XX";
  dsn_str = "GPS.TXT";

  Serial.print("Initializing SD card...");
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");

}

void loop()
{
  // when serial device is available 
  while (ss.available() > 0)

  // looking if gps is ready
  if (gps.encode(ss.read()))
  {
	// 
    if (gps.location.isValid())
    {
      latitude = gps.location.lat();
      lat_str = String(latitude , 6);
      longitude = gps.location.lng();
      lng_str = String(longitude , 6);
      hoehe = gps.altitude.meters();
      alt_str = String(hoehe,2);
      geschwindigkeit = gps.speed.kmph();
      speed_str = String(geschwindigkeit,2);

	  // each day one dataset 
      if (dsn_str == "GPS.TXT")
      {
        dsn_str = "GP";
        dsn_str += date_str.substring(2,4);
        dsn_str += date_str.substring(5,7);
        dsn_str += date_str.substring(8,10);
        dsn_str += ".TXT";
        Serial.print("Dataset used:");
        Serial.println(dsn_str);
      }
    } 
    
	// extracting the date
    if (gps.date.isValid())
    {
      date_str = "";
      date = gps.date.day();
      month = gps.date.month();
      year = gps.date.year();

      date_str += String(year);
      date_str += "-";
      if (month < 10)
        date_str += '0';
      date_str += String(month);
      date_str += "-";
      if (date < 10)
        date_str += '0';
      date_str += String(date);
    }

	// extracting the time 
    if (gps.time.isValid())
    {
      time_str = "";
      hour = gps.time.hour();
      minute = gps.time.minute();
      second = gps.time.second();

      if (hour < 10)
        time_str = '0';
      time_str += String(hour);
      time_str += ":";
      if (minute < 10)
        time_str += '0';
      time_str += String(minute);
      time_str += ":";
      if (second < 10)
        time_str += '0';
      time_str += String(second);
    }
  }
  
  // write only every 5 seconds
  if (((time_str.charAt(7)=='0') or (time_str.charAt(7)=='5')) and (time_str!=time_prt))
  {
    StaticJsonBuffer<500> jsonBuffer;
    JsonObject& root = jsonBuffer.createObject();
    root["date"] = date_str;
    root["time"] = time_str;
    root["lon"] = lng_str;
    root["lat"] = lat_str;
    root["alt"] = alt_str;
    root["speed"] = speed_str;
  
    String jsonString;
    root.printTo(jsonString);
    char data[120];
    jsonString.toCharArray(data, (jsonString.length() + 1));

    // open the file. note that only one file can be open at a time,
    // so you have to close this one before opening another.
    File dataFile = SD.open(dsn_str, FILE_WRITE);
  
    // if the file is available, write to it:
    if (dataFile) {
      dataFile.println(data);
      dataFile.close();
      // print to the serial port too:
      Serial.println(data);
    }
    // if the file isn't open, pop up an error:
    else {
      Serial.println("error opening dataset");
    }
    time_prt=time_str;
    delay(100);
  }

}
