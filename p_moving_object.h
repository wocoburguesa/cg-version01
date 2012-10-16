#include <math.h>
#include <unistd.h>
#include <iostream>
#include "p_object.h"

#ifndef P_MOVING_OBJECT_H
#define P_MOVING_OBJECT_H

using namespace std;

class MovingObject : public Object{
 protected:
  float acceleration;
  float speed;
  float max_speed;
  float friction_constant;
  float current_friction;
  float angle;
  POINT mov_vector;

 public:
  MovingObject(){};

  MovingObject(float x_init,
	       float y_init,
	       float max,
	       float accel,
	       float friction,
	       float car_size,
	       float ang=90.0f){

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
  POINT get_top_left(){ return corners[0]; };
  POINT get_bottom_left(){ return corners[1]; };
  POINT get_bottom_right(){ return corners[2]; };
  POINT get_top_right(){ return corners[3]; };

  float get_speed(){ return speed; };
  float get_angle(){ return angle; };
  float get_max_speed(){ return max_speed; };
  float get_size(){ return radius; };
  float get_acceleration(){ return acceleration; };
  float get_friction(){ return friction_constant; };
  POINT get_mov_vector(){ return mov_vector; };
  /********** GETTERS **********/

  /********** UPDATERS  **********/
  void set_top_left(){
    float ang = (angle + TH);
    POINT direction(cos(ang*PI/180.0f), sin(ang*PI/180.0f));

    float denom = sqrt(direction.first*direction.first +
		       direction.second*direction.second);
    float factor = radius / denom;
    corners[0].first = x + (direction.first * factor);
    corners[0].second = y + (direction.second * factor);
  }

  void set_top_right(){
    float ang = (angle + TH + 300);
    POINT direction(cos(ang*PI/180), sin(ang*PI/180));
    
    float denom = sqrt(direction.first*direction.first +
		       direction.second*direction.second);
    float factor = radius / denom;
    corners[3].first = x + (direction.first * factor);
    corners[3].second = y + (direction.second * factor);
  }

  void set_bottom_left(){
    float ang = (angle + 120 + TH);
    POINT direction(cos(ang*PI/180), sin(ang*PI/180));
    
    float denom = sqrt(direction.first*direction.first +
		       direction.second*direction.second);
    float factor = radius / denom;
    corners[1].first = x + (direction.first * factor);
    corners[1].second = y + (direction.second * factor);
  }

  void set_bottom_right(){
    float ang = (angle + TH + 180);
    POINT direction(cos(ang*PI/180), sin(ang*PI/180));
    
    float denom = sqrt(direction.first*direction.first +
		       direction.second*direction.second);
    float factor = radius / denom;
    corners[2].first = x + (direction.first * factor);
    corners[2].second = y + (direction.second * factor);
  }

  void process_friction(){
    if(speed > 0)
      speed -= current_friction;
    else if(speed < 0)
      speed += current_friction;
  }

  void update_position(){
    float denom = sqrt(mov_vector.first*mov_vector.first +
		       mov_vector.second*mov_vector.second);
    float factor = speed / denom;
    x += mov_vector.first * factor;
    y += mov_vector.second * factor;
    
    // update corners
    set_top_left();
    set_bottom_left();
    set_bottom_right();
    set_top_right();

    // update equations
    for(int i = 0; i < corners.size(); ++i){
      float slope, intercept, x1, y1, x2, y2;
      x1 = corners[i].first;
      y1 = corners[i].second;
      x2 = corners[(i+1)%corners.size()].first;
      y2 = corners[(i+1)%corners.size()].second;
      if(x1 != x2){
	slope = (y1 - y2)/(x1 - x2);
	intercept = y1 - slope * x1;
	equations[i] = pair<float, float>(slope, intercept);
      }
      else{
	equations[i] = pair<float, float>(x1, INF);
      }
    }
  }
  /********** UPDATERS **********/

  /********** MOVEMENT HANDLERS **********/
  void move_forward(){
    if(speed < max_speed)
      speed += acceleration;
    else;
    update_position();
  }

  void move_back(){
    if(speed < max_speed)
      speed -= acceleration;
    else;
    update_position();
  }

  void turn_left(){
    angle = (angle + PLAYER_TURNING_SPEED);
    mov_vector.first = cos(angle*PI/180);
    mov_vector.second = sin(angle*PI/180);
  }

  void turn_right(){
    angle = (angle - PLAYER_TURNING_SPEED);
    mov_vector.first = cos(angle*PI/180);
    mov_vector.second = sin(angle*PI/180);
  }

  virtual void bump() = 0;
  /********** MOVEMENT HANDLERS **********/

  virtual void update() = 0;
};

#endif
