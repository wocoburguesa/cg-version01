#include <math.h>
#include <unistd.h>
#include <iostream>
#include "map_handler.h"
#include "p_lifebar.h"

#define POINT pair<float, float>

using namespace std;

class GameHandler{
 private:
  float friction;
  MapHandler * maphan;
  PlayerHandler * player;
  LifeBar * lifebar;

 public:
  GameHandler(PlayerHandler * p){
    maphan = new MapHandler();
    player = p;
    maphan->add_player(p);
    lifebar = new LifeBar(p);
  }

  MapHandler* get_maphan(){ return maphan; };
  LifeBar* get_lifebar(){ return lifebar; };

  void spawn_building(vector<POINT> &corners){
    maphan->add_static_object(corners);
  }

  void spawn_enemy(){
    float size = rand() % 100;
    size = MIN_ENEMY_SIZE + (MAX_ENEMY_SIZE - MIN_ENEMY_SIZE)*size/100.0f;
    float max_speed = rand() % 100;
    max_speed =
      MIN_ENEMY_SPEED + (MAX_ENEMY_SPEED - MIN_ENEMY_SPEED)*max_speed/100.0f;
    float x = player->get_x_y().first - 6.0f;
    float y = player->get_x_y().second - 1.0f;
    float angle = PLAYER_STARTING_ANGLE;
    maphan->add_enemy_object(x, y, max_speed, player->get_acceleration(),
			     player->get_friction(), size, 0.0f,
			     ENEMY_STARTING_HEALTH);
  }

  void spawn_projectile(MovingObject * shooter){
    POINT shooter_vec = shooter->get_mov_vector();
    float denom =
      sqrt(shooter_vec.first*shooter_vec.first +
	   shooter_vec.second*shooter_vec.second);
    float factor = shooter->get_radius() / denom;
    float x = shooter->get_x_y().first + shooter_vec.first * factor;
    float y = shooter->get_x_y().second + shooter_vec.second * factor;

    maphan->add_projectile(x, y,
			   0.0f, //max speed, overridden
			   0.0f, //accel, overridden
			   PROJECTILE_FRICTION, // friction
			   PROJECTILE_SIZE,
			   shooter->get_angle());
  }

  void update(){
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

    maphan->update();
  }

};
