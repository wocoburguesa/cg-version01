#include <math.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include "map_handler.h"

#ifndef GAME_HANDLER_H
#define GAME_HANDLER_H

#define POINT pair<float, float>

using namespace std;

class GameHandler{
 private:
  MapHandler * maphan;
  PlayerHandler * player;

  int items_collected;
  int game_condition;

 public:
  GameHandler(){
    items_collected = 0;
    game_condition = 0;
    maphan = new MapHandler();
    player = new PlayerHandler(PLAYER_STARTING_X,
			       PLAYER_STARTING_Y,
			       PLAYER_HEIGHT,
			       PLAYER_MAX_SPEED,
			       PLAYER_ACCELERATION,
			       PLAYER_FRICTION,
			       PLAYER_SIZE,
			       PLAYER_STARTING_ANGLE,
			       PLAYER_STARTING_HEALTH);
    maphan->add_player(player);
  }

  PlayerHandler * get_player(){ return player; };

  void set_game(char * level){
    maphan->reset_pickup_status();
    maphan->reset_buildings();
    maphan->reset_enemies();
    maphan->reset_pickups();
    maphan->reset_projectiles();
    items_collected = 0;
    game_condition = 0;
    player->reset_health();
    player->set_center(Point3D(PLAYER_STARTING_X,
			       PLAYER_STARTING_Y,
			       GROUND_LEVEL));
    player->set_angle(PLAYER_STARTING_ANGLE);

    srand(time(0));

    ifstream in(level);
    string kind;
    int quantity;
    float x, y, z;
    while(in >> kind){
      if(kind == "BOUNDS"){
	vector<Point3D> object;
	float color_x = (rand()%100)/100.0f;
	float color_y = (rand()%100)/100.0f;
	float color_z = (rand()%100)/100.0f;
	in >> quantity;
	while(quantity--){
	  for(int i = 0; i < 4; ++i){
	    in >> x >> y >> z;
	    object.push_back(Point3D(x*2.0f, y*2.0f, z*2.0f));
	  }
	  spawn_bound(object, Point3D(color_x, color_y, color_z));
	  object.clear();
	}
      }
      else if(kind == "BUILDINGS"){
	vector<Point3D> object;
	float color_x = (rand()%100)/100.0f;
	float color_y = (rand()%100)/100.0f;
	float color_z = (rand()%100)/100.0f;
	in >> quantity;
	while(quantity--){
	  for(int i = 0; i < 4; ++i){
	    in >> x >> y >> z;
	    object.push_back(Point3D(x*2.0f, y*2.0f, z*2.0f));
	  }
	  spawn_building(object, Point3D(color_x, color_y, color_z));
	  object.clear();
	}
      }
      else if(kind == "ENEMIES"){
	in >> quantity;
	float color_x = (rand()%100)/100.0f;
	float color_y = (rand()%100)/100.0f;
	float color_z = (rand()%100)/100.0f;
	while(quantity--){
	  in >> x >> y >> z;
	  cout << x << " " << y << " " << z << endl;
	  spawn_enemy(x, y, z, Point3D(color_x, color_y, color_z));
	}
      }
    }
    in.close();
  }

  MapHandler* get_maphan(){ return maphan; };
  int check_game_condition(){ return game_condition; };

  int get_items_collected(){ return items_collected; };
  
  float get_rand_in_range(float a, float b){
    if(a > b){
      int aux = b;
      b = a;
      a = aux;
    }
    else;

    float diff = b - a;
    diff = diff * (rand()%100)/100.0f;
    return a + diff;
  }

  void spawn_building(vector<Point3D> &corners, Point3D color){
    float height = get_rand_in_range(MIN_BUILDING_HEIGHT,
				     MAX_BUILDING_HEIGHT);

    maphan->add_static_object(corners, BOUND_HEIGHT, 0, color);
  }

  void spawn_bound(vector<Point3D> &corners, Point3D color){
    maphan->add_bound(corners, BOUND_HEIGHT, color);
  }

  void spawn_enemy(float x, float y, float angle, Point3D color){
    float size = get_rand_in_range(MIN_ENEMY_SIZE, MAX_ENEMY_SIZE);
    float max_speed = get_rand_in_range(MIN_ENEMY_SPEED, MAX_ENEMY_SPEED);
    float height = get_rand_in_range(MIN_ENEMY_HEIGHT, MAX_ENEMY_HEIGHT);

    maphan->add_enemy_object(x, y, height,
			     max_speed, player->get_acceleration(),
			     player->get_friction(), size, angle,
			     ENEMY_STARTING_HEALTH, color);
  }

  void spawn_projectile(MovingObject * shooter, float angle=-1.0f){
    if(angle == -1.0f){
      Point2D shooter_vec = shooter->get_mov_vector();
      float denom =
	sqrt(shooter_vec.x*shooter_vec.x +
	     shooter_vec.y*shooter_vec.y);
      float factor = shooter->get_radius() / denom;
      float x = shooter->get_center().x + shooter_vec.x * factor;
      float y = shooter->get_center().y + shooter_vec.y * factor;
      maphan->add_projectile(x, y,
			     PROJECTILE_HEIGHT,
			     0.0f, //max speed, overridden
			     0.0f, //accel, overridden
			     PROJECTILE_FRICTION, // friction
			     PROJECTILE_SIZE,
			     shooter->get_angle());
    }
    else{
      float exit_ang = shooter->get_angle() + angle;
      Point2D out_vector(cos(exit_ang*PI/180), sin(exit_ang*PI/180));
      float denom =
	sqrt(out_vector.x*out_vector.x +
	     out_vector.y*out_vector.y);
      float factor = shooter->get_radius() / denom;

      float x = shooter->get_center().x + out_vector.x * factor;
      float y = shooter->get_center().y + out_vector.y * factor;
      maphan->add_projectile(x, y,
			     PROJECTILE_HEIGHT,
			     0.0f, //max speed, overridden
			     0.0f, //accel, overridden
			     PROJECTILE_FRICTION, // friction
			     PROJECTILE_SIZE,
			     shooter->get_angle()+angle);
    }
  }

  void update(){
    if(player->get_remaining_life_pct() <= 0.0f)
      game_condition = -1;
    else;

    if(items_collected == ITEMS_NEEDED)// || maphan->get_enemies_count() == 0)
      game_condition = 1;
    else;

    if(game_condition != 0)
      return;

    if(maphan->check_for_pickup()){
      items_collected++;
      maphan->reset_pickup_status();
    }
    else;
      
    vector<Enemy*> enemies = maphan->get_enemies();
    for(int i = 0; i < enemies.size(); ++i){
      if(enemies[i]->get_shot_fired()){
	spawn_projectile(enemies[i]);
	enemies[i]->flip_shot_fired();
      }
    }
 
   if(player->get_shot_fired()){
      spawn_projectile(player);
      player->flip_shot_fired();
    }
   else;

   if(player->get_uber_hit_count() >= PLAYER_UBER_HIT_TARGET){
     spawn_projectile(player, 45.0f);
     spawn_projectile(player, 90.0f);
     spawn_projectile(player, 135.0f);
     spawn_projectile(player, 180.0f);
     spawn_projectile(player, 225.0f);
     spawn_projectile(player, 270.0f);
     spawn_projectile(player, 315.0f);
     spawn_projectile(player, 360.0f);
     player->reset_uber_count();
    }
   else;

    maphan->update();
  }

};


#endif
