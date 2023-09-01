#include <Arduino.h>

#ifndef ROBOT_TUBERIAS_RAIZ_H
#define ROBOT_TUBERIAS_RAIZ_H

template <class T>
inline Print &operator<<(Print &obj, T arg)
{
  obj.print(arg);
  return obj;
}

#endif


