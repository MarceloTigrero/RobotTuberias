#include "robot_tuberias_raiz.h"
#include <TinyGPSPlus.h>
#include <sstream>
#include <ESP32Time.h>

// robot_tuberias_gps.h
#ifndef ROBOT_TUBERIAS_GPS_H
#define ROBOT_TUBERIAS_GPS_H

extern ESP32Time rtc;  // Solo declaración externa

// Otras declaraciones y contenido del archivo

#endif
/*
ESP32
UART	RX IO	TX IO	CTS	RTS
UART0	GPIO3	GPIO1	N/A	N/A
UART1	GPIO9	GPIO10	GPIO6	GPIO11
UART2	GPIO16	GPIO17	GPIO8	GPIO7
*/
#define RXD0 3
#define TXD0 1

#define RXD1 9
#define TXD1 10

#define RXD2 16
#define TXD2 17

struct Date_time {
  uint16_t year;
  uint8_t month;
  uint8_t day;
  uint8_t hour;
  uint8_t minute;
  uint8_t second;
  double lat;
  double lng;
  double alt;
  char date[15];
};


void obtainDate_time(Date_time *date_time, TinyGPSPlus *gps);
void printDate_time(Date_time *date_time);
void printDate_time(Date_time *date_time, char *buffer);
void printDate(Date_time *date_time, char *buffer);
void printtime(Date_time *date_time, char *buffer);
void printDate_time(Date_time *date_time, std::stringstream &ss);
void readGPS(Date_time *date_time, TinyGPSPlus *gps);
void read1GPS(Date_time *date_time, TinyGPSPlus *gps);
void read2GPS(Date_time *date_time, TinyGPSPlus *gps);
void readGPS(Date_time *date_time, TinyGPSPlus *gps, HardwareSerial *serial);

void Upgrade_time( void * pvParameters );




