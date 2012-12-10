#include <math.h>
#include <unistd.h>
#include <iostream>
#include "constants.h"

#ifndef P_OBJECT_H
#define P_OBJECT_H

using namespace std;

class Object{
 protected:
  float x;
  float y;
  float height;
  float radius;
  vector<POINT> corners;
  vector<POINT> equations;

 public:
  POINT get_x_y(){ return POINT(x, y); };
  float get_height(){ return height; };
  float get_radius(){ return radius; };

  vector< POINT > get_corners(){ return corners; };
  vector<POINT> get_equations(){ return equations; };

  float distance(POINT a, POINT b){
    return sqrt((a.first - b.first)*(a.first - b.first) +
		(a.second - b.second)*(a.second - b.second));
  }
};

#endif
