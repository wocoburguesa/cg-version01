#include <math.h>
#include <unistd.h>
#include <iostream>
#include "p_static_object.h"
#include "p_enemy.h"
#include "p_player.h"
#include "p_projectile.h"

#ifndef MAP_HANDLER_H
#define MAP_HANDLER_H

#define POINT pair<float, float>
#define INF 0xffffffff

using namespace std;

class MapHandler{
 private:
  vector<StaticObject*> buildings;
  vector<StaticObject*> bounds;
  vector<StaticObject*> pickups;
  vector<Enemy*> enemies;
  vector<Projectile*> projectiles;
  PlayerHandler * player;

  bool register_pickup;

 public:
  MapHandler(){
    register_pickup = 0;
  };

  /********** GETTERS **********/
  int get_buildings_count(){ return buildings.size(); }
  int get_bounds_count(){ return bounds.size(); }
  int get_pickups_count(){ return pickups.size(); }
  int get_enemies_count(){ return enemies.size(); }
  int get_projectiles_count(){ return projectiles.size(); }

  bool check_for_pickup(){ return register_pickup; }

  StaticObject* get_building_by_idx(int idx){ return buildings[idx]; }
  StaticObject* get_bound_by_idx(int idx){ return bounds[idx]; }
  StaticObject* get_pickup_by_idx(int idx){ return pickups[idx]; }
  Enemy* get_enemy_by_idx(int idx){ return enemies[idx]; }
  Projectile* get_projectile_by_idx(int idx){ return projectiles[idx]; }

  vector<StaticObject*> get_buildings(){ return buildings; }
  vector<Enemy*> get_enemies(){ return enemies; }
  vector<Projectile*> get_projectiles(){ return projectiles; }
  /********** GETTERS **********/

  /********** SETTERS **********/
  void reset_pickup_status(){ register_pickup = 0; }
  void reset_buildings(){ buildings.clear(); }
  void reset_bounds(){ bounds.clear(); }
  void reset_enemies(){ enemies.clear(); }
  void reset_pickups(){ pickups.clear(); }
  void reset_projectiles(){ projectiles.clear(); }
  /********** SETTERS **********/

  /********** OBJECT CREATORS **********/
  void add_bound(vector<Point3D> &c,
		 float height,
		 Point3D color){
    StaticObject * new_object = new StaticObject(c, height, 0);
    new_object->set_color(color);
    bounds.push_back(new_object);
  }

  void add_static_object(vector<Point3D> &c,
			 float height,
			 bool is_pickup,
			 Point3D color){
    StaticObject * new_object = new StaticObject(c, height, is_pickup);
    new_object->set_color(color);
    if(is_pickup)
      pickups.push_back(new_object);
    else
      buildings.push_back(new_object);
  }

  void add_pickup(MovingObject * dropper){
    Point3D center = dropper->get_center();
    vector<Point3D> corners;
    corners.push_back(Point3D(center.x-PICKUP_SIZE,
			      center.y+PICKUP_SIZE,
			      GROUND_LEVEL));
    corners.push_back(Point3D(center.x-PICKUP_SIZE,
			      center.y-PICKUP_SIZE,
			      GROUND_LEVEL));
    corners.push_back(Point3D(center.x+PICKUP_SIZE,
			      center.y-PICKUP_SIZE,
			      GROUND_LEVEL));
    corners.push_back(Point3D(center.x+PICKUP_SIZE,
			      center.y+PICKUP_SIZE,
			      GROUND_LEVEL));

    add_static_object(corners, PICKUP_HEIGHT, 1, dropper->get_color());
  }


  void add_enemy_object(float x, float y, float height, float max_speed,
			float acceleration, float friction,
			float size, float angle, float health,
			Point3D color){
    Enemy * new_enemy = new Enemy(x, y, height,
				  max_speed, acceleration, friction,
				  size, angle, health);
    new_enemy->set_color(color);
    enemies.push_back(new_enemy);
  }

  void add_projectile(float x, float y, float height, float max_speed,
		      float acceleration, float friction,
		      float size, float angle){
    Projectile * new_projectile = new Projectile(x, y, height, max_speed,
						 acceleration, friction,
						 size, angle);
    projectiles.push_back(new_projectile);
  }

  void add_player(PlayerHandler * p){
    player = p;
  }
  /********** OBJECT CREATORS **********/

  /********** UTLILITY FUNCTIONS **********/
  float distance(Point2D a, Point2D b){
    return sqrt((a.x - b.x)*(a.x - b.x) +
		(a.y - b.y)*(a.y - b.y));
  }

  float distance(Point3D a, Point3D b){
    return sqrt((a.x - b.x)*(a.x - b.x) +
		(a.y - b.y)*(a.y - b.y) +
		(a.z - b.z)*(a.z - b.z));
  }

  float two_d_distance(Point3D a, Point3D b){
    return sqrt((a.x - b.x)*(a.x - b.x) +
		(a.y - b.y)*(a.y - b.y));
  }

  Equation get_line_equation(Point3D a, Point3D b){
    float slope, intercept;
    slope = (a.y - b.y)/(a.x - b.x);
    intercept = a.y - slope * a.x;
    return Equation(slope, intercept);
  }

