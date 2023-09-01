#include "robot_tuberias_raiz.h"
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

struct robot_tuberias_i2c_data
{
    double accelX;
    double accelY;
    double accelZ;
    double gyroX;
    double gyroY;
    double gyroZ;
    double temp;
};


void init_mpu(Adafruit_MPU6050 &mpu);
void printconfig_mpu(Adafruit_MPU6050 &mpu);
void get_mpu_data(Adafruit_MPU6050 &mpu);
void print_mpu_data(sensors_event_t a, sensors_event_t g, sensors_event_t temp);
void catch_mpu_data(Adafruit_MPU6050 &mpu,robot_tuberias_i2c_data *robot_tuberias_i2c);