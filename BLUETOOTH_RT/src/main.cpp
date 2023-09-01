#include "robot_tuberias_bluetooth.h"

TinyGPSPlus gps = TinyGPSPlus();
Adafruit_MPU6050 mpu;
TaskHandle_t Task1; // hydrophone
TaskHandle_t Task2; // time

std::stringstream ss;

BluetoothSerial SerialBT;
#define HydrophonePin 4
const int pinLed = 2;
const uint8_t resolution = 12;
bool filecreated = false;
char c = '0'; // estado inicial

Date_time date_time1 = {0, 0, 0, 0, 0, 0, 0, 0, 0, ""};
Date_time date_time2 = {0, 0, 0, 0, 0, 0, 0, 0, 0, ""};
robot_tuberias_i2c_data robot_tuberias_i2c = {0, 0, 0, 0, 0, 0, 0};
hydrophone_data hydrophone = {HydrophonePin, resolution, 0, 0};

void cargardatos_SD(Date_time date, double hydrophone, double accelX, double accelY, double accelZ, double gyroX, double gyroY, double gyroZ);
void cargartimpos_SD(Date_time date1,Date_time date2);

void setup(){
  Serial.begin(9600);
  while (!Serial)
    delay(10);

  xTaskCreatePinnedToCore(
      Read_Hydrophone, // Task function. 
      "Task1",         // name of task. /
      10000,           // Stack size of task /
      &hydrophone,     // parameter of the task /
      1,               // priority of the task 
      &Task1,          // Task handle to keep track of created task /
      0);              // pin task to core 0 /
  delay(500);
}
void loop(){
  // task 1
  yield();
  print_hydrophone_data(&hydrophone);
  yield();
}

/*
void setup()
{
  Serial.begin(9600);
  while (!Serial)
    delay(10);

  xTaskCreatePinnedToCore(
      Read_Hydrophone, // Task function. 
      "Task1",         // name of task. /
      10000,           // Stack size of task /
      &hydrophone,     // parameter of the task /
      1,               // priority of the task 
      &Task1,          // Task handle to keep track of created task /
      0);              // pin task to core 0 /
  delay(500);
  xTaskCreatePinnedToCore(
      Upgrade_time, // Task function. /
      "Task2",      // name of task. 
      10000,        // Stack size of task /
      &date_time1,  // parameter of the task /
      1,            // priority of the task /
      &Task2,       // Task handle to keep track of created task /
      0);           // pin task to core 0 /
  delay(500);
  pinMode(pinLed, OUTPUT);

  // esperar a que se conecte el monitor serial para continuar y almacenamos el address del dispositivo para poder conectarnos a el
  SerialBT.begin("Robot-Tuberias"); // Bluetooth device name
  while (!SerialBT.connected())
    delay(100);
  Serial << "\n  Connectado ";
}

void loop()
{
  yield();
  if (SerialBT.available())
  {
    char c = SerialBT.read();
    switch (c)
  {
  case '1':
    Serial << "Estado" << c << "\n";
    // disconnect bluetooth
    SerialBT.disconnect();
    // send message bluetooth that the state is 1
    read2GPS(&date_time1, &gps);
    // envio de datos de lat y long por bluetooth en un solo mensaje
    // connect bluetooth
    while (!SerialBT.connected())
      delay(100);
    SerialBT.print(date_time1.lat, 6);
    SerialBT.print(",");
    SerialBT.println(date_time1.lng, 6);

    Serial << date_time1.lat << "," << date_time1.lng << " - date,time\n"
           << date_time1.date << "," << date_time1.hour << ":" << date_time1.minute << ":" << date_time1.second << "\n";
    break;
  case '2':
    Serial << "Estado" << c << "\n";
    // send message bluetooth that the state is 0
    SerialBT.println("State 2");

    // disconnect bluetooth
    SerialBT.disconnect();
    init_mpu(mpu);
    printconfig_mpu(mpu);
    while (c == '2')
    {
      yield();
      catch_mpu_data(mpu, &robot_tuberias_i2c);
      yield();
      cargardatos_SD(date_time1, hydrophone.analogValue, robot_tuberias_i2c.accelX, robot_tuberias_i2c.accelY, robot_tuberias_i2c.accelZ, robot_tuberias_i2c.gyroX, robot_tuberias_i2c.gyroY, robot_tuberias_i2c.gyroZ);
      yield();
      if (SerialBT.available())
      {
        c = SerialBT.read();
      }
    }

    break;
  case '3':
    read2GPS(&date_time2, &gps);
    // envio de datos de lat y long por bluetooth en un solo mensaje
    SerialBT.print(date_time2.lat, 6);
    SerialBT.print(",");
    SerialBT.println(date_time2.lng, 6);
    Serial << "Estado" << c << "\n";
    Serial << date_time2.lat << "," << date_time2.lng << " - date,time\n"
           << date_time2.date << "," << date_time2.hour << ":" << date_time2.minute << ":" << date_time2.second << "\n";
    break;
  case '4':
    // send message bluetooth that the state is 0
    SerialBT.println("State 4");
    Serial << "Estado" << c << "\n";
    cargartimpos_SD(date_time1, date_time2);
    // disconnect bluetooth
    SerialBT.disconnect();
    // forsamos un error en el bluetooth para que se envie el mensaje de error
    SerialBT.begin("Robot-Tuberias"); // Bluetooth device name
  

    c = '0';  
    break;
  default:
    break;
  }
  }
}

*/


