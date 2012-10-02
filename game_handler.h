#include <math.h>
#include <unistd.h>
#include <iostream>
#include "map_handler.h"

#define POINT pair<float, float>

using namespace std;

class GameHandler{
 private:
  float min_enemy_size;
  float max_enemy_size;
  float max_enemy_speed;
  float min_enemy_speed;
  float friction;
  MapHandler maphan;
  PlayerHandler * player;

 public:
  GameHandler(PlayerHandler * p){
    player = p;
    maphan.add_player(p);
    min_enemy_size = p->get_size() * 0.8f;
    max_enemy_size = p->get_size() * 1.2f;
    min_enemy_speed = p->get_max_speed() * 0.8f;
    max_enemy_speed = p->get_max_speed() * 1.2f;
  }

  MapHandler get_maphan(){ return maphan; };

  void add_static_object(vector<POINT> &corners){
    maphan.add_static_object(corners);
  }

  void spawn_enemy(){
    float size = rand() % 100;
    size = min_enemy_size + (max_enemy_size - min_enemy_size)*size/100.0f;
    float max_speed = rand() % 100;
    max_speed = min_enemy_speed + (max_enemy_speed - min_enemy_speed)*max_speed/100.0f;
    float x = player->get_x_y().first - 3.0f;
    float y = player->get_x_y().second - 1.0f;
    maphan.add_moving_object(x, y, max_speed, player->get_acceleration(),
			     player->get_friction(), size, 90.0f);
  }

  void update(){
    maphan.update();
  }

};
