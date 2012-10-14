#include <math.h>
#include <unistd.h>
#include <iostream>
#include "p_moving_object.h"

#ifndef P_PLAYER_H
#define P_PLAYER_H

using namespace std;

class PlayerHandler : public MovingObject{
 private:
  bool *pushed;

 public:
  /********** BUTTON PRESS CONTROLLERS **********/
  void push_forward(){ pushed[0] = 1; };
  void push_back(){ pushed[2] = 1; };
  void push_left(){ pushed[1] = 1; };
  void push_right(){ pushed[3] = 1; };
  //  void push_space(){ shoot(); };

  void release_forward(){ pushed[0] = 0; };
  void release_back(){ pushed[2] = 0; };
  void release_left(){ pushed[1] = 0; };
  void release_right(){ pushed[3] = 0; };
  /********** BUTTON PRESS CONTROLLERS **********/

  void update(){
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
};

#endif
