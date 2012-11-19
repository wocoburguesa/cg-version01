#include <math.h>
#include <unistd.h>
#include <iostream>
#include "p_moving_object.h"

#ifndef P_PLAYER_H
#define P_PLAYER_H

using namespace std;

class PlayerHandler : public MovingObject{
 private:
  float health;
  bool *pushed;
  bool shot_fired;
  int flare_shown;
  int shot_cooldown;

  int uber_hit_count;
  int uber_reset;

 public:
  PlayerHandler(Point3D init_center,
		float max,
		float accel,
		float friction,
		float car_size,
		float steer,
		float inclination,
		float roll,
		float health_init){
    // setting initial position
    center = init_center;
    
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
    // theta: horizontal rotation (over z-axis)
    // phi: roll (over y-axis)
    steer_angle = steer;
    inclination_angle = inclination;
    roll_angle = roll;

    // setting initial health
    health = health_init;

    // setting initial status of shot_fired
    shot_fired = 0;

    // setting initial status of uber shot
    uber_hit_count = 0;
    uber_reset = UBER_RESET_CONSTANT;

    //setting initial status of pushed array
    pushed = new bool[4];
    pushed[0] = pushed[1] = pushed[2] = pushed[3] = 0;

    // setting initial movement vector
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

  /********** GETTERS **********/
  bool get_shot_fired(){ return shot_fired; }
  bool get_flare_shown(){ return flare_shown; }

  int get_uber_hit_count(){ return uber_hit_count; }

  float get_life(){ return health; }
  float get_remaining_life_pct(){
    return health / PLAYER_STARTING_HEALTH;
  }
  /********** GETTERS **********/

  /********** SETTERS **********/
  void flip_shot_fired(){ shot_fired = !shot_fired; }
  void reset_uber_count(){ uber_hit_count = 0; uber_reset = UBER_RESET_CONSTANT;}
  void reset_health(){ health = PLAYER_STARTING_HEALTH; }
  void set_center(Point3D new_center){ center = new_center; }
  void set_steer_angle(float new_steer_angle){
    steer_angle = new_steer_angle;
    mov_vector = make_mov_vector(steer_angle, inclination_angle);
  }
  /********** SETTERS **********/

  /********** BUTTON PRESS CONTROLLERS **********/
  void push_forward(){ pushed[0] = 1; uber_hit_count++; };
  void push_back(){ pushed[2] = 1; uber_hit_count++; };
  void push_left(){ pushed[1] = 1; };
  void push_right(){ pushed[3] = 1; };
  void push_space(){ shot_fired = 1; flare_shown = FLARE_SHOWN_CONSTANT; };

  void release_forward(){ pushed[0] = 0; };
  void release_back(){ pushed[2] = 0; };
  void release_left(){ pushed[1] = 0; };
  void release_right(){ pushed[3] = 0; };
  /********** BUTTON PRESS CONTROLLERS **********/

  // NEW ON UPDATE: needs updating
  /*  void update(){
    if(flare_shown > 0)
      flare_shown--;
    else;

    if(uber_reset > 0)
      uber_reset--;
    else{
      uber_reset = 10000;
      uber_hit_count = 0;
    }
    
    process_friction();
    update_position();
    if(pushed[0]){
      if(speed < 0)
	current_friction = friction_constant*5;
      else{
	current_friction = friction_constant;
	move_forward();
      }
    }
    else;
    if(pushed[2]){
      if(speed > 0)
	current_friction = friction_constant*5;
      else{
	current_friction = friction_constant;
	move_back();
      }
    }
    else;
    if(pushed[1])
      if(speed < 0)
	turn_right();
      else
	turn_left();
    else;
    if(pushed[3])
      if(speed < 0)
	turn_left();
      else
	turn_right();
    else;
    }*/

  void register_hit(){
    health -= PROJECTILE_STRENGTH/3.0f;
  }

  void register_crash(){
    health -= CRASH_DAMAGE;
  }

  bool vital_signs(){
    if(health <= 0.0f)
      return 0;
    return 1;
  }

  // NEW ON UPDATE: needs updating
  /*  void bump(){
    speed = -(speed * 0.8f);
    update_position();
    update_position();
    update_position();
    }*/
};

#endif
