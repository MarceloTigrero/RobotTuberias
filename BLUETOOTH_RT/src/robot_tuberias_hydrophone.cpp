#include "robot_tuberias_hydrophone.h"

//Task1code: Reads an analog input on pin 2, prints the result to the Serial Monitor
void Read_Hydrophone( void * pvParameters ){
  hydrophone_data *taskHydrophone = (hydrophone_data*)pvParameters;
  int pinsensor = taskHydrophone->Pinnumber;

  //set the resolution to 12 bits (0-4096)
  analogReadResolution(taskHydrophone->resolution); //12 bits (0-4096) 3.3v
  // how calculate the resolution
    // 2^12 = 4096
    // 3.3v / 4096 = 0.00080566406v (0.80566406mv)
    // 0.80566406mv * 1000 = 0.80566406v (805.66406mv)
  while (1){
    //cada 1ms  segundo se ejecuta la funcion
    //vTaskDelay(1);// 1ms
    taskHydrophone->analogValue = analogRead(pinsensor);
    taskHydrophone->analogVolts = analogReadMilliVolts(pinsensor);
    //print_hydrophone_data(taskHydrophone);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  } 
}

void print_hydrophone_data(hydrophone_data *taskHydrophone){
  Serial << "Core :" << xPortGetCoreID() << ", analogValue: " << taskHydrophone->analogValue << ", analogVolts: " << taskHydrophone->analogVolts << "\n";
}

void obtener_hydrophone_data(hydrophone_data *taskHydrophone, int *analogValue, int *analogVolts){
  *analogValue = taskHydrophone->analogValue;
  *analogVolts = taskHydrophone->analogVolts;
}

void obtener_hydrophone_data( char *buffer, hydrophone_data *data ){
  sprintf( buffer, "%d,%d", data->analogValue, data->analogVolts );
}