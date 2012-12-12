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
  Point3D color;
  float height;
  float radius;
  vector<Point3D> corners;
  vector<Equation> equations;

 public:
  Point3D get_center(){ return center; };
  Point3D get_color(){ return color; };

  float get_height(){ return height; };
  float get_radius(){ return radius; };

  vector<Point3D> get_corners(){ return corners; };
  vector<Equation> get_equations(){ return equations; };

  void set_color(Point3D new_color){ color = new_color; };

  float distance(Point3D a, Point3D b){
    return sqrt((a.x - b.x)*(a.x - b.x) +
		(a.y - b.y)*(a.y - b.y) +
		(a.z - b.z)*(a.z - b.z));
  }

  float distance(Point2D a, Point2D b){
    return sqrt((a.x - b.x)*(a.x - b.x) +
		(a.y - b.y)*(a.y - b.y));
  }
};

#endif
