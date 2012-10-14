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
  bool in_transit;
  POINT target_vector;

 public:
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

  void go_to(POINT target){
    destination = target;
    in_transit = 1;
    float norm = sqrt((target.first * target.first) +
		      (target.second * target.second));
    target_vector = POINT(target.first/norm, target.second/norm);
  }

};

class C : public A{
 public:
  C(){ cout <<  "construyendo C"  << endl; }
  void print2(){ cout << "print2" << endl ;}
};

#endif
