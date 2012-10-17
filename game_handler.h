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

  int items_collected;
  int game_condition;

 public:
  GameHandler(PlayerHandler * p){
    items_collected = 0;
    game_condition = 0;
    maphan = new MapHandler();
    player = p;
    maphan->add_player(p);
    lifebar = new LifeBar(p);
  }

  void set_game(){
    items_collected = 0;
    game_condition = 0;
    player->reset_health();

    vector<POINT> building;
    /********** MAP BOUNDS **********/
    building.push_back(pair<float, float>(-(MAP_WIDTH+1.0f),(MAP_WIDTH+1.0f)));
    building.push_back(pair<float, float>(-(MAP_WIDTH+1.0f),-(MAP_WIDTH+1.0f)));
    building.push_back(pair<float, float>(-MAP_WIDTH,-(MAP_WIDTH+1.0f)));
    building.push_back(pair<float, float>(-MAP_WIDTH,(MAP_WIDTH+1.0f)));
    spawn_building(building);
    building.clear();

    building.push_back(pair<float, float>(-MAP_WIDTH,-MAP_WIDTH));
    building.push_back(pair<float, float>(-MAP_WIDTH,-(MAP_WIDTH+1.0f)));
    building.push_back(pair<float, float>((MAP_WIDTH+1.0f),-(MAP_WIDTH+1.0f)));
    building.push_back(pair<float, float>((MAP_WIDTH+1.0f),-MAP_WIDTH));
    spawn_building(building);
    building.clear();

    building.push_back(pair<float, float>(MAP_WIDTH,(MAP_WIDTH+1.0f)));
    building.push_back(pair<float, float>(MAP_WIDTH,-MAP_WIDTH));
    building.push_back(pair<float, float>((MAP_WIDTH+1.0f),-MAP_WIDTH));
    building.push_back(pair<float, float>((MAP_WIDTH+1.0f),(MAP_WIDTH+1.0f)));
    spawn_building(building);
    building.clear();

    building.push_back(pair<float, float>(-MAP_WIDTH,(MAP_WIDTH+1.0f)));
    building.push_back(pair<float, float>(-MAP_WIDTH,MAP_WIDTH));
    building.push_back(pair<float, float>(MAP_WIDTH,MAP_WIDTH));
    building.push_back(pair<float, float>(MAP_WIDTH,(MAP_WIDTH+1.0f)));
    spawn_building(building);
    building.clear();
    /********** MAP BOUNDS **********/

    /********** BUILDINGS **********/
    building.push_back(pair<float, float>(-15.0f, -10.0f));
    building.push_back(pair<float, float>(-15.0f, -16.0f));
    building.push_back(pair<float, float>(-9.0f, -16.0f));
    building.push_back(pair<float, float>(-9.0f, -10.0f));
    spawn_building(building);
    building.clear();

    building.push_back(pair<float, float>(-27.0f, 16.0f));
    building.push_back(pair<float, float>(-27.0f, 10.0f));
    building.push_back(pair<float, float>(-21.0f, 10.0f));
    building.push_back(pair<float, float>(-21.0f, 16.0f));
    spawn_building(building);
    building.clear();

    building.push_back(pair<float, float>(16.0f, 5.0f));
    building.push_back(pair<float, float>(16.0f, -2.0f));
    building.push_back(pair<float, float>(23.0f, -2.0f));
    building.push_back(pair<float, float>(23.0f, 5.0f));
    spawn_building(building);
    building.clear();
    /********** BUILDINGS **********/

    /********** ENEMIES **********/
    spawn_enemy(-21.0f, 18.0f);
    spawn_enemy(-14.0f, 17.0f);
    spawn_enemy(0.0f, 18.0f);
    spawn_enemy(9.0f, 15.0f);
    spawn_enemy(12.0f, 14.0f);
    spawn_enemy(-19.0f, 1.0f);
    spawn_enemy(-20.0f, -8.0f);
    /*spawn_enemy(-21.0f, -11.0f);
    spawn_enemy(-4.0f, -15.0f);
    spawn_enemy(4.0f, -10.0f);*/
  }

  MapHandler* get_maphan(){ return maphan; };
  LifeBar* get_lifebar(){ return lifebar; };
  int check_game_condition(){ return game_condition; };

  int get_items_collected(){ return items_collected; };

  void spawn_building(vector<POINT> &corners){
    maphan->add_static_object(corners, 0);
  }

  void spawn_enemy(float x, float y){
    float size = rand() % 100;
    size = MIN_ENEMY_SIZE + (MAX_ENEMY_SIZE - MIN_ENEMY_SIZE)*size/100.0f;
    float max_speed = rand() % 100;
    max_speed =
      MIN_ENEMY_SPEED + (MAX_ENEMY_SPEED - MIN_ENEMY_SPEED)*max_speed/100.0f;
    /*    float x = player->get_x_y().first - 6.0f;
	  float y = player->get_x_y().second - 1.0f;*/
    float angle = PLAYER_STARTING_ANGLE;
    maphan->add_enemy_object(x, y, max_speed, player->get_acceleration(),
			     player->get_friction(), size, 180.0f,
			     ENEMY_STARTING_HEALTH);
  }

  void spawn_projectile(MovingObject * shooter, float angle=-1.0f){
    if(angle == -1.0f){
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
    else{
      float exit_ang = shooter->get_angle() + angle;
      POINT out_vector(0.0f, 0.0f);
      out_vector.first = cos(exit_ang*PI/180);
      out_vector.second = sin(exit_ang*PI/180);
      float denom =
	sqrt(out_vector.first*out_vector.first +
	     out_vector.second*out_vector.second);
      float factor = shooter->get_radius() / denom;
      float x = shooter->get_x_y().first + out_vector.first * factor;
      float y = shooter->get_x_y().second + out_vector.second * factor;
      maphan->add_projectile(x,
			     y,
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

   cout << player->get_uber_hit_count() << endl;
   if(player->get_uber_hit_count() >= PLAYER_UBER_HIT_TARGET){
     cout << "heyo" << endl;
     spawn_projectile(player, 45.0f);
     spawn_projectile(player, 90.0f);
     spawn_projectile(player, 135.0f);
     spawn_projectile(player, 180.0f);
     spawn_projectile(player, 215.0f);
     spawn_projectile(player, 260.0f);
     spawn_projectile(player, 305.0f);
     spawn_projectile(player, 360.0f);
     player->reset_uber_count();
    }
   else;

    maphan->update();
  }

};