  bool point_in_line(Point3D p, Equation l1, Equation l2){

  }
  /********** UTLILITY FUNCTIONS **********/

  /********** COLLISION DETECTORS **********/
  bool check_for_collision(vector<Point3D> active,
			   vector<Point3D> passive,
			   vector<Equation> equations){
    float slope, intercept, xi, yi;
    Equation equation;
    /*
      slope = slope for equation of a side of the moving object
      intercept = intercept for equation of a side of the moving object
      xi, yi = intersection of a side of the moving with a side of the static object
    */

    //front
    equation = get_line_equation(active[0], active[3]);
    slope = equation.slope;
    intercept = equation.intercept;

    for(int i = 0; i < equations.size(); ++i){
      if(equations[i].intercept == INF)
	xi = equations[i].slope;
      else
	xi = (equations[i].intercept - intercept)/(slope - equations[i].slope);
      yi = slope * xi + intercept;
      if((xi - passive[i].x)*
	 (xi - passive[(i+1)%passive.size()].x) <= 0 &&
	 (yi - passive[i].y)*
	 (yi - passive[(i+1)%passive.size()].y) <= 0 &&
	 (xi - active[0].x)*(xi - active[3].x) <= 0 &&
	 (yi - active[0].y)*(yi - active[3].y) <= 0)
	return 1;
    }

    //right
    equation = get_line_equation(active[2], active[3]);
    slope = equation.slope;
    intercept = equation.intercept;

    for(int i = 0; i < equations.size(); ++i){
      if(equations[i].intercept == INF)
	xi = equations[i].slope;
      else
	xi = (equations[i].intercept - intercept)/(slope - equations[i].slope);
      yi = slope * xi + intercept;
      if((xi - passive[i].x)*
	 (xi - passive[(i+1)%passive.size()].x) <= 0 &&
	 (yi - passive[i].y)*
	 (yi - passive[(i+1)%passive.size()].y) <= 0 &&
	 (xi - active[2].x)*(xi - active[3].x) <= 0 &&
	 (yi - active[2].y)*(yi - active[3].y) <= 0)
	return 1;
    }

    //left
    equation = get_line_equation(active[0], active[1]);
    slope = equation.slope;
    intercept = equation.intercept;

    for(int i = 0; i < equations.size(); ++i){
      if(equations[i].intercept == INF)
	xi = equations[i].slope;
      else
	xi = (equations[i].intercept - intercept)/(slope - equations[i].slope);
      yi = slope * xi + intercept;
      if((xi - passive[i].x)*
	 (xi - passive[(i+1)%passive.size()].x) <= 0 &&
	 (yi - passive[i].y)*
	 (yi - passive[(i+1)%passive.size()].y) <= 0 &&
	 (xi - active[0].x)*(xi - active[1].x) <= 0 &&
	 (yi - active[0].y)*(yi - active[1].y) <= 0)
	return 1;
    }

    //back
    equation = get_line_equation(active[1], active[2]);
    slope = equation.slope;
    intercept = equation.intercept;

    for(int i = 0; i < equations.size(); ++i){
      if(equations[i].intercept == INF)
	xi = equations[i].slope;
      else
	xi = (equations[i].intercept - intercept)/(slope - equations[i].slope);
      yi = slope * xi + intercept;
      if((xi - passive[i].x)*
	 (xi - passive[(i+1)%passive.size()].x) <= 0 &&
	 (yi - passive[i].y)*
	 (yi - passive[(i+1)%passive.size()].y) <= 0 &&
	 (xi - active[1].x)*(xi - active[2].x) <= 0 &&
	 (yi - active[1].y)*(yi - active[2].y) <= 0)
	return 1;
    }
    return 0;
  }

  void check_moving_vs_static_collisions(){
    for(int i = 0; i < buildings.size(); ++i){
      if(two_d_distance(player->get_center(), buildings[i]->get_center()) <=
	 (player->get_radius() + buildings[i]->get_radius())){
	if(check_for_collision(player->get_corners(),
			       buildings[i]->get_corners(),
			       buildings[i]->get_equations())){
	  player->bump();
	  player->register_crash();
	}
      }
    }

    for(int i = 0; i < bounds.size(); ++i){
      if(two_d_distance(player->get_center(), bounds[i]->get_center()) <=
	 (player->get_radius() + bounds[i]->get_radius())){
	if(check_for_collision(player->get_corners(),
			       bounds[i]->get_corners(),
			       bounds[i]->get_equations())){
	  player->bump();
	  player->register_crash();
	}
      }
    }

    for(int i = 0; i < enemies.size(); ++i)
      for(int j = 0; j < enemies.size(); ++j){
	if(two_d_distance(enemies[i]->get_center(),
			  buildings[j]->get_center()) <=
	   (enemies[i]->get_radius() + buildings[j]->get_radius())){
	  if(check_for_collision(enemies[i]->get_corners(),
				 buildings[j]->get_corners(),
				 buildings[j]->get_equations()))
	    enemies[i]->bump();
	}
      }

    for(int i = 0; i < enemies.size(); ++i)
      for(int j = 0; j < buildings.size(); ++j){
	if(two_d_distance(enemies[i]->get_center(),
			  buildings[j]->get_center()) <=
	   (enemies[i]->get_radius() + buildings[j]->get_radius())){
	  if(check_for_collision(enemies[i]->get_corners(),
				 buildings[j]->get_corners(),
				 buildings[j]->get_equations()))
	    enemies[i]->bump();
	}
      }
  }  
  
