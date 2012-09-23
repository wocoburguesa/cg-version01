#include <math.h>
#include <unistd.h>
#include <iostream>

#define PI 3.141592654
#define TH 30             //angle formed between the diagonal and mov_vector
#define HIPO 1.154700538  //half the diagonal of the "car"

using namespace std;

class MovementHandler{
 private:
  float x, y;
  float acceleration;
  float speed;
  float max_speed;
  float friction_constant;
  float current_friction;
  float angle;
  float car_size;
  pair<float, float> mov_vector;
  bool *pushed;

 public:
  MovementHandler(float max,
		  float accel,
		  float friction,
		  float size,
		  float ang=90.0f){
    x = y = speed = 0.0f;   //initial pos and speed
    acceleration = accel;   //rate of speed increase
    max_speed = max;        //maximum posible speed
    friction_constant = friction;
    current_friction = friction;

    car_size = size;

    //initial mov_vector
    angle = ang;
    mov_vector = pair<float, float>(cos(angle*PI/180),
				    sin(angle*PI/180));

    //button press flags
    pushed = new bool[4];
    pushed[0] = pushed[1] = pushed[2] = pushed[3] = 0;
  }


  /*** BUTTON PRESS CONTROLLERS ***/
  void push_forward(){ pushed[0] = 1; };
  void push_back(){ pushed[2] = 1; };
  void push_left(){ pushed[1] = 1; };
  void push_right(){ pushed[3] = 1; };

  void release_forward(){ pushed[0] = 0; };
  void release_back(){ pushed[2] = 0; };
  void release_left(){ pushed[1] = 0; };
  void release_right(){ pushed[3] = 0; };
  /*** END BUTTON PRESS CONTROLLERS ***/

  /*** GETTERS FOR DRAWING THE CAR ***/
  pair<float, float> get_top_left(){
    float ang = (angle + TH);
    pair<float, float> direction(cos(ang*PI/180.0f), sin(ang*PI/180.0f));

    float denom = sqrt(direction.first*direction.first +
		       direction.second*direction.second);
    float factor = car_size / denom;
    return pair<float, float>(x+(direction.first*factor),
			      y+(direction.second*factor));
  }

  pair<float, float> get_top_right(){
    float ang = (angle + TH + 300);
    pair<float, float> direction(cos(ang*PI/180), sin(ang*PI/180));
    
    float denom = sqrt(direction.first*direction.first +
		       direction.second*direction.second);
    float factor = car_size / denom;
    return pair<float, float>(x+(direction.first*factor),
			      y+(direction.second*factor));
  }

  pair<float, float> get_bottom_left(){
    float ang = (angle + 120 + TH);
    pair<float, float> direction(cos(ang*PI/180), sin(ang*PI/180));
    
    float denom = sqrt(direction.first*direction.first +
		       direction.second*direction.second);
    float factor = car_size / denom;
    return pair<float, float>(x+(direction.first*factor),
			      y+(direction.second*factor));
  }

  pair<float, float> get_bottom_right(){
    float ang = (angle + TH + 180);
    pair<float, float> direction(cos(ang*PI/180), sin(ang*PI/180));
    
    float denom = sqrt(direction.first*direction.first +
		       direction.second*direction.second);
    float factor = car_size / denom;
    return pair<float, float>(x+(direction.first*factor),
			      y+(direction.second*factor));
  }
  /*** END GETTERS FOR CAR ***/

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

  void update_position(){
    float denom = sqrt(mov_vector.first*mov_vector.first +
		       mov_vector.second*mov_vector.second);
    float factor = speed / denom;
    x += mov_vector.first * factor;
    y += mov_vector.second * factor;
  }

  //  void friction

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
    angle = (angle + 0.05);
    mov_vector.first = cos(angle*PI/180);
    mov_vector.second = sin(angle*PI/180);
  }

  void turn_right(){
    angle = (angle - 0.05);
    mov_vector.first = cos(angle*PI/180);
    mov_vector.second = sin(angle*PI/180);
  }
};
