#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <math.h>
#include <unistd.h>
#include <iostream>

#include "game_handler.h"
#include "SOIL.h"

#ifndef DRAWER_H
#define DRAWER_H

#define PLAYER_NORMAL 0
#define PLAYER_FIRING 1
#define ENEMY_NORMAL 2
#define ENEMY_FIRING 3
#define ENEMY_DAMAGE1 4
#define ENEMY_DAMAGE1_FIRING 5
#define ENEMY_DAMAGE2 6
#define ENEMY_DAMAGE2_FIRING 7
#define ENEMY_DAMAGE3 8
#define ENEMY_DAMAGE3_FIRING 9
#define BULLET 10
#define FLOOR 11

#define PLAYER_NORMAL_MASK 12
#define PLAYER_FIRING_MASK 13
#define ENEMY_NORMAL_MASK 14
#define ENEMY_FIRING_MASK 15
#define ENEMY_DAMAGE1_MASK 16
#define ENEMY_DAMAGE1_FIRING_MASK 17
#define ENEMY_DAMAGE2_MASK 18
#define ENEMY_DAMAGE2_FIRING_MASK 19
#define ENEMY_DAMAGE3_MASK 20
#define ENEMY_DAMAGE3_FIRING_MASK 21
#define BULLET_MASK 22

#define WIN_BILLBOARD 23
#define LOSE_BILLBOARD 24

#define PICKUP 25
#define PICKUP_MASK 26

#define ZERO_OF_SEVEN 27
#define ZERO_OF_SEVEN_MASK 28
#define ONE_OF_SEVEN 29
#define ONE_OF_SEVEN_MASK 30
#define TWO_OF_SEVEN 31
#define TWO_OF_SEVEN_MASK 32
#define THREE_OF_SEVEN 33
#define THREE_OF_SEVEN_MASK 34
#define FOUR_OF_SEVEN 35
#define FOUR_OF_SEVEN_MASK 36
#define FIVE_OF_SEVEN 37
#define FIVE_OF_SEVEN_MASK 38
#define SIX_OF_SEVEN 39
#define SIX_OF_SEVEN_MASK 40

using namespace std;

class ObjectDrawer{
 private:
  GameHandler * gamehan;
  PlayerHandler * playerhan;
  float game_over_billboard_distance;
  float camera_distance;
  GLuint * textures;

 public:
  ObjectDrawer(GameHandler * ghan, PlayerHandler * phan){
    gamehan = ghan;
    playerhan = phan;
    game_over_billboard_distance = 0.0f;
    camera_distance = 10.f;
    textures = new GLuint[41];
  }

  void load_texture(int idx, string filename){
    textures[idx] = SOIL_load_OGL_texture
      (
       ("textures/" + filename).c_str(),
       SOIL_LOAD_RGBA,
       SOIL_CREATE_NEW_ID,
       SOIL_FLAG_POWER_OF_TWO | SOIL_FLAG_MULTIPLY_ALPHA
       );
  }