  void check_moving_vs_moving_collisions(){
    for(int i = 0; i < enemies.size(); ++i){
      if(two_d_distance(player->get_center(), enemies[i]->get_center()) <=
	 (player->get_radius() + enemies[i]->get_radius())){
	if(check_for_collision(player->get_corners(),
			       enemies[i]->get_corners(),
			       enemies[i]->get_equations())){
	  player->bump();
	  player->register_crash();
	}
      }
    }

    for(int i = 0; i < enemies.size(); ++i){
      if(two_d_distance(player->get_center(), enemies[i]->get_center()) <=
	 (player->get_radius() + enemies[i]->get_radius())){
	if(check_for_collision(enemies[i]->get_corners(),
			       player->get_corners(),
			       player->get_equations()))
	  enemies[i]->bump();
      }
    }

    for(int i = 0; i < enemies.size(); ++i)
      for(int j = 0; j < enemies.size(); ++j){
	if(i != j)
	  if(two_d_distance(enemies[i]->get_center(),
			    enemies[j]->get_center()) <=
	     (enemies[i]->get_radius() + enemies[j]->get_radius())){
	    if(check_for_collision(enemies[i]->get_corners(),
				   enemies[j]->get_corners(),
				   enemies[j]->get_equations()))
	      enemies[i]->bump();
	}
      }
  }

  void check_projectile_collisions(){
    for(int i = 0; i < projectiles.size();){
      Projectile * current = projectiles[i];
      for(int j = 0; j < enemies.size(); ++j){
	if(two_d_distance(projectiles[i]->get_center(),
			  enemies[j]->get_center()) <=
	   (projectiles[i]->get_radius() + enemies[j]->get_radius())){
	  if(check_for_collision(projectiles[i]->get_corners(),
				 enemies[j]->get_corners(),
				 enemies[j]->get_equations())){
	    enemies[j]->register_hit();
	    projectiles.erase(projectiles.begin()+i);
	    delete current;
	    break;
	  }
	}
      }
      if(current)
	++i;
    }

    for(int i = 0; i < projectiles.size();){
      Projectile * current = projectiles[i];
      for(int j = 0; j < buildings.size(); ++j){
	if(two_d_distance(projectiles[i]->get_center(),
		       buildings[j]->get_center()) <=
	   (projectiles[i]->get_radius() + buildings[j]->get_radius())){
	  if(check_for_collision(projectiles[i]->get_corners(),
				 buildings[j]->get_corners(),
				 buildings[j]->get_equations())){
	  projectiles.erase(projectiles.begin()+i);
	  delete current;
	  break;
	  }
	}
      }
      if(current)
	++i;
    }

    for(int i = 0; i < projectiles.size();){
      Projectile * current = projectiles[i];
      if(two_d_distance(projectiles[i]->get_center(), player->get_center()) <=
	 (projectiles[i]->get_radius() + player->get_radius())){
	if(check_for_collision(projectiles[i]->get_corners(),
			       player->get_corners(),
			       player->get_equations())){
	  player->register_hit();
	  projectiles.erase(projectiles.begin()+i);
	  delete current;
	  break;
	}
      }
      else;
      if(current)
	++i;
    }
  }

  void check_player_pickups(){
    for(int i = 0; i < pickups.size(); ){
      StaticObject * current = pickups[i];
      if(two_d_distance(player->get_center(), pickups[i]->get_center()) <=
	 (player->get_radius() + pickups[i]->get_radius())){
	if(check_for_collision(player->get_corners(),
			       pickups[i]->get_corners(),
			       pickups[i]->get_equations())){
	  pickups.erase(pickups.begin()+i);
	  register_pickup = 1;
	  delete current;
	  break;
	}
      }
      else;
      if(current)
	++i;
    }
  }  
 /********** COLLISION DETECTORS **********/

  /********** DEATH DETECTORS **********/
  void check_for_dead_enemies(){
    for(int i = 0; i < enemies.size();){
      Enemy * current = enemies[i];
      if(!current->vital_signs()){
	add_pickup(current);
	enemies.erase(enemies.begin()+i);
	delete current;
      }
      else
	++i;
    }
  }

 
  void update(){
    check_moving_vs_static_collisions();
    check_moving_vs_moving_collisions();
    check_projectile_collisions();
    check_for_dead_enemies();
    check_player_pickups();
    player->update();
    for(int i = 0; i < enemies.size(); ++i){
      enemies[i]->update();
      Point3D p_center = player->get_center();
      enemies[i]->go_to(Point2D(p_center.x, p_center.y));
    }
    for(int i = 0; i < projectiles.size(); ++i)
      projectiles[i]->update();
  }
};

#endif
