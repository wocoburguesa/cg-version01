#include <math.h>
#include <unistd.h>
#include <iostream>
#include "constants.h"
#include "p_moving_object.h"

#ifndef CAMERA_HANDLER_H
#define CAMERA_HANDLER_H

using namespace std;

class CameraHandler{
 protected:
  Point3D center;
  MovingObject * focus_object;
  Point3D focus_point;
  Point3D up_vector;

  float camera_distance;
  float friction_constant;
  float current_friction;
  float acceleration;
  float speed;
  float max_speed;
  bool * pushed;

 public:
  CameraHandler(MovingObject * focus_obj){
    camera_distance = DEFAULT_CAMERA_DISTANCE;
    
    focus_object = focus_obj;
    focus_point = Point3D(focus_object->get_center().x,
			  focus_object->get_center().y,
			  focus_object->get_center().z);

    up_vector = Point3D(UP_X, UP_Y, UP_Z);
    center = Point3D(focus_object->get_center().x + CENTER_DIFF_X,
		     focus_object->get_center().y + camera_distance,
		     focus_object->get_center().z + CAMERA_HEIGHT);

    //setting initial status of pushed array
    pushed = new bool[4];
    pushed[0] = pushed[1] = pushed[2] = pushed[3] = 0;

    // setting friction constant and initial friction
    friction_constant = CAMERA_FRICTION;
    current_friction = CAMERA_FRICTION;

    // setting initial speed
    speed = 0.0f;
    max_speed = CAMERA_MAX_SPEED;

    // setting rate of speed increase
    acceleration = CAMERA_ACCELERATION;
  }

  Point3D get_center(){ return center; };
  Point3D get_focus(){ return focus_point; };
  Point3D get_up(){ return up_vector; };

  /********** BUTTON PRESS CONTROLLERS **********/
  void push_forward(){ pushed[0] = 1; };
  void push_back(){ pushed[2] = 1; };
  void push_left(){ pushed[1] = 1; };
  void push_right(){ pushed[3] = 1; };

  void release_forward(){ pushed[0] = 0; speed = 0; };
  void release_back(){ pushed[2] = 0; speed = 0; };
  void release_left(){ pushed[1] = 0; };
  void release_right(){ pushed[3] = 0; };
  /********** BUTTON PRESS CONTROLLERS **********/

  void process_friction(){
    if(speed > 0)
      speed -= current_friction;
    else if(speed < 0)
      speed += current_friction;
  }

  void update(){
    process_friction();
    update_position();
    if(pushed[0]){
      if(speed < 0.0f)
	current_friction = friction_constant*5;
      else{
	current_friction = friction_constant;
	move_forward();
      }
    }
    else;
    if(pushed[2])
      if(speed > 0.0f)
	current_friction = friction_constant*5;
      else{
	current_friction = friction_constant;
	move_back();
      }
    else;
  }

  void update_position(){
    up_vector = Point3D(focus_object->get_mov_vector().x,
			focus_object->get_mov_vector().y,
			UP_Z);

    focus_point = Point3D(focus_object->get_center().x,
			  focus_object->get_center().y,
			  focus_object->get_center().z);

    // cambiar CENTER_DIFF_Y a camera distance o algo
    Point3D focus_center = focus_object->get_center();
    Point2D focus_vector = focus_object->get_mov_vector();
    center = Point3D(focus_center.x + focus_vector.x*camera_distance,
		     focus_center.y + focus_vector.y*camera_distance,
		     focus_center.z + CAMERA_HEIGHT - camera_distance*0.8f);
  }

  /********** MOVEMENT HANDLERS **********/
  /*
    Comparison signs for camera distance are inverted because it's
    a negative value
   */
  void move_forward(){
    if(camera_distance <= MIN_CAMERA_DISTANCE){
      if(speed < max_speed)
	speed += acceleration;
      else;
      camera_distance += speed;
    }
    else
      speed = 0.0f;
    update_position();
  }

  void move_back(){
    if(camera_distance >= MAX_CAMERA_DISTANCE){
      if(speed < max_speed)
	speed -= acceleration;
      else;
      camera_distance += speed;
    }
    else
      speed = 0.0f;
    update_position();
  }
  /********** MOVEMENT HANDLERS **********/

  float distance(Point3D a, Point3D b){
    return sqrt((a.x - b.x)*(a.x - b.x) +
		(a.y - b.y)*(a.y - b.y) + 
		(a.z - b.z)*(a.z - b.z));
  }
};

#endif
