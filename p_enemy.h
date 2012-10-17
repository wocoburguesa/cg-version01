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
  int flare_shown;
  int shot_cooldown;

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
  bool get_flare_shown(){ return flare_shown;} 
  /********** GETTERS **********/

  /********** SETTERS **********/
  void flip_shot_fired(){ shot_fired = !shot_fired; }
  /********** SETTERS **********/

  void update(){
    if(flare_shown > 0)
      flare_shown--;
    else;
    if(shot_cooldown > 0)
      shot_cooldown--;
    else;
    
    process_friction();
    update_position();
    if(in_transit){
      if(distance(POINT(x, y), destination) > 3.0f){
	move_forward();
	float current_slope = (mov_vector.second)/(mov_vector.first);
	float current_intercept = y - current_slope * x;
	
	float ortho_slope = - (1 / current_slope);
	float ortho_intercept =
	  destination.second - destination.first * ortho_slope;

	float projection_x = (ortho_intercept - current_intercept)/
	  (current_slope - ortho_slope);
	float projection_y = current_slope * projection_x + current_intercept;

	float distance_to_projection =
	  distance(destination, POINT(projection_x, projection_y));

	if(distance_to_projection > 0.001f){
	  float intercept_left =
	    corners[0].second - corners[0].first * current_slope;
	  float intercept_right =
	    corners[3].second - corners[3].first * current_slope;

	  float projection_left_x = (ortho_intercept - intercept_left)/
	    (current_slope - ortho_slope);
	  float projection_left_y = current_slope * projection_left_x +
	    intercept_left;
	  float projection_right_x = (ortho_intercept - intercept_right)/
	    (current_slope - ortho_slope);
	  float projection_right_y = current_slope * projection_right_x +
	    intercept_right;

	  POINT left(projection_left_x, projection_left_y);
	  POINT right(projection_right_x, projection_right_y);
	  
	  if(distance(destination, left) > distance(destination, right))
	    turn_right();
	  else
	    turn_left();
	}
	else if(shot_cooldown <= 0)
	  shoot();
      }
      else
	in_transit = 0;
    }
    else if(speed > 0)
      move_back();
  }

  void shoot(){
    shot_fired = 1;
    shot_cooldown = ENEMY_SHOT_COOLDOWN;
    flare_shown = FLARE_SHOWN_CONSTANT;
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
    update_position();
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
