#include <math.h>
#include <unistd.h>
#include <iostream>

#define PI 3.141592654
#define TH 30             //angle formed between the diagonal and mov_vector
#define POINT pair<float, float>

using namespace std;

class MovingObject{
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
  POINT destination;
  bool in_transit;
  POINT target_vector;
  POINT mov_vector;
  vector< POINT > corners;
  vector< POINT > equations;

 public:
  MovingObject(float x_init,
	       float y_init,
	       float max,
	       float accel,
	       float friction,
	       float size,
	       float ang=90.0f){
    x = x_init;
    y = y_init;
    speed = 0.0f;   //initial pos and speed
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

  float distance(pair<float, float> a, pair<float, float> b){
    return sqrt((a.first - b.first)*(a.first - b.first) +
		(a.second - b.second)*(a.second - b.second));
  }

  void update(){
    process_friction();
    update_position();
    if(in_transit){
      if(distance(POINT(x, y), destination) > 3.0f){
	move_forward();
	if(distance(mov_vector, target_vector) > 0.0001f){
	  float current_slope = (destination.second - y)/(destination.first - x);
	  if(abs(current_slope - (target_vector.second/target_vector.first)) >
	     0.1f){
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

  float get_radius(){
    return radius;
  }

  void set_speed(float new_speed){
    speed = new_speed;
  }

  float get_speed(){ return speed; };

  void bump(){
    //    speed = (speed > 0) ? -0.0005 : 0.0005;
    speed = -(speed * 0.1f);
    turn_left();
    update_position();
  }

  void go_to(POINT target){
    destination = target;
    in_transit = 1;
    float norm = sqrt((target.first * target.first) +
		      (target.second * target.second));
    target_vector = POINT(target.first/norm, target.second/norm);
  }

  vector<POINT> get_corners(){ return corners; };

  vector<POINT> get_equations(){ return equations; };
};
