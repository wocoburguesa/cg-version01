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
  float steer_angle;
  float inclination_angle;
  float roll_angle;
  Point3D mov_vector;

 public:
  MovingObject(){};

  MovingObject(Point3D init_center,
	       float max,
	       float accel,
	       float friction,
	       float car_size,
	       float steer,
	       float inclination,
	       float roll){

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

    // setting initial movement vector
    mov_vector = make_mov_vector(steer_angle, inclination_angle);

    // setting initial corners
    for(int i = 0; i < 8; ++i)
      corners.push_back(Point3D());
    set_corners();

    // setting initial equations
    // NEW ON UPDATE: not updated yet for 3d
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

  /********** UTILS **********/
  Point3D make_mov_vector(float theta, float phi){
    return Point3D(cos(theta*PI/180),
		   sin(theta*PI/180)*cos(phi*PI/180),
		   sin(phi*PI/180));
  }

  /********** GETTERS **********/
  Point3D get_top_left(){ return corners[0]; };
  Point3D get_bottom_left(){ return corners[1]; };
  Point3D get_bottom_right(){ return corners[2]; };
  Point3D get_top_right(){ return corners[3]; };

  float get_speed(){ return speed; };
  float get_steer_angle(){ return steer_angle; };
  float get_max_speed(){ return max_speed; };
  float get_size(){ return radius; };
  float get_acceleration(){ return acceleration; };
  float get_friction(){ return friction_constant; };
  Point3D get_mov_vector(){ return mov_vector; };
  /********** GETTERS **********/

  /********** UPDATERS  **********/
  void set_corners(){
    set_top_left();
    set_top_right();
    set_bottom_left();
    set_bottom_right();
    /*    cout << "Center: (" << center.x << ", " << center.y << ", " << center.z << ")" << endl;
    for(int i = 0; i < 8; ++i)
    cout << "Corner #" << i << ": (" << corners[i].x << ", " << corners[i].y << ", " << corners[i].z << ")" << endl;*/
  }

  // NEW ON UPDATE: all of these need checking on direction, probably wont work
  // when there is inclination
  void set_top_left(){
    float ang = (steer_angle + TOP_ANGLE);
    POINT direction(cos(ang*PI/180.0f), sin(ang*PI/180.0f));

    float denom = sqrt(direction.first*direction.first +
		       direction.second*direction.second);
    float factor = radius / denom;

    // going 3D
    float z_diff = radius * (cos(TOP_ANGLE*PI/180) / sin(TOP_ANGLE*PI/180)) / 2.0f;
    
    corners[0].x = center.x + (direction.first * factor);
    corners[0].y = center.y + (direction.second * factor);
    corners[0].z = center.z + z_diff;

    corners[4].x = center.x + (direction.first * factor);
    corners[4].y = center.y + (direction.second * factor);
    corners[4].z = center.z - z_diff;
  }

  void set_top_right(){
    float ang = (steer_angle + TOP_ANGLE + 300);
    POINT direction(cos(ang*PI/180), sin(ang*PI/180));
    
    float denom = sqrt(direction.first*direction.first +
		       direction.second*direction.second);
    float factor = radius / denom;

    // going 3D
    float z_diff = radius * (cos(TOP_ANGLE*PI/180) / sin(TOP_ANGLE*PI/180)) / 2.0f;
    
    corners[3].x = center.x + (direction.first * factor);
    corners[3].y = center.y + (direction.second * factor);
    corners[3].z = center.z + z_diff;

    corners[7].x = center.x + (direction.first * factor);
    corners[7].y = center.y + (direction.second * factor);
    corners[7].z = center.z - z_diff;
  }

  void set_bottom_left(){
    float ang = (steer_angle + 120 + TOP_ANGLE);
    POINT direction(cos(ang*PI/180), sin(ang*PI/180));
    
    float denom = sqrt(direction.first*direction.first +
		       direction.second*direction.second);
    float factor = radius / denom;

    // going 3D
    float z_diff = radius * (cos(TOP_ANGLE*PI/180) / sin(TOP_ANGLE*PI/180)) / 2.0f;

    corners[1].x = center.x + (direction.first * factor);
    corners[1].y = center.y + (direction.second * factor);
    corners[1].z = center.z + z_diff;

    corners[5].x = center.x + (direction.first * factor);
    corners[5].y = center.y + (direction.second * factor);
    corners[5].z = center.z - z_diff;
  }

  void set_bottom_right(){
    float ang = (steer_angle + TOP_ANGLE + 180);
    POINT direction(cos(ang*PI/180), sin(ang*PI/180));
    
    float denom = sqrt(direction.first*direction.first +
		       direction.second*direction.second);
    float factor = radius / denom;

    // going 3D
    float z_diff = radius * (cos(TOP_ANGLE*PI/180) / sin(TOP_ANGLE*PI/180)) / 2.0f;
    
    corners[2].x = center.x + (direction.first * factor);
    corners[2].y = center.y + (direction.second * factor);
    corners[2].z = center.z + z_diff;

    corners[6].x = center.x + (direction.first * factor);
    corners[6].y = center.y + (direction.second * factor);
    corners[6].z = center.z - z_diff;
  }

  void process_friction(){
    if(speed > 0)
      speed -= current_friction;
    else if(speed < 0)
      speed += current_friction;
  }

  void update_position(){
    // NEW ON UPDATE: check that it works with inclination (probably not)
    float denom = sqrt(mov_vector.x*mov_vector.x +
		       mov_vector.y*mov_vector.y);
    float factor = speed / denom;
    center.x += mov_vector.x * factor;
    center.y += mov_vector.y * factor;
    
    // update corners
    set_corners();

    // update equations
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
	equations[i] = pair<float, float>(slope, intercept);
      }
      else{
	equations[i] = pair<float, float>(x1, INF);
      }
    }*/
  }
  /********** UPDATERS **********/

  /********** MOVEMENT HANDLERS **********/
  // NEW ON UPDATE: all of this needs updating to 3d
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
    steer_angle += PLAYER_TURNING_SPEED;
    mov_vector.x = cos(steer_angle*PI/180);
    mov_vector.y = sin(steer_angle*PI/180);
  }

  void turn_right(){
    steer_angle -= PLAYER_TURNING_SPEED;
    mov_vector.x = cos(steer_angle*PI/180);
    mov_vector.y = sin(steer_angle*PI/180);
  }

  // virtual void bump() = 0;
  /********** MOVEMENT HANDLERS **********/

  //  virtual void update() = 0;
};

#endif
