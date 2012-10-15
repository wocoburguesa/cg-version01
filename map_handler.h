#include <math.h>
#include <unistd.h>
#include <iostream>
#include "p_static_object.h"
#include "p_enemy.h"
#include "p_player.h"
#include "p_projectile.h"

#define POINT pair<float, float>
#define INF 0xffffffff

using namespace std;

class MapHandler{
 private:
  vector<StaticObject*> buildings;
  vector<Enemy*> enemies;
  vector<Projectile*> projectiles;
  PlayerHandler * player;

 public:
  MapHandler(){};

  /********** GETTERS **********/
  int get_buildings_count(){ return buildings.size(); }
  int get_enemies_count(){ return enemies.size(); }
  int get_projectiles_count(){ return projectiles.size(); }

  StaticObject* get_building_by_idx(int idx){ return buildings[idx]; }
  Enemy* get_enemy_by_idx(int idx){ return enemies[idx]; }
  Projectile* get_projectile_by_idx(int idx){ return projectiles[idx]; }

  vector<StaticObject*> get_buildings(){ return buildings; }
  vector<Enemy*> get_enemies(){ return enemies; }
  vector<Projectile*> get_projectiles(){ return projectiles; }
  /********** GETTERS **********/

  /********** OBJECT CREATORS **********/
  void add_static_object(vector< pair<float, float> > &c){
    StaticObject * new_building = new StaticObject(c);
    buildings.push_back(new_building);
  }

  void add_enemy_object(float x, float y, float max_speed,
			float acceleration, float friction,
			float size, float angle, float health){
    Enemy * new_enemy = new Enemy(x, y, max_speed, acceleration, friction,
				  size, angle, health);
    enemies.push_back(new_enemy);
  }

  void add_projectile(float x, float y, float max_speed,
		      float acceleration, float friction,
		      float size, float angle){
    Projectile * new_projectile = new Projectile(x, y, max_speed,
						 acceleration, friction,
						 size, angle);
    projectiles.push_back(new_projectile);
  }

  void add_player(PlayerHandler * p){
    player = p;
  }
  /********** OBJECT CREATORS **********/

  /********** UTLILITY FUNCTIONS **********/
  float distance(POINT a, POINT b){
    return sqrt((a.first - b.first)*(a.first - b.first) +
		(a.second - b.second)*(a.second - b.second));
  }

  POINT get_line_equation(POINT a, POINT b){
    float slope, intercept;
    slope = (a.second - b.second)/(a.first - b.first);
    intercept = a.second - slope * a.first;
    return POINT(slope, intercept);
  }

  bool point_in_line(POINT p, POINT l1, POINT l2){

  }
  /********** UTLILITY FUNCTIONS **********/

