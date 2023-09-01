#include "robot_tuberias_raiz.h"

struct hydrophone_data {
  int Pinnumber;
  uint8_t resolution;
  unsigned long analogValue;
  unsigned long analogVolts;
};



void Read_Hydrophone( void * pvParameters );
void print_hydrophone_data( hydrophone_data *data );
void obtener_hydrophone_data( char *buffer, hydrophone_data *data );

