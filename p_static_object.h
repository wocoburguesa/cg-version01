#include <math.h>
#include <unistd.h>
#include <iostream>
#include "p_object.h"

#ifndef P_STATIC_OBJECT_H
#define P_STATIC_OBJECT_H

using namespace std;

class StaticObject : public Object{
 private:
  bool pickup;
  
 public:
  StaticObject(vector<Point3D> &c, bool is_pickup){
    // determining if it is a pickup item
    pickup = is_pickup;
    
    // setting the corners vector
    corners = c;

    // setting original x and y
    center.x = (corners[0].x + corners[3].x)/2.0f;
    center.y = (corners[0].y + corners[1].y)/2.0f;
    center.z = (corners[0].z + corners[4].z)/2.0f;

    // setting radius according to corners
    radius = distance(corners[0], center);

    for(int i = 1; i < corners.size(); ++i){
      float buff = distance(corners[i], center);
      if(buff > radius)
	radius = buff;
    }

    // equations
    // NOT YET UPDATED FOR 3D!!!!!!
    /*    for(int i = 0; i < corners.size(); ++i){
      float slope, intercept, x1, y1, x2, y2;
      x1 = corners[i].first;
      y1 = corners[i].second;
      x2 = corners[(i+1)%corners.size()].first;
      y2 = corners[(i+1)%corners.size()].second;
      if(x1 != x2){
	slope = (y1 - y2)/(x1 - x2);
	intercept = y1 - slope * x1;
	equations.push_back(pair<float, float>(slope, intercept));
      }
      else{
	equations.push_back(pair<float, float>(x1, INF));
      }
      }*/
  }

  bool is_pickup(){ return pickup; }
};

#endif
