#include "robot_tuberias_gps.h"
ESP32Time rtc(3600); 
//Task2code: updates the GPS with the RTC data every second
void Upgrade_time( void * pvParameters ){
  Date_time *date_time = (Date_time*)pvParameters;
  rtc.setTime(0, 0, 0, 0, 0, 0);
  while (1){
    date_time->second = rtc.getSecond();
    date_time->minute = rtc.getMinute();
    date_time->hour = rtc.getHour(true);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void obtainDate_time(Date_time *date_time, TinyGPSPlus *gps){
  if (gps->date.isValid() && gps->time.isValid() && gps->location.isValid() && gps->altitude.isValid())
  {
    date_time->year = gps->date.year();
    date_time->month = gps->date.month();
    date_time->day = gps->date.day();
    date_time->hour = gps->time.hour();
    date_time->minute = gps->time.minute();
    date_time->second = gps->time.second();
    date_time->lat = gps->location.lat();
    date_time->lng = gps->location.lng();
    date_time->alt = gps->altitude.meters();
    printDate(date_time, date_time->date); 
    //rtc.setTime(date_time->second, date_time->minute, date_time->hour, date_time->day, date_time->month, date_time->year);
    rtc.setTime(gps->time.second(), gps->time.minute(), gps->time.hour(), gps->date.day(), gps->date.month(), gps->date.year());
  }
}
void printDate_time(Date_time *date_time){
  Serial << "Date/Time: ";
  Serial << date_time->year << "/" << date_time->month << "/" << date_time->day << " ";
  Serial << date_time->hour << ":" << date_time->minute << ":" << date_time->second << "\n";
}
void printDate_time(Date_time *date_time, char *buffer){
  sprintf(buffer, "%d/%d/%d_%d:%d:%d", date_time->year, date_time->month, date_time->day, date_time->hour, date_time->minute, date_time->second);
}
void printDate(Date_time *date_time, char *buffer){
  if (date_time->month < 10)
  {
    if (date_time->day < 10)
    {
      sprintf(buffer, "%d0%d0%d", date_time->year, date_time->month, date_time->day);
    }
    else
    {
      sprintf(buffer, "%d0%d%d", date_time->year, date_time->month, date_time->day);
    }
  }
  else
  {
    if (date_time->day < 10)
    {
      sprintf(buffer, "%d%d0%d", date_time->year, date_time->month, date_time->day);
    }
    else
    {
      sprintf(buffer, "%d%d%d", date_time->year, date_time->month, date_time->day);
    }
  }
}
void printtime(Date_time *date_time, char *buffer){
  if (date_time->hour < 10)
  {
    if (date_time->minute < 10)
    {
      sprintf(buffer, "0%d:0%d:%d", date_time->hour, date_time->minute, date_time->second);
    }
    else
    {
      sprintf(buffer, "0%d:%d:%d", date_time->hour, date_time->minute, date_time->second);
    }
  }
  else
  {
    if (date_time->minute < 10)
    {
      sprintf(buffer, "%d:0%d:%d", date_time->hour, date_time->minute, date_time->second);
    }
    else
    {
      sprintf(buffer, "%d:%d:%d", date_time->hour, date_time->minute, date_time->second);
    }
  }
}

void printDate_time(Date_time *date_time, std::stringstream &ss){
  ss << date_time->year << "/" << date_time->month << "/" << date_time->day << " ";
  ss << date_time->hour << ":" << date_time->minute << ":" << date_time->second;
}

void readGPS(Date_time *date_time, TinyGPSPlus *gps)
{
  Serial.begin(9600, SERIAL_8N1, RXD0, TXD0);
  while (date_time->year == 0)
  {
    while (Serial.available() > 0)
    {
      if (gps->encode(Serial.read()))
      {
        obtainDate_time(date_time, gps);
      } 
    }
  }
  Serial.end();
}

void read1GPS(Date_time *date_time, TinyGPSPlus *gps)
{
  Serial1.begin(9600, SERIAL_8N1, RXD1, TXD1);
  while (date_time->year == 0)
  {
    while (Serial1.available() > 0)
    {
      if (gps->encode(Serial1.read()))
      {
        obtainDate_time(date_time, gps);
      } 
    }
  }
  Serial1.end();
}

void read2GPS(Date_time *date_time, TinyGPSPlus *gps)
{
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2); 
  while (date_time->year == 0)
  {
    while (Serial2.available() > 0)
    {
      if (gps->encode(Serial2.read()))
      {
        obtainDate_time(date_time, gps);

      } 
    }
    
  }
  Serial2.end();
}

// use another serial port
void readGPS(Date_time *date_time, TinyGPSPlus *gps, HardwareSerial *serial)
{
  while (date_time->year == 0)
  {
    while (serial->available() > 0)
    {
      if (gps->encode(serial->read()))
      {
        obtainDate_time(date_time, gps);
      } 
    }
    
  }
}

// use another serial port uart2 (ESP32) 16,17
void readGPS(Date_time *date_time, TinyGPSPlus *gps, HardwareSerial *serial, int rx, int tx)
{
  serial->begin(9600, SERIAL_8N1, rx, tx);
  while (date_time->year == 0)
  {
    while (serial->available() > 0)
    {
      if (gps->encode(serial->read()))
      {
        obtainDate_time(date_time, gps);
      } 
    }
    
  }
}
