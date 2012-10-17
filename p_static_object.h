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
  StaticObject(vector< POINT > &c, bool is_pickup){
    // determining if it is a pickup item
    pickup = is_pickup;
    
    // setting the corners vector
    corners = c;

    // setting original x and y
    x = (corners[0].first + corners[3].first)/2;
    y = (corners[0].second + corners[1].second)/2;

    // setting radius according to corners
    radius = sqrt((corners[0].first - x)*(corners[0].first - x) +
		  (corners[0].second - y)*(corners[0].second - y));
    for(int i = 1; i < corners.size(); ++i){
      float buff = sqrt((corners[i].first - x)*(corners[i].first - x) +
			(corners[i].second - y)*(corners[i].second - y));
      if(buff > radius)
	radius = buff;
    }

    // settings equations of the borders
    for(int i = 0; i < corners.size(); ++i){
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
    }
  }

  bool is_pickup(){ return pickup; }
};

#endif
