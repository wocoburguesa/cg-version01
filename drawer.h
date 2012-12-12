#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <math.h>
#include <unistd.h>
#include <iostream>

#include "game_handler.h"

#ifndef DRAWER_H
#define DRAWER_H

using namespace std;

class ObjectDrawer{
 private:
  GameHandler * gamehan;
  PlayerHandler * playerhan;
  float camera_distance;

 public:
  ObjectDrawer(GameHandler * ghan, PlayerHandler * phan){
    gamehan = ghan;
    playerhan = phan;
    camera_distance = 10.f;
  }

  void draw_box(vector<Point3D> &corners,
		float bottom, float top, bool is_bound=0){
    Point3D normal;

    glBegin(GL_QUADS);
    //Front
    normal = get_normal(Point3D(corners[3].x, corners[3].y, top),
			Point3D(corners[3].x, corners[3].y, bottom),
			Point3D(corners[0].x, corners[0].y, bottom));
    glNormal3f(normal.x, normal.y, normal.z);
    glVertex3f(corners[3].x, corners[3].y, top);        //top left
    glVertex3f(corners[3].x, corners[3].y, bottom);  //bottom left
    glVertex3f(corners[0].x, corners[0].y, bottom);//bottom right
    glVertex3f(corners[0].x, corners[0].y, top);      //top right

    //Left
    normal = get_normal(Point3D(corners[0].x, corners[0].y, top),
			Point3D(corners[0].x, corners[0].y, bottom),
			Point3D(corners[1].x, corners[1].y, bottom));
    glNormal3f(normal.x, normal.y, normal.z);
    glVertex3f(corners[0].x, corners[0].y, top);        //top left
    glVertex3f(corners[0].x, corners[0].y, bottom);  //bottom left
    glVertex3f(corners[1].x, corners[1].y, bottom);//bottom right
    glVertex3f(corners[1].x, corners[1].y, top);      //top right
    
    if(is_bound)
      return;
    else;

    //Bottom
    normal = get_normal(Point3D(corners[3].x, corners[3].y, bottom),
			Point3D(corners[2].x, corners[2].y, bottom),
			Point3D(corners[1].x, corners[1].y, bottom));
    glNormal3f(normal.x, normal.y, normal.z);
    glVertex3f(corners[3].x, corners[3].y, bottom);        //top left
    glVertex3f(corners[2].x, corners[2].y, bottom);  //bottom left
    glVertex3f(corners[1].x, corners[1].y, bottom);//bottom right
    glVertex3f(corners[0].x, corners[0].y, bottom);      //top right

    //Top
    normal = get_normal(Point3D(corners[0].x, corners[0].y, top),
			Point3D(corners[1].x, corners[1].y, top),
			Point3D(corners[2].x, corners[2].y, top));
    glNormal3f(normal.x, normal.y, normal.z);
    glVertex3f(corners[0].x, corners[0].y, top);        //top left
    glVertex3f(corners[1].x, corners[1].y, top);  //bottom left
    glVertex3f(corners[2].x, corners[2].y, top);//bottom right
    glVertex3f(corners[3].x, corners[3].y, top);      //top right

    //Back
    normal = get_normal(Point3D(corners[1].x, corners[1].y, top),
			Point3D(corners[1].x, corners[1].y, bottom),
			Point3D(corners[2].x, corners[2].y, bottom));
    glNormal3f(normal.x, normal.y, normal.z);
    glVertex3f(corners[1].x, corners[1].y, top);        //top left
    glVertex3f(corners[1].x, corners[1].y, bottom);  //bottom left
    glVertex3f(corners[2].x, corners[2].y, bottom);//bottom right
    glVertex3f(corners[2].x, corners[2].y, top);      //top right

    //Right
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
    vector<Point3D> corners = playerhan->get_corners();
    float bottom = playerhan->get_center().z - playerhan->get_height()/2.0f;
    float top = playerhan->get_center().z + playerhan->get_height()/2.0f;

    Point3D color = playerhan->get_color();
    glColor3f(color.x, color.y, color.z);

    draw_box(corners, bottom, top);
  }

