#include <math.h>
#include <unistd.h>
#include <iostream>
#include "p_moving_object.h"

#ifndef P_PROJECTILE_H
#define P_PROJECTILE_H

using namespace std;

class Projectile : public MovingObject{
 public:
  Projectile(float x_init,
	     float y_init,
	     float hght,
	     float max,
	     float accel,
	     float friction,
	     float size,
	     float ang=90.0f){
    // setting initial position
    x = x_init;
    y = y_init;

    //setting height
    height = hght;

    speed = PROJECTILE_BASE_SPEED;

    acceleration = PROJECTILE_ACCELERATION;

    max_speed = PROJECTILE_MAX_SPEED;

    // setting friction constant and initial friction
    friction_constant = current_friction = friction;

    // setting radius
    radius = size;

    // setting initial angle (counter-clockwise)
    angle = ang;

    // setting initial movement vector
    mov_vector = POINT(cos(angle*PI/180),
		       sin(angle*PI/180));

    // setting initial corners
    corners.push_back(POINT(0.0f, 0.0f));
    corners.push_back(POINT(0.0f, 0.0f));
    corners.push_back(POINT(0.0f, 0.0f));
    corners.push_back(POINT(0.0f, 0.0f));
    set_top_left();
    set_bottom_left();
    set_bottom_right();
    set_top_right();

    // setting initial equations
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

  void set_position(MovingObject * shooter){
    
  }
  
  void update(){
    process_friction();
    update_position();
    move_forward();
  }

  void bump(){
    return;
  }
};

#endif
