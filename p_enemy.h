#include <math.h>
#include <unistd.h>
#include <iostream>
#include "p_moving_object.h"

#ifndef P_ENEMY_H
#define P_ENEMY_H

using namespace std;

class Enemy : public MovingObject{
 private:
  POINT destination;
  POINT target_vector;
  bool in_transit;

  float health;
  bool shot_fired;

 public:
  Enemy(float x_init,
	float y_init,
	float max,
	float accel,
	float friction,
	float car_size,
	float ang,
	float health_init){
    // setting initial position
    x = x_init;
    y = y_init;

    // setting initial speed
    speed = 0.0f;

    // setting rate of speed increase
    acceleration = accel;

    // setting maximum possible speed
    max_speed = max;

    // setting friction constant and initial friction
    friction_constant = friction;
    current_friction = friction;
    
    // setting radius
    radius = car_size;

    // setting initial angle (counter-clockwise)
    angle = ang;

    // setting initial health
    health = health_init;

    // setting initial status of shot_fired
    shot_fired = 0;

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

  /********** GETTERS **********/
  bool get_shot_fired(){ return shot_fired; }
  /********** GETTERS **********/

  /********** SETTERS **********/
  void flip_shot_fired(){ shot_fired = !shot_fired; }
  /********** SETTERS **********/

  void update(){
    process_friction();
    update_position();
    if(in_transit){
      if(distance(POINT(x, y), destination) > 3.0f){
	move_forward();
	if(distance(mov_vector, target_vector) > 0.0001f){
	  float current_slope = (destination.second - y)/(destination.first - x);
	  if(abs(current_slope - (target_vector.second/target_vector.first)) >
	     0.01f){
	    float left_angle = angle + 0.05;
	    float right_angle = angle - 0.05;
	    if(distance(POINT(cos(left_angle*PI/180), sin(left_angle*PI/180)),
			target_vector) <
	       distance(POINT(cos(right_angle*PI/180), sin(right_angle*PI/180)),
			target_vector))
	      turn_left();
	    else
	      turn_right();
	  }
	}
      }
      else
	in_transit = 0;
    }
    else if(speed > 0)
      move_back();
  }

  void register_hit(){
    health -= PROJECTILE_STRENGTH;
  }

  bool vital_signs(){
    if(health <= 0.0f)
      return 0;
    return 1;
  }

  float get_life(){ return health; }

  void bump(){
    speed = -(speed * 0.4f);
    turn_left();
    update_position();
  }

  void go_to(POINT target){
    destination = target;
    in_transit = 1;
    float norm = sqrt((target.first * target.first) +
		      (target.second * target.second));
    target_vector = POINT(target.first/norm, target.second/norm);
  }

};

#endif