  void draw_floor(){
    glColor3f(1.0f, 1.0f, 1.0f);

    glBegin(GL_QUADS);
    glVertex3f(0.0f, 120.0f, 0.0f);        //top left
    glVertex3f(0.0f, 0.0f, 0.0f);        //top left
    glVertex3f(120.0f, 0.0f, 0.0f);        //top left
    glVertex3f(120.0f, 120.0f, 0.0f);        //top left
    glEnd();
  }

  void draw_buildings(){
    MapHandler * maphan = gamehan->get_maphan();
    float height;

    for(int i = 0; i < maphan->get_buildings_count(); ++i){
      StaticObject * curr = maphan->get_building_by_idx(i);
      float bottom = curr->get_center().z - curr->get_height()/2.0f;
      float top = curr->get_center().z + curr->get_height()/2.0f;

      vector<Point3D> corners = curr->get_corners();
      Point3D color = curr->get_color();
      glColor3f(color.x, color.y, color.z);
      
      draw_box(corners, bottom, top);
    }
  }

  void draw_bounds(){
    MapHandler * maphan = gamehan->get_maphan();
    float height;

    for(int i = 0; i < maphan->get_bounds_count(); ++i){
      StaticObject * curr = maphan->get_bound_by_idx(i);
      float bottom = curr->get_center().z - curr->get_height()/2.0f;
      float top = curr->get_center().z + curr->get_height()/2.0f;

      vector<Point3D> corners = curr->get_corners();
      Point3D color = curr->get_color();
      glColor3f(color.x, color.y, color.z);
      
      draw_box(corners, bottom, top, 1);
    }
  }

  void draw_pickups(){
    MapHandler * maphan = gamehan->get_maphan();
    float height;

    for(int i = 0; i < maphan->get_pickups_count(); ++i){
      StaticObject * curr = maphan->get_pickup_by_idx(i);
      float bottom = curr->get_center().z - curr->get_height()/2.0f;
      float top = curr->get_center().z + curr->get_height()/2.0f;

      vector<Point3D> corners = curr->get_corners();
      Point3D color = curr->get_color();
      glColor3f(color.x, color.y, color.z);

      draw_box(corners, bottom, top);
    }
  }

  void draw_enemies(){
    MapHandler * maphan = gamehan->get_maphan();
    float height;

    for(int i = 0; i < maphan->get_enemies_count(); ++i){
      Enemy * curr = maphan->get_enemy_by_idx(i);
      float bottom = curr->get_center().z - curr->get_height()/2.0f;
      float top = curr->get_center().z + curr->get_height()/2.0f;

      vector<Point3D> corners = curr->get_corners();
      Point3D color = curr->get_color();
      glColor3f(color.x, color.y, color.z);

      draw_box(corners, bottom, top);
    }
  }

  void draw_projectiles(){
    MapHandler * maphan = gamehan->get_maphan();
    float height;
  
    for(int i = 0; i < maphan->get_projectiles_count(); ++i){
      Projectile * curr = maphan->get_projectile_by_idx(i);
      float bottom = curr->get_center().z - curr->get_height()/2.0f;
      float top = curr->get_center().z + curr->get_height()/2.0f;

      vector<Point3D> corners = curr->get_corners();
      Point3D color = curr->get_color();
      glColor3f(color.x, color.y, color.z);

      draw_box(corners, bottom, top);
    }
  }

  void normalize(Point3D * vector){
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

    //a
    a.x = v1.x - v2.x;
    a.y = v1.y - v2.y;
    a.z = v1.z - v2.z;
    //b
    b.x = v2.x - v3.x;
    b.y = v2.y - v3.y;
    b.z = v2.z - v3.z;

    normal.x = (a.y * b.z) - (a.z * b.y);
    normal.y = (a.z * b.x) - (a.x * b.z);
    normal.z = (a.x * b.y) - (a.y * b.x);

    normalize(&normal);
    return normal;
  }
};

#endif
