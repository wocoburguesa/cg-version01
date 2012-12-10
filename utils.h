#include <math.h>
#include <unistd.h>
#include <iostream>

#ifndef UTILS_H
#define UTILS_H

using namespace std;

class Point2D{
 public:
  float x;
  float y;

  Point2D(){};

  Point2D(float x_, float y_){
    x = x_;
    y = y_;
  }
};

class Point3D{
 public:
  float x;
  float y;
  float z;

  Point3D(){};

  Point3D(float x_, float y_, float z_){
    x = x_;
    y = y_;
    z = z_;
  }

  Point3D(Point2D p, float z_){
    x = p.x;
    y = p.y;
    z = z_;
  }
};

class Equation{
 public:
  float slope;
  float intercept;
  
  Equation(){};

  Equation(float s, float i){
    slope = s;
    intercept = i;
  }
};

#endif