void cargardatos_SD(Date_time date, double hydrophone, double accelX, double accelY, double accelZ, double gyroX, double gyroY, double gyroZ)
{
  if (!SD.begin())
  {
    Serial.println("Card Mount Failed");
    return;
  }
  // nombre del archivo "/" + buffer + ".csv"
  char s[20] = "/";
  strcat(s, date.date);
  strcat(s, ".csv");
  // abrir archivo
  if (!filecreated)
  {
    writeFile(SD, s, "Hora,Hydrophone,AccelX,AccelY,AccelZ,GyroX,GyroY,GyroZ\n");
    filecreated = true;
  }
  else
  {
    char data[100];
    // convertir uint8_t a int
    int hour = date.hour;
    int minute = date.minute;
    int second = date.second;
    sprintf(data, "%d:%d:%d,%f,%f,%f,%f,%f,%f,%f\n", hour, minute, second, hydrophone, accelX, accelY, accelZ, gyroX, gyroY, gyroZ);
    // sprintf(data, "%f:%f:%f,%f,%f,%f,%f,%f,%f,%f\n",date.hour,date.minute,date.second,hydrophone,accelX,accelY,accelZ,gyroX,gyroY,gyroZ);
    appendFile(SD, s, data);
  }
  // cerrar archivo
  SD.end();
}
void cargartimpos_SD(Date_time date1,Date_time date2)
{
  if (!SD.begin())
  {
    Serial.println("Card Mount Failed");
    return;
  }
  // nombre del archivo "/" + buffer + ".csv"
  char s[20] = "/";
  strcat(s, date1.date);
  strcat(s, "_time.txt");
  // abrir archivo
  writeFile(SD, s, "fecha inicio\n");
  appendFile(SD, s, date1.date);
  char data[20];
  sprintf(data, " -  %d:%d:%d\n", date1.hour, date1.minute, date1.second);
  appendFile(SD, s, data);
  appendFile(SD, s, "fecha fin\n");
  appendFile(SD, s, date2.date);
  sprintf(data, " -  %d:%d:%d\n", date2.hour, date2.minute, date2.second);
  appendFile(SD, s, data);
  // cerrar archivo
  SD.end();
}

/*
      //stop task 2
  vTaskSuspend(Task2);

      // activate task 2
      vTaskResume(Task2);


BluetoothSerial SerialBT;

const int pinLed = 2;
void setup() {
  SerialBT.begin("ESP32test"); //Bluetooth device name
  pinMode(pinLed, OUTPUT);

}

void loop() {
  if (SerialBT.available()) {
    char c = SerialBT.read();
    if (c == '1') {
      digitalWrite(pinLed, HIGH);
    } else if (c == '0') {
      digitalWrite(pinLed, LOW);
    }
  }

}
*/
