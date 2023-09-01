#include "robot_tuberias_i2c.h"

void init_mpu(Adafruit_MPU6050 &mpu){
    if (!mpu.begin()) {
        Serial << "Failed to find MPU6050 chip\n";
        while (1) {
        delay(10);
        }
    }
    Serial << "MPU6050 Found!\n";
    mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
    mpu.setGyroRange(MPU6050_RANGE_500_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);
    delay(100);

}

void printconfig_mpu(Adafruit_MPU6050 &mpu){
  Serial << "\nAccelerometer range set to: " ;
  switch (mpu.getAccelerometerRange()) {
  case MPU6050_RANGE_2_G:
    Serial << "+-2G" ;
    break;
  case MPU6050_RANGE_4_G:
    Serial << "+-4G" ;
    break;
  case MPU6050_RANGE_8_G:
    Serial << "+-8G" ;
    break;
  case MPU6050_RANGE_16_G:
    Serial << "+-16G" ;
    break;
  }
  Serial << "\nGyro range set to: " ;
  switch (mpu.getGyroRange()) {
  case MPU6050_RANGE_250_DEG:
    Serial << "+- 250 deg/s" ;
    break;
  case MPU6050_RANGE_500_DEG:
    Serial << "+- 500 deg/s" ;
    break;
  case MPU6050_RANGE_1000_DEG:
    Serial << "+- 1000 deg/s" ;
    break;
  case MPU6050_RANGE_2000_DEG:
    Serial << "+- 2000 deg/s" ;
    break;
  }
  Serial << "\nFilter bandwidth set to: " ;
  switch (mpu.getFilterBandwidth()) {
  case MPU6050_BAND_260_HZ:
    Serial << "260 Hz" ;
    break;
  case MPU6050_BAND_184_HZ:
    Serial << "184 Hz" ;
    break;
  case MPU6050_BAND_94_HZ:
    Serial << "94 Hz" ;
    break;
  case MPU6050_BAND_44_HZ:
    Serial << "44 Hz" ;
    break;
  case MPU6050_BAND_21_HZ:
    Serial << "21 Hz" ;
    break;
  case MPU6050_BAND_10_HZ:
    Serial << "10 Hz" ;
    break;
  case MPU6050_BAND_5_HZ:
    Serial << "5 Hz" ;
    break;
  }
  Serial << "\n" ;
  delay(100);
}

void get_mpu_data(Adafruit_MPU6050 &mpu){
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);
    print_mpu_data(a, g, temp);
}


void print_mpu_data(sensors_event_t a, sensors_event_t g, sensors_event_t temp){
  Serial << "Acceleration X: " << a.acceleration.x << ", Y: " << a.acceleration.y << ", Z: " << a.acceleration.z << " [m/s^2]\n";
  Serial << "Rotation X: " << g.gyro.x << ", Y: " << g.gyro.y << ", Z: " << g.gyro.z << " [rad/s]\n";
  Serial << "Temperature: " << temp.temperature << " [degC]\n";
  Serial << "\n" ;
  delay(100);
}

void catch_mpu_data(Adafruit_MPU6050 &mpu, robot_tuberias_i2c_data *robot_tuberias_i2c){
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  robot_tuberias_i2c->accelX = a.acceleration.x;
  robot_tuberias_i2c->accelY = a.acceleration.y;
  robot_tuberias_i2c->accelZ = a.acceleration.z;
  robot_tuberias_i2c->gyroX = g.gyro.x;
  robot_tuberias_i2c->gyroY = g.gyro.y;
  robot_tuberias_i2c->gyroZ = g.gyro.z;
  robot_tuberias_i2c->temp = temp.temperature;
}
