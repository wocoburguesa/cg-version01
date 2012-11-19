#include <math.h>
#include <unistd.h>
#include <iostream>

#ifndef UTILS_H
#define UTILS_H

using namespace std;

class Point3D{
 public:
  float x;
  float y;
  float z;

  Point3D(){
    x = y = z = 0.0f;
  }

  Point3D(float x_, float y_, float z_){
    x = x_;
    y = y_;
    z = z_;
  }
};

#endif
