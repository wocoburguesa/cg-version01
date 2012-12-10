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
  StaticObject(vector<Point3D> &c, float hght, bool is_pickup){
    // determining if it is a pickup item
    pickup = is_pickup;
    
    // setting the corners vector
    corners = c;

    // setting original x and y
    center = Point3D((corners[0].x + corners[3].x)/2.0f,
		     (corners[0].y + corners[3].y)/2.0f,
		     (GROUND_LEVEL+hght)/2.0f);

    // setting height
    height = hght;

    // setting radius according to corners
    radius = sqrt((corners[0].x - center.x)*(corners[0].x - center.x) +
		  (corners[0].y - center.y)*(corners[0].y - center.y));
    for(int i = 1; i < corners.size(); ++i){
      float buff = sqrt((corners[i].x - center.x)*(corners[i].x - center.x) +
			(corners[i].y - center.y)*(corners[i].y - center.y));
      if(buff > radius)
	radius = buff;
    }

    // settings equations of the borders
    for(int i = 0; i < corners.size(); ++i){
      float slope, intercept, x1, y1, x2, y2;
      x1 = corners[i].x;
      y1 = corners[i].y;
      x2 = corners[(i+1)%corners.size()].x;
      y2 = corners[(i+1)%corners.size()].y;
      if(x1 != x2){
	slope = (y1 - y2)/(x1 - x2);
	intercept = y1 - slope * x1;
	equations.push_back(Equation(slope, intercept));
      }
      else{
	equations.push_back(Equation(x1, INF));
      }
    }
  }

  bool is_pickup(){ return pickup; }
};

#endif
