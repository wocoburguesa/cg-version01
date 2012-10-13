#include <math.h>
#include <unistd.h>
#include <iostream>

#define PI 3.141592654
#define TH 30             //angle formed between the diagonal and mov_vector
#define POINT pair<float, float>

using namespace std;

class PlayerHandler{
 private:
  float x, y;
  float acceleration;
  float speed;
  float max_speed;
  float friction_constant;
  float current_friction;
  float angle;
  float car_size;
  float radius;
  POINT mov_vector;
  vector< POINT > corners;
  vector< POINT > equations;
  bool *pushed;

 public:
  PlayerHandler(float max,
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
    radius = size;

    //initial mov_vector
    angle = ang;
    mov_vector = POINT(cos(angle*PI/180),
		       sin(angle*PI/180));

    //button press flags
    pushed = new bool[4];
    pushed[0] = pushed[1] = pushed[2] = pushed[3] = 0;

    corners.push_back(POINT(0.0f, 0.0f));
    corners.push_back(POINT(0.0f, 0.0f));
    corners.push_back(POINT(0.0f, 0.0f));
    corners.push_back(POINT(0.0f, 0.0f));
    set_top_left();
    set_bottom_left();
    set_bottom_right();
    set_top_right();
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


  /*** BUTTON PRESS CONTROLLERS ***/
  void push_forward(){ pushed[0] = 1; };
  void push_back(){ pushed[2] = 1; };
  void push_left(){ pushed[1] = 1; };
  void push_right(){ pushed[3] = 1; };
  void push_space(){ shoot(); };

  void release_forward(){ pushed[0] = 0; };
  void release_back(){ pushed[2] = 0; };
  void release_left(){ pushed[1] = 0; };
  void release_right(){ pushed[3] = 0; };
  /*** END BUTTON PRESS CONTROLLERS ***/

  /*** GETTERS FOR DRAWING THE CAR ***/
  POINT get_x_y(){
    return POINT(x, y);
  }

  POINT get_top_left(){ return corners[0]; };
  POINT get_bottom_left(){ return corners[1]; };
  POINT get_bottom_right(){ return corners[2]; };
  POINT get_top_right(){ return corners[3]; };

  /*** END GETTERS FOR CAR ***/


  void set_top_left(){
    float ang = (angle + TH);
    POINT direction(cos(ang*PI/180.0f), sin(ang*PI/180.0f));

    float denom = sqrt(direction.first*direction.first +
		       direction.second*direction.second);
    float factor = car_size / denom;
    corners[0].first = x + (direction.first * factor);
    corners[0].second = y + (direction.second * factor);
  }

  void set_top_right(){
    float ang = (angle + TH + 300);
    POINT direction(cos(ang*PI/180), sin(ang*PI/180));
    
    float denom = sqrt(direction.first*direction.first +
		       direction.second*direction.second);
    float factor = car_size / denom;
    corners[3].first = x + (direction.first * factor);
    corners[3].second = y + (direction.second * factor);
  }

  void set_bottom_left(){
    float ang = (angle + 120 + TH);
    POINT direction(cos(ang*PI/180), sin(ang*PI/180));
    
    float denom = sqrt(direction.first*direction.first +
		       direction.second*direction.second);
    float factor = car_size / denom;
    corners[1].first = x + (direction.first * factor);
    corners[1].second = y + (direction.second * factor);
  }

  void set_bottom_right(){
    float ang = (angle + TH + 180);
    POINT direction(cos(ang*PI/180), sin(ang*PI/180));
    
    float denom = sqrt(direction.first*direction.first +
		       direction.second*direction.second);
    float factor = car_size / denom;
    corners[2].first = x + (direction.first * factor);
    corners[2].second = y + (direction.second * factor);
  }

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
    set_top_left();
    set_bottom_left();
    set_bottom_right();
    set_top_right();

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

  float get_radius(){ return radius; };

  float get_max_speed(){ return max_speed; };

  float get_size(){ return car_size; };

  float get_acceleration(){ return acceleration; };

  float get_friction(){ return friction_constant; };

  vector<POINT> get_equations(){ return equations; };

  void set_speed(float new_speed){
    speed = new_speed;
  }

  void bump(){
    //    speed = (speed > 0) ? -0.0005 : 0.0005;
    speed = -(speed * 0.8);
    update_position();
  }

  vector< POINT > get_corners(){ return corners; };
};