  void load_textures(){
    load_texture(PLAYER_NORMAL, "player_normal.bmp");
    load_texture(PLAYER_FIRING, "player_firing.bmp");
    load_texture(ENEMY_NORMAL, "enemy_normal.bmp");
    load_texture(ENEMY_FIRING, "enemy_firing.bmp");
    load_texture(ENEMY_DAMAGE1, "enemy_damage1.bmp");
    load_texture(ENEMY_DAMAGE1_FIRING, "enemy_damage1_firing.bmp");
    load_texture(ENEMY_DAMAGE2, "enemy_damage2.bmp");
    load_texture(ENEMY_DAMAGE2_FIRING, "enemy_damage2_firing.bmp");
    load_texture(ENEMY_DAMAGE3, "enemy_damage3.bmp");
    load_texture(ENEMY_DAMAGE3_FIRING, "enemy_damage3_firing.bmp");
    load_texture(BULLET, "bullet_fire.bmp");
    load_texture(FLOOR, "floor.bmp");

    load_texture(PLAYER_NORMAL_MASK, "player_normal_mask.bmp");
    load_texture(PLAYER_FIRING_MASK, "player_firing_mask.bmp");
    load_texture(ENEMY_NORMAL_MASK, "enemy_normal_mask.bmp");
    load_texture(ENEMY_FIRING_MASK, "enemy_firing_mask.bmp");
    load_texture(ENEMY_DAMAGE1_MASK, "enemy_damage1_mask.bmp");
    load_texture(ENEMY_DAMAGE1_FIRING_MASK, "enemy_damage1_firing_mask.bmp");
    load_texture(ENEMY_DAMAGE2_MASK, "enemy_damage2_mask.bmp");
    load_texture(ENEMY_DAMAGE2_FIRING_MASK, "enemy_damage2_firing_mask.bmp");
    load_texture(ENEMY_DAMAGE3_MASK, "enemy_damage3_mask.bmp");
    load_texture(ENEMY_DAMAGE3_FIRING_MASK, "enemy_damage3_firing_mask.bmp");
    load_texture(BULLET_MASK, "bullet_fire_mask.bmp");

    load_texture(WIN_BILLBOARD, "win_billboard.bmp");
    load_texture(LOSE_BILLBOARD, "lose_billboard.bmp");

    load_texture(PICKUP, "pickup_item.bmp");
    load_texture(PICKUP_MASK, "pickup_item_mask.bmp");

    load_texture(ZERO_OF_SEVEN, "0_of_7.bmp");
    load_texture(ZERO_OF_SEVEN_MASK, "0_of_7_mask.bmp");
    load_texture(ONE_OF_SEVEN, "1_of_7.bmp");
    load_texture(ONE_OF_SEVEN_MASK, "1_of_7_mask.bmp");
    load_texture(TWO_OF_SEVEN, "2_of_7.bmp");
    load_texture(TWO_OF_SEVEN_MASK, "2_of_7_mask.bmp");
    load_texture(THREE_OF_SEVEN, "3_of_7.bmp");
    load_texture(THREE_OF_SEVEN_MASK, "3_of_7_mask.bmp");
    load_texture(FOUR_OF_SEVEN, "4_of_7.bmp");
    load_texture(FOUR_OF_SEVEN_MASK, "4_of_7_mask.bmp");
    load_texture(FIVE_OF_SEVEN, "5_of_7.bmp");
    load_texture(FIVE_OF_SEVEN_MASK, "5_of_7_mask.bmp");
    load_texture(SIX_OF_SEVEN, "6_of_7.bmp");
    load_texture(SIX_OF_SEVEN_MASK, "6_of_7_mask.bmp");
  }

  void draw_mask(int mask_idx,
		 vector<POINT> &corners,
		 float z_axis=0.0f,
		 bool turn=0){
    glDisable(GL_DEPTH_TEST);
    glBlendFunc(GL_DST_COLOR,GL_ZERO);
    glBindTexture(GL_TEXTURE_2D, textures[mask_idx]);
    glBegin(GL_QUADS);
    if(!turn){
      glTexCoord2f(1.0f, 0.0f);
      glVertex3f(corners[0].first, corners[0].second, z_axis);
      glTexCoord2f(0.0f, 0.0f);
      glVertex3f(corners[1].first, corners[1].second, z_axis);
      glTexCoord2f(0.0f, 1.0f);
      glVertex3f(corners[2].first, corners[2].second, z_axis);
      glTexCoord2f(1.0f, 1.0f);
      glVertex3f(corners[3].first, corners[3].second, z_axis);
    }
    else{
      glTexCoord2f(0.0f, 0.0f);
      glVertex3f(corners[0].first, corners[0].second, z_axis);
      glTexCoord2f(0.0f, 1.0f);
      glVertex3f(corners[1].first, corners[1].second, z_axis);
      glTexCoord2f(1.0f, 1.0f);
      glVertex3f(corners[2].first, corners[2].second, z_axis);
      glTexCoord2f(1.0f, 0.0f);
      glVertex3f(corners[3].first, corners[3].second, z_axis);
    }
    glEnd();
  }

