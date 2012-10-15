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

 public:
  PlayerHandler(float x_init,
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

    //setting initial status of pushed array
    pushed = new bool[4];
    pushed[0] = pushed[1] = pushed[2] = pushed[3] = 0;

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
  bool get_flare_show(){ return flare_shown; }

  float get_life(){ return health; }
  float get_remaining_life_pct(){
    return health / PLAYER_STARTING_HEALTH;
  }

  /********** GETTERS **********/

  /********** SETTERS **********/
  void flip_shot_fired(){ shot_fired = !shot_fired; }
  /********** SETTERS **********/

  /********** BUTTON PRESS CONTROLLERS **********/
  void push_forward(){ pushed[0] = 1; };
  void push_back(){ pushed[2] = 1; };
  void push_left(){ pushed[1] = 1; };
  void push_right(){ pushed[3] = 1; };
  void push_space(){ shot_fired = 1; flare_shown = 200; };

  void release_forward(){ pushed[0] = 0; };
  void release_back(){ pushed[2] = 0; };
  void release_left(){ pushed[1] = 0; };
  void release_right(){ pushed[3] = 0; };
  /********** BUTTON PRESS CONTROLLERS **********/

  void update(){
    if(flare_shown > 0)
      flare_shown--;
    else;
    
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
  }

  void register_hit(){
    health -= PROJECTILE_STRENGTH;
  }

  void register_crash(){
    health -= CRASH_DAMAGE;
  }

  bool vital_signs(){
    if(health <= 0.0f)
      return 0;
    return 1;
  }

  void bump(){
    speed = -(speed * 0.8f);
    update_position();
  }
};

#endif
