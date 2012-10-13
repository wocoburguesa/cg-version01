#include <math.h>
#include <unistd.h>
#include <iostream>
#include "static_object.h"
#include "moving_object.h"
#include "movement.h"

#define POINT pair<float, float>
#define INF 0xffffffff

using namespace std;

class MapHandler{
 private:
  vector<StaticObject> sobjects;
  vector<MovingObject> mobjects;
  PlayerHandler * player;

 public:
  MapHandler(){};

  void add_static_object(vector< pair<float, float> > &c){
    StaticObject new_sobject(c);
    sobjects.push_back(new_sobject);
  }

  void add_moving_object(float x, float y, float max_speed,
			 float acceleration, float friction,
			 float size, float angle){
    MovingObject new_mobject(x, y, max_speed, acceleration, friction,
			     size, angle);
    mobjects.push_back(new_mobject);
  }

  void add_player(PlayerHandler * p){
    player = p;
  }

  int get_sobjects_count(){
    return sobjects.size();
  }

  StaticObject get_sobject_by_idx(int idx){
    return sobjects[idx];
  }

  int get_mobjects_count(){
    return mobjects.size();
  }

  MovingObject get_mobject_by_idx(int idx){
    return mobjects[idx];
  }

  float distance(pair<float, float> a, pair<float, float> b){
    return sqrt((a.first - b.first)*(a.first - b.first) +
		(a.second - b.second)*(a.second - b.second));
  }

  POINT get_line_equation(POINT a, POINT b){
    float slope, intercept;
    slope = (a.second - b.second)/(a.first - b.first);
    intercept = a.second - slope * a.first;
    return POINT(slope, intercept);
  }

  bool check_collision_fixed(vector<POINT> moving,
			     vector<POINT> fixed,
			     vector<POINT> equations){
    float slope, intercept, xi, yi;
    POINT equation;
    /*
      slope = slope for equation of a side of the moving object
      intercept = intercept for equation of a side of the moving object
      xi, yi = intersection of a side of the moving with a side of the static object
    */

    //front
    equation = get_line_equation(moving[0], moving[3]);
    slope = equation.first;
    intercept = equation.second;

    for(int i = 0; i < equations.size(); ++i){
      if(equations[i].second == INF)
	xi = equations[i].first;
      else
	xi = (equations[i].second - intercept)/(slope - equations[i].first);
      yi = slope * xi + intercept;
      if((xi - fixed[i].first)*(xi - fixed[(i+1)%fixed.size()].first) <= 0 &&
	 (yi - fixed[i].second)*(yi - fixed[(i+1)%fixed.size()].second) <= 0 &&
	 (xi - moving[0].first)*(xi - moving[3].first) <= 0 &&
	 (yi - moving[0].second)*(yi - moving[3].second) <= 0)
	return 1;
    }

    //right
    equation = get_line_equation(moving[2], moving[3]);
    slope = equation.first;
    intercept = equation.second;

    for(int i = 0; i < equations.size(); ++i){
      if(equations[i].second == INF)
	xi = equations[i].first;
      else
	xi = (equations[i].second - intercept)/(slope - equations[i].first);
      yi = slope * xi + intercept;
      if((xi - fixed[i].first)*(xi - fixed[(i+1)%fixed.size()].first) <= 0 &&
	 (yi - fixed[i].second)*(yi - fixed[(i+1)%fixed.size()].second) <= 0 &&
	 (xi - moving[2].first)*(xi - moving[3].first) <= 0 &&
	 (yi - moving[2].second)*(yi - moving[3].second) <= 0)
	return 1;
    }

    //left
    equation = get_line_equation(moving[0], moving[1]);
    slope = equation.first;
    intercept = equation.second;

    for(int i = 0; i < equations.size(); ++i){
      if(equations[i].second == INF)
	xi = equations[i].first;
      else
	xi = (equations[i].second - intercept)/(slope - equations[i].first);
      yi = slope * xi + intercept;
      if((xi - fixed[i].first)*(xi - fixed[(i+1)%fixed.size()].first) <= 0 &&
	 (yi - fixed[i].second)*(yi - fixed[(i+1)%fixed.size()].second) <= 0 &&
	 (xi - moving[0].first)*(xi - moving[1].first) <= 0 &&
	 (yi - moving[0].second)*(yi - moving[1].second) <= 0)
	return 1;
    }

    //back
    equation = get_line_equation(moving[1], moving[2]);
    slope = equation.first;
    intercept = equation.second;

    for(int i = 0; i < equations.size(); ++i){
      if(equations[i].second == INF)
	xi = equations[i].first;
      else
	xi = (equations[i].second - intercept)/(slope - equations[i].first);
      yi = slope * xi + intercept;
      if((xi - fixed[i].first)*(xi - fixed[(i+1)%fixed.size()].first) <= 0 &&
	 (yi - fixed[i].second)*(yi - fixed[(i+1)%fixed.size()].second) <= 0 &&
	 (xi - moving[1].first)*(xi - moving[2].first) <= 0 &&
	 (yi - moving[1].second)*(yi - moving[2].second) <= 0)
	return 1;
    }
    return 0;
  }

  void check_moving_vs_static_collisions(){
    for(int i = 0; i < sobjects.size(); ++i){
      if(distance(player->get_x_y(), sobjects[i].get_x_y()) <=
	 (player->get_radius() + sobjects[i].get_radius())){
	if(check_collision_fixed(player->get_corners(),
				 sobjects[i].get_corners(),
				 sobjects[i].get_equations()))
	  player->bump();
      }
    }

    for(int i = 0; i < mobjects.size(); ++i)
      for(int j = 0; j < sobjects.size(); ++j){
	if(distance(mobjects[i].get_x_y(), sobjects[j].get_x_y()) <=
	   (mobjects[i].get_radius() + sobjects[j].get_radius())){
	  if(check_collision_fixed(mobjects[i].get_corners(),
				   sobjects[j].get_corners(),
				   sobjects[j].get_equations()))
	    mobjects[i].bump();
	}
      }
  }  
  
  void check_moving_vs_moving_collisions(){
    for(int i = 0; i < mobjects.size(); ++i){
      if(distance(player->get_x_y(), mobjects[i].get_x_y()) <=
	 (player->get_radius() + mobjects[i].get_radius())){
	if(check_collision_fixed(player->get_corners(),
				 mobjects[i].get_corners(),
				 mobjects[i].get_equations()))
	  player->bump();
      }
    }

    for(int i = 0; i < mobjects.size(); ++i){
      if(distance(player->get_x_y(), mobjects[i].get_x_y()) <=
	 (player->get_radius() + mobjects[i].get_radius())){
	if(check_collision_fixed(mobjects[i].get_corners(),
				 player->get_corners(),
				 player->get_equations()))
	  mobjects[i].bump();
      }
    }

    for(int i = 0; i < mobjects.size(); ++i)
      for(int j = 0; j < mobjects.size(); ++j){
	if(i != j)
	  if(distance(mobjects[i].get_x_y(), mobjects[j].get_x_y()) <=
	     (mobjects[i].get_radius() + mobjects[j].get_radius())){
	    if(check_collision_fixed(mobjects[i].get_corners(),
				     mobjects[j].get_corners(),
				     mobjects[j].get_equations()))
	      mobjects[i].bump();
	}
      }
  }  

  void update(){
    check_moving_vs_static_collisions();
    check_moving_vs_moving_collisions();
    player->update();
    for(int i = 0; i < mobjects.size(); ++i){
      mobjects[i].go_to(POINT(8,0));
      mobjects[i].update();
    }
  }
};
