#include <math.h>
#include <unistd.h>
#include <iostream>

#define PI 3.141592654
#define TH 30
#define HIPO 1.154700538

using namespace std;

class MovementHandler{
 private:
  float x, y;
  float acceleration;
  float speed;
  float max_speed;
  float angle;
  pair<float, float> mov_vector;
  bool *pushed;

 public:
  MovementHandler(float max, float accel, float ang=90.0f){
    x = y = speed = 0.0f;
    acceleration = accel;
    angle = ang;
    mov_vector = pair<float, float>(0.0f, 1.0f);
    pushed = new bool[4];
    pushed[0] = pushed[1] = pushed[2] = pushed[3] = 0;
    max_speed = max;
  }

  void push_forward(){ pushed[0] = 1; };
  void push_back(){ pushed[2] = 1; };
  void push_left(){ pushed[1] = 1; };
  void push_right(){ pushed[3] = 1; };

  void release_forward(){
    pushed[0] = 0;
    
  }
  void release_back(){ pushed[2] = 0; };
  void release_left(){ pushed[1] = 0; };
  void release_right(){ pushed[3] = 0; };

  pair<float, float> get_top_left(){
    float ang = (angle + TH);// % 360;
    pair<float, float> direction(cos(ang*PI/180.0f), sin(ang*PI/180.0f));

    float denom = sqrt(direction.first*direction.first +
		       direction.second*direction.second);
    float factor = HIPO / denom;
    return pair<float, float>(x+(direction.first*factor),
			      y+(direction.second*factor));
  }

  pair<float, float> get_top_right(){
    float ang = (angle + TH + 300);// % 360;
    pair<float, float> direction(cos(ang*PI/180), sin(ang*PI/180));
    
    float denom = sqrt(direction.first*direction.first +
		       direction.second*direction.second);
    float factor = HIPO / denom;
    return pair<float, float>(x+(direction.first*factor),
			      y+(direction.second*factor));
  }

  pair<float, float> get_bottom_left(){
    float ang = (angle + 120 + TH);// % 360;
    pair<float, float> direction(cos(ang*PI/180), sin(ang*PI/180));
    
    float denom = sqrt(direction.first*direction.first +
		       direction.second*direction.second);
    float factor = HIPO / denom;
    return pair<float, float>(x+(direction.first*factor),
			      y+(direction.second*factor));
  }

  pair<float, float> get_bottom_right(){
    float ang = (angle + TH + 180);// % 360;
    pair<float, float> direction(cos(ang*PI/180), sin(ang*PI/180));
    
    float denom = sqrt(direction.first*direction.first +
		       direction.second*direction.second);
    float factor = HIPO / denom;
    return pair<float, float>(x+(direction.first*factor),
			      y+(direction.second*factor));
  }

  void update(){
    if(pushed[0])
      move_forward();
    else{
      if(speed > 0 && !pushed[2]){
	move_back();
      }
    }
    if(pushed[2])
      move_back();
    else{
      if(speed < 0 && !pushed[0]){
	move_forward();
      }
    }
    if(pushed[1])
      turn_left();
    else;
    if(pushed[3])
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
    angle = (angle + 0.05);// % 360;
    mov_vector.first = cos(angle*PI/180);
    mov_vector.second = sin(angle*PI/180);
  }

  void turn_right(){
    angle = (angle - 0.05);// % 360;
    mov_vector.first = cos(angle*PI/180);
    mov_vector.second = sin(angle*PI/180);
  }
};
