#include <math.h>
#include <unistd.h>
#include <iostream>
#include "p_moving_object.h"

#ifndef P_PROJECTILE_H
#define P_PROJECTILE_H

using namespace std;

class Projectile : public MovingObject{
 public:
  Projectile(Point3D center_init,
	     float max,
	     float accel,
	     float friction,
	     float size,
	     float steer,
	     float inclination,
	     float roll){

    // setting initial position
    center = center_init;

    speed = PROJECTILE_BASE_SPEED;

    acceleration = PROJECTILE_ACCELERATION;

    max_speed = PROJECTILE_MAX_SPEED;

    // setting friction constant and initial friction
    friction_constant = current_friction = friction;

    // setting radius
    radius = size;

    // setting initial angle (counter-clockwise)
    // theta: horizontal rotation (over z-axis)
    // phi: roll (over y-axis)
    steer_angle = steer;
    inclination_angle = inclination;
    roll_angle = roll;

    // setting initial movement vector
    mov_vector = make_mov_vector(steer_angle, inclination_angle);


    // setting initial corners
    for(int i = 0; i < 8; ++i)
      corners.push_back(Point3D());
    set_corners();

    // setting initial equations
    // NEW ON UPDATE: needs updating
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

  void set_position(MovingObject * shooter){
    
  }
  
  // NEW ON UPDATE: needs updating
  /*  void update(){
    process_friction();
    update_position();
    move_forward();
    }*/

  void bump(){
    return;
  }
};

#endif
