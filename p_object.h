#include <math.h>
#include <unistd.h>
#include <iostream>
#include "constants.h"

#ifndef P_OBJECT_H
#define P_OBJECT_H

using namespace std;

class Object{
 protected:
  Point3D center;
  float radius;
  vector<Point3D> corners;
  vector<Point3D> equations;

 public:
  Point3D get_center(){ return center; };

  float get_radius(){ return radius; };

  vector<Point3D> get_corners(){ return corners; };

  vector<Point3D> get_equations(){ return equations; };

  float distance(Point3D a, Point3D b){
    return sqrt((a.x - b.x)*(a.x - b.x) +
		(a.y - b.y)*(a.y - b.y) + 
		(a.z - b.z)*(a.z - b.z));
  }
};

#endif