  void draw_box(vector<Point3D> &corners, float bottom, float top){
    Point3D normal;

    glBegin(GL_QUADS);
    //Bottom
    glColor3f(1.0f, 0.0f, 0.0f);
    normal = get_normal(Point3D(corners[3].x, corners[3].y, bottom),
			Point3D(corners[2].x, corners[2].y, bottom),
			Point3D(corners[1].x, corners[1].y, bottom));
    glNormal3f(normal.x, normal.y, normal.z);
    //    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(corners[3].x, corners[3].y, bottom);        //top left
    //    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(corners[2].x, corners[2].y, bottom);  //bottom left
    //    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(corners[1].x, corners[1].y, bottom);//bottom right
    //    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(corners[0].x, corners[0].y, bottom);      //top right

    //Top
    glColor3f(0.0f, 1.0f, 0.0f);
    normal = get_normal(Point3D(corners[0].x, corners[0].y, top),
			Point3D(corners[1].x, corners[1].y, top),
			Point3D(corners[2].x, corners[2].y, top));
    glNormal3f(normal.x, normal.y, normal.z);
    glVertex3f(corners[0].x, corners[0].y, top);        //top left
    
    glVertex3f(corners[1].x, corners[1].y, top);  //bottom left
    glVertex3f(corners[2].x, corners[2].y, top);//bottom right
    glVertex3f(corners[3].x, corners[3].y, top);      //top right

    //Back
    glColor3f(0.0f, 0.0f, 1.0f);
    normal = get_normal(Point3D(corners[1].x, corners[1].y, top),
			Point3D(corners[1].x, corners[1].y, bottom),
			Point3D(corners[2].x, corners[2].y, bottom));
    glNormal3f(normal.x, normal.y, normal.z);
    glVertex3f(corners[1].x, corners[1].y, top);        //top left
    glVertex3f(corners[1].x, corners[1].y, bottom);  //bottom left
    glVertex3f(corners[2].x, corners[2].y, bottom);//bottom right
    glVertex3f(corners[2].x, corners[2].y, top);      //top right

    //Front
    glColor3f(0.0f, 0.0f, 1.0f);
    normal = get_normal(Point3D(corners[3].x, corners[3].y, top),
			Point3D(corners[3].x, corners[3].y, bottom),
			Point3D(corners[0].x, corners[0].y, bottom));
    glNormal3f(normal.x, normal.y, normal.z);
    glVertex3f(corners[3].x, corners[3].y, top);        //top left
    glVertex3f(corners[3].x, corners[3].y, bottom);  //bottom left
    glVertex3f(corners[0].x, corners[0].y, bottom);//bottom right
    glVertex3f(corners[0].x, corners[0].y, top);      //top right

    //Left
    glColor3f(0.0f, 0.0f, 1.0f);
    normal = get_normal(Point3D(corners[0].x, corners[0].y, top),
			Point3D(corners[0].x, corners[0].y, bottom),
			Point3D(corners[1].x, corners[1].y, bottom));
    glNormal3f(normal.x, normal.y, normal.z);
    glVertex3f(corners[0].x, corners[0].y, top);        //top left
    glVertex3f(corners[0].x, corners[0].y, bottom);  //bottom left
    glVertex3f(corners[1].x, corners[1].y, bottom);//bottom right
    glVertex3f(corners[1].x, corners[1].y, top);      //top right

    //Right
    glColor3f(0.0f, 0.0f, 1.0f);
    normal = get_normal(Point3D(corners[2].x, corners[2].y, top),
			Point3D(corners[2].x, corners[2].y, bottom),
			Point3D(corners[3].x, corners[3].y, bottom));
    glNormal3f(normal.x, normal.y, normal.z);
    glVertex3f(corners[2].x, corners[2].y, top);        //top left
    glVertex3f(corners[2].x, corners[2].y, bottom);  //bottom left
    glVertex3f(corners[3].x, corners[3].y, bottom);//bottom right
    glVertex3f(corners[3].x, corners[3].y, top);      //top right
    glEnd();
  }
  
  void draw_player(){
    //    glEnable(GL_TEXTURE_2D);

    vector<Point3D> corners = playerhan->get_corners();
    float bottom = playerhan->get_center().z - playerhan->get_height()/2.0f;
    float top = playerhan->get_center().z + playerhan->get_height()/2.0f;
    
    draw_box(corners, bottom, top);
    //    glEnable(GL_BLEND);
  
    //draw mask for alpha blending
    /*    if(playerhan->get_flare_shown() > 0)
      draw_mask(PLAYER_FIRING_MASK, corners);
    else
    draw_mask(PLAYER_NORMAL_MASK, corners);*/

    /*    glBlendFunc(GL_ONE,GL_ONE);
    if(playerhan->get_flare_shown() > 0)
      glBindTexture(GL_TEXTURE_2D, textures[PLAYER_FIRING]);
    else
    glBindTexture(GL_TEXTURE_2D, textures[PLAYER_NORMAL]);*/

    //    glDisable(GL_TEXTURE_2D);
    //    glDisable(GL_BLEND);
  }