  /********** COLLISION DETECTORS **********/
  bool check_for_collision(vector<POINT> active,
			   vector<POINT> passive,
			   vector<POINT> equations){
    float slope, intercept, xi, yi;
    POINT equation;
    /*
      slope = slope for equation of a side of the moving object
      intercept = intercept for equation of a side of the moving object
      xi, yi = intersection of a side of the moving with a side of the static object
    */

    //front
    equation = get_line_equation(active[0], active[3]);
    slope = equation.first;
    intercept = equation.second;

    for(int i = 0; i < equations.size(); ++i){
      if(equations[i].second == INF)
	xi = equations[i].first;
      else
	xi = (equations[i].second - intercept)/(slope - equations[i].first);
      yi = slope * xi + intercept;
      if((xi - passive[i].first)*
	 (xi - passive[(i+1)%passive.size()].first) <= 0 &&
	 (yi - passive[i].second)*
	 (yi - passive[(i+1)%passive.size()].second) <= 0 &&
	 (xi - active[0].first)*(xi - active[3].first) <= 0 &&
	 (yi - active[0].second)*(yi - active[3].second) <= 0)
	return 1;
    }

    //right
    equation = get_line_equation(active[2], active[3]);
    slope = equation.first;
    intercept = equation.second;

    for(int i = 0; i < equations.size(); ++i){
      if(equations[i].second == INF)
	xi = equations[i].first;
      else
	xi = (equations[i].second - intercept)/(slope - equations[i].first);
      yi = slope * xi + intercept;
      if((xi - passive[i].first)*
	 (xi - passive[(i+1)%passive.size()].first) <= 0 &&
	 (yi - passive[i].second)*
	 (yi - passive[(i+1)%passive.size()].second) <= 0 &&
	 (xi - active[2].first)*(xi - active[3].first) <= 0 &&
	 (yi - active[2].second)*(yi - active[3].second) <= 0)
	return 1;
    }

    //left
    equation = get_line_equation(active[0], active[1]);
    slope = equation.first;
    intercept = equation.second;

    for(int i = 0; i < equations.size(); ++i){
      if(equations[i].second == INF)
	xi = equations[i].first;
      else
	xi = (equations[i].second - intercept)/(slope - equations[i].first);
      yi = slope * xi + intercept;
      if((xi - passive[i].first)*
	 (xi - passive[(i+1)%passive.size()].first) <= 0 &&
	 (yi - passive[i].second)*
	 (yi - passive[(i+1)%passive.size()].second) <= 0 &&
	 (xi - active[0].first)*(xi - active[1].first) <= 0 &&
	 (yi - active[0].second)*(yi - active[1].second) <= 0)
	return 1;
    }

    //back
    equation = get_line_equation(active[1], active[2]);
    slope = equation.first;
    intercept = equation.second;

    for(int i = 0; i < equations.size(); ++i){
      if(equations[i].second == INF)
	xi = equations[i].first;
      else
	xi = (equations[i].second - intercept)/(slope - equations[i].first);
      yi = slope * xi + intercept;
      if((xi - passive[i].first)*
	 (xi - passive[(i+1)%passive.size()].first) <= 0 &&
	 (yi - passive[i].second)*
	 (yi - passive[(i+1)%passive.size()].second) <= 0 &&
	 (xi - active[1].first)*(xi - active[2].first) <= 0 &&
	 (yi - active[1].second)*(yi - active[2].second) <= 0)
	return 1;
    }
    return 0;
  }

  void check_moving_vs_static_collisions(){
    for(int i = 0; i < buildings.size(); ++i){
      if(distance(player->get_x_y(), buildings[i]->get_x_y()) <=
	 (player->get_radius() + buildings[i]->get_radius())){
	if(check_for_collision(player->get_corners(),
			       buildings[i]->get_corners(),
			       buildings[i]->get_equations())){
	  player->bump();
	  player->register_crash();
	}
      }
    }

    for(int i = 0; i < enemies.size(); ++i)
      for(int j = 0; j < enemies.size(); ++j){
	if(distance(enemies[i]->get_x_y(), buildings[j]->get_x_y()) <=
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
      if(distance(player->get_x_y(), enemies[i]->get_x_y()) <=
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
      if(distance(player->get_x_y(), enemies[i]->get_x_y()) <=
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
	  if(distance(enemies[i]->get_x_y(), enemies[j]->get_x_y()) <=
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
	if(distance(projectiles[i]->get_x_y(), enemies[j]->get_x_y()) <=
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
	if(distance(projectiles[i]->get_x_y(), buildings[j]->get_x_y()) <=
	   (projectiles[i]->get_radius() + buildings[j]->get_radius())){
	  if(check_for_collision(projectiles[i]->get_corners(),
				 buildings[j]->get_corners(),
				 buildings[j]->get_equations())){
	  cout << "wtf" << endl;
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
      if(distance(projectiles[i]->get_x_y(), player->get_x_y()) <=
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
  /********** COLLISION DETECTORS **********/

  /********** DEATH DETECTORS **********/
  void check_for_dead_enemies(){
    for(int i = 0; i < enemies.size();){
      Enemy * current = enemies[i];
      if(!current->vital_signs()){
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
    player->update();
    for(int i = 0; i < enemies.size(); ++i){
      enemies[i]->go_to(POINT(8,0));
      enemies[i]->update();
    }
    for(int i = 0; i < projectiles.size(); ++i)
      projectiles[i]->update();
  }
};