  void draw_floor(){
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textures[FLOOR]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-50.0f, 50.0f, 0.0f);        //top left
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-50.0f, -50.0f, 0.0f);        //top left
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(50.0f, -50.0f, 0.0f);        //top left
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(50.0f, 50.0f, 0.0f);        //top left
    glEnd();
    glDisable(GL_TEXTURE_2D);
  }

  void draw_buildings(){
    MapHandler * maphan = gamehan->get_maphan();
    float height;

    for(int i = 0; i < maphan->get_buildings_count(); ++i){
      StaticObject * curr = maphan->get_building_by_idx(i);
      float bottom = curr->get_center().z - curr->get_height()/2.0f;
      float top = curr->get_center().z + curr->get_height()/2.0f;

      vector<Point3D> corners = curr->get_corners();
      glColor3f(0.7f, 0.7f, 0.7f);
      
      draw_box(corners, bottom, top);
    }
  }

  void draw_pickups(){
    MapHandler * maphan = gamehan->get_maphan();
    float height;

    glColor3f(1.0f, 1.0f, 1.0f);

    for(int i = 0; i < maphan->get_pickups_count(); ++i){
      StaticObject * curr = maphan->get_pickup_by_idx(i);
      float bottom = curr->get_center().z - curr->get_height()/2.0f;
      float top = curr->get_center().z + curr->get_height()/2.0f;

      vector<Point3D> corners = curr->get_corners();

      draw_box(corners, bottom, top);

      /*      for(int j = 0; j < corners.size(); ++j){
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	//draw mask for alpha blending
	draw_mask(PICKUP_MASK, corners);

	glBlendFunc(GL_ONE,GL_ONE);
	glBindTexture(GL_TEXTURE_2D, textures[PICKUP]);
      
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(corners[0].first, corners[0].second, 0.0f);        //top left
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(corners[1].first, corners[1].second, 0.0f);  //bottom left
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(corners[2].first, corners[2].second, 0.0f);//bottom right
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(corners[3].first, corners[3].second, 0.0f);      //top right
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	}*/
    }
  }

  void draw_enemies(){
    MapHandler * maphan = gamehan->get_maphan();
    float height;

    //    glEnable(GL_TEXTURE_2D);
    //    glEnable(GL_BLEND);
  
    glColor3f(1.0f, 1.0f, 1.0f);
    for(int i = 0; i < maphan->get_enemies_count(); ++i){
      Enemy * curr = maphan->get_enemy_by_idx(i);
      float bottom = curr->get_center().z - curr->get_height()/2.0f;
      float top = curr->get_center().z + curr->get_height()/2.0f;

      vector<Point3D> corners = curr->get_corners();

      draw_box(corners, bottom, top);

 /*      // draw mask for alpha blending
      if(curr->get_flare_shown() > 0)
	draw_mask(ENEMY_FIRING_MASK, corners);
      else
	draw_mask(ENEMY_NORMAL_MASK, corners);

      // setting correct blending mode
      glBlendFunc(GL_ONE,GL_ONE);

      if(curr->get_life() > 300.0f)
	if(curr->get_flare_shown() > 0)
	  glBindTexture(GL_TEXTURE_2D, textures[ENEMY_FIRING]);
	else
	  glBindTexture(GL_TEXTURE_2D, textures[ENEMY_NORMAL]);
      else if(curr->get_life() > 200.0f)
	if(curr->get_flare_shown() > 0)
	  glBindTexture(GL_TEXTURE_2D, textures[ENEMY_DAMAGE1_FIRING]);
	else
	  glBindTexture(GL_TEXTURE_2D, textures[ENEMY_DAMAGE1]);
      else if(curr->get_life() > 100.0f)
	if(curr->get_flare_shown() > 0)
	  glBindTexture(GL_TEXTURE_2D, textures[ENEMY_DAMAGE2_FIRING]);
	else
	  glBindTexture(GL_TEXTURE_2D, textures[ENEMY_DAMAGE2]);
      else
	if(curr->get_flare_shown() > 0)
	  glBindTexture(GL_TEXTURE_2D, textures[ENEMY_DAMAGE3_FIRING]);
	else
	glBindTexture(GL_TEXTURE_2D, textures[ENEMY_DAMAGE3]);

      glBegin(GL_QUADS);
      glTexCoord2f(1.0f, 0.0f);
      glVertex3f(corners[0].first, corners[0].second, 0.0f);
      glTexCoord2f(0.0f, 0.0f);
      glVertex3f(corners[1].first, corners[1].second, 0.0f);
      glTexCoord2f(0.0f, 1.0f);
      glVertex3f(corners[2].first, corners[2].second, 0.0f);
      glTexCoord2f(1.0f, 1.0f);
      glVertex3f(corners[3].first, corners[3].second, 0.0f);
      glEnd();*/
    }
    //    glDisable(GL_TEXTURE_2D);
    //glDisable(GL_BLEND);
  }

  void draw_projectiles(){
    MapHandler * maphan = gamehan->get_maphan();
    float height;
    /*    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textures[BULLET]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);*/
    glColor3f(1.0f, 0.56863f, 0.0f);
  
    for(int i = 0; i < maphan->get_projectiles_count(); ++i){
      Projectile * curr = maphan->get_projectile_by_idx(i);
      float bottom = curr->get_center().z - curr->get_height()/2.0f;
      float top = curr->get_center().z + curr->get_height()/2.0f;

      vector<Point3D> corners = curr->get_corners();

      draw_box(corners, bottom, top);
      /*    glBegin(GL_QUADS);
      glTexCoord2f(1.0f, 0.0f);
      glVertex3f(corners[0].first, corners[0].second, 0.0f);
      glTexCoord2f(0.0f, 0.0f);
      glVertex3f(corners[1].first, corners[1].second, 0.0f);
      glTexCoord2f(0.0f, 1.0f);
      glVertex3f(corners[2].first, corners[2].second, 0.0f);
      glTexCoord2f(1.0f, 1.0f);
      glVertex3f(corners[3].first, corners[3].second, 0.0f);
      glEnd();*/
    }
    //    glDisable(GL_TEXTURE_2D);
  }

  void draw_lifebar(){
    float player_x = playerhan->get_center().x;
    float player_y = playerhan->get_center().y;

    float life_left = playerhan->get_remaining_life_pct();

    float bottom = player_y - 2.5f;
    float left = player_x - 5.0f;
    float right = player_x - 4.5f;
    float top = player_y - 2.5f + life_left * 1.5f;
    if(life_left < 0.0f)
      top = player_y - 2.5f;
    else;
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);

    glVertex3f(left, player_y-1, camera_distance-7);
    glVertex3f(left, bottom, camera_distance-7);
    glVertex3f(right, bottom, camera_distance-7);
    glVertex3f(right, player_y-1, camera_distance-7);
    glEnd();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE);
    glColor3f((1-life_left)*1.0f, life_left*1.0f, 0.0f);
    glBegin(GL_QUADS);

    glVertex3f(left, top, camera_distance-7);
    glVertex3f(left, bottom, camera_distance-7);
    glVertex3f(right, bottom, camera_distance-7);
    glVertex3f(right, top, camera_distance-7);
    glEnd();
    glDisable(GL_BLEND);
  }

  void draw_item_counter_icon(){
    float player_x = playerhan->get_center().x;
    float player_y = playerhan->get_center().y;

    float x = player_x + 4.75f;
    float y = player_y + 2.25f;

    vector<POINT> corners;
    corners.push_back(pair<float, float>(x-PICKUP_SIZE,
					 y+PICKUP_SIZE));
    corners.push_back(pair<float, float>(x-PICKUP_SIZE,
					 y-PICKUP_SIZE));
    corners.push_back(pair<float, float>(x+PICKUP_SIZE,
					 y-PICKUP_SIZE));
    corners.push_back(pair<float, float>(x+PICKUP_SIZE,
					 y+PICKUP_SIZE));

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    //draw mask for alpha blending
    draw_mask(PICKUP_MASK, corners, camera_distance-7.0f);
  
    glBlendFunc(GL_ONE,GL_ONE);
    glBindTexture(GL_TEXTURE_2D, textures[PICKUP]);
  
    glBegin(GL_QUADS);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(corners[0].first, corners[0].second, camera_distance-7.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(corners[1].first, corners[1].second, camera_distance-7.0f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(corners[2].first, corners[2].second, camera_distance-7.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(corners[3].first, corners[3].second, camera_distance-7.0f);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
  }

  void draw_item_counter(int items){
    float player_x = playerhan->get_center().x;
    float player_y = playerhan->get_center().y;

    float x = player_x + 2.25f;
    float y = player_y + 2.25f;

    vector<POINT> corners;
    corners.push_back(pair<float, float>(x-COUNTER_WIDTH,
					 y+COUNTER_HEIGHT));
    corners.push_back(pair<float, float>(x-COUNTER_WIDTH,
					 y-COUNTER_HEIGHT));
    corners.push_back(pair<float, float>(x+COUNTER_WIDTH,
					 y-COUNTER_HEIGHT));
    corners.push_back(pair<float, float>(x+COUNTER_WIDTH,
					 y+COUNTER_HEIGHT));

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    //draw mask for alpha blending
    draw_mask(ZERO_OF_SEVEN+(items*2), corners, camera_distance-7.0f, 1);
  
    glBlendFunc(GL_ONE,GL_ONE);
    glBindTexture(GL_TEXTURE_2D, textures[ZERO_OF_SEVEN+(items*2)+1]);
  
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(corners[0].first, corners[0].second, camera_distance-7.0f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(corners[1].first, corners[1].second, camera_distance-7.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(corners[2].first, corners[2].second, camera_distance-7.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(corners[3].first, corners[3].second, camera_distance-7.0f);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
  }

  void draw_endgame_billboard(){
    if(game_over_billboard_distance < 12.0f)
      game_over_billboard_distance += 0.05f;
    else;
    float player_x = playerhan->get_center().x;
    float player_y = playerhan->get_center().y;

    glColor3f(1.0f, 1.0f, 1.0f);

    glPushMatrix();
    glRotatef(0.0f, 0.0f, 0.0f, 0.0f);

    if(gamehan->check_game_condition() < 0)
      glBindTexture(GL_TEXTURE_2D, textures[LOSE_BILLBOARD]);
    else
      glBindTexture(GL_TEXTURE_2D, textures[WIN_BILLBOARD]);
  
    glEnable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(player_x-3.148f, player_y+2.0f, camera_distance + 5.0f - game_over_billboard_distance);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(player_x-3.148f, player_y-2.0f, camera_distance + 5.0f - game_over_billboard_distance);

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(player_x+3.148f, player_y-2.0f, camera_distance + 5.0f - game_over_billboard_distance);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(player_x+3.148f, player_y+2.0f, camera_distance + 5.0f - game_over_billboard_distance);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    glPopMatrix();
  }

  void normalize(Point3D * vector)
  {
    // calculate the length of the vector
    float length = (float)(sqrt((vector->x * vector->x) +
				(vector->y * vector->y) +
				(vector->z * vector->z)));

    // avoid division by 0
    if (length == 0.0f)
      length = 1.0f;

    // reduce to unit size
    vector->x /= length;
    vector->y /= length;
    vector->z /= length;
  }
  
  Point3D get_normal(Point3D v1, Point3D v2, Point3D v3){
    Point3D a, b, normal;

    // calculate the vectors A and B
    // note that v[3] is defined with counterclockwise winding in mind
    // a
    a.x = v1.x - v2.x;
    a.y = v1.y - v2.y;
    a.z = v1.z - v2.z;
    // b
    b.x = v2.x - v3.x;
    b.y = v2.y - v3.y;
    b.z = v2.z - v3.z;

    // calculate the cross product and place the resulting vector
    // into the address specified by vector_t *normal
    normal.x = (a.y * b.z) - (a.z * b.y);
    normal.y = (a.z * b.x) - (a.x * b.z);
    normal.z = (a.x * b.y) - (a.y * b.x);

    normalize(&normal);
    return normal;
  }
};

#endif
