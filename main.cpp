/*
NOTAS:
-frustrum para el dibujado de solo lo necesario
-investigar push y pop matrix
 */

#include <stdlib.h>
#include <vector>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include "game_handler.h"
#include "SOIL.h"

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

GameHandler * gamehan;
PlayerHandler * playerhan;

float camera_distance;

float game_over_billboard_distance;

GLuint textures[12];

int LoadGLTextures(){
  /* load an image file directly as a new OpenGL texture */
  textures[PLAYER_NORMAL] = SOIL_load_OGL_texture
    (
     "textures/player_normal.bmp",
     SOIL_LOAD_AUTO,
     SOIL_CREATE_NEW_ID,
     SOIL_FLAG_POWER_OF_TWO | SOIL_FLAG_MULTIPLY_ALPHA
     );

  textures[PLAYER_FIRING] = SOIL_load_OGL_texture
    (
     "textures/player_firing.bmp",
     SOIL_LOAD_AUTO,
     SOIL_CREATE_NEW_ID,
     SOIL_FLAG_POWER_OF_TWO | SOIL_FLAG_MULTIPLY_ALPHA
     );

  textures[ENEMY_NORMAL] = SOIL_load_OGL_texture
    (
     "textures/enemy_normal.bmp",
     SOIL_LOAD_AUTO,
     SOIL_CREATE_NEW_ID,
     SOIL_FLAG_POWER_OF_TWO | SOIL_FLAG_MULTIPLY_ALPHA
     );
  
  textures[ENEMY_FIRING] = SOIL_load_OGL_texture
    (
     "textures/enemy_firing.bmp",
     SOIL_LOAD_AUTO,
     SOIL_CREATE_NEW_ID,
     SOIL_FLAG_POWER_OF_TWO | SOIL_FLAG_MULTIPLY_ALPHA
     );
  
  textures[ENEMY_DAMAGE1] = SOIL_load_OGL_texture
    (
     "textures/enemy_damage1.bmp",
     SOIL_LOAD_AUTO,
     SOIL_CREATE_NEW_ID,
     SOIL_FLAG_POWER_OF_TWO | SOIL_FLAG_MULTIPLY_ALPHA
     );
  
  textures[ENEMY_DAMAGE1_FIRING] = SOIL_load_OGL_texture
    (
     "textures/enemy_damage1_firing.bmp",
     SOIL_LOAD_AUTO,
     SOIL_CREATE_NEW_ID,
     SOIL_FLAG_POWER_OF_TWO | SOIL_FLAG_MULTIPLY_ALPHA
     );
  
  textures[ENEMY_DAMAGE2] = SOIL_load_OGL_texture
    (
     "textures/enemy_damage2.bmp",
     SOIL_LOAD_AUTO,
     SOIL_CREATE_NEW_ID,
     SOIL_FLAG_POWER_OF_TWO | SOIL_FLAG_MULTIPLY_ALPHA
     );
  
  textures[ENEMY_DAMAGE2_FIRING] = SOIL_load_OGL_texture
    (
     "textures/enemy_damage2_firing.bmp",
     SOIL_LOAD_AUTO,
     SOIL_CREATE_NEW_ID,
     SOIL_FLAG_POWER_OF_TWO | SOIL_FLAG_MULTIPLY_ALPHA
     );
  
  textures[ENEMY_DAMAGE3] = SOIL_load_OGL_texture
    (
     "textures/enemy_damage3.bmp",
     SOIL_LOAD_AUTO,
     SOIL_CREATE_NEW_ID,
     SOIL_FLAG_POWER_OF_TWO | SOIL_FLAG_MULTIPLY_ALPHA
     );
  
  textures[ENEMY_DAMAGE3_FIRING] = SOIL_load_OGL_texture
    (
     "textures/enemy_damage3_firing.bmp",
     SOIL_LOAD_AUTO,
     SOIL_CREATE_NEW_ID,
     SOIL_FLAG_POWER_OF_TWO | SOIL_FLAG_MULTIPLY_ALPHA
     );
  
  textures[BULLET] = SOIL_load_OGL_texture
    (
     "textures/bullet_fire.bmp",
     SOIL_LOAD_AUTO,
     SOIL_CREATE_NEW_ID,
     SOIL_FLAG_POWER_OF_TWO | SOIL_FLAG_MULTIPLY_ALPHA
     );
  
  textures[FLOOR] = SOIL_load_OGL_texture
    (
     "textures/floor.bmp",
     SOIL_LOAD_AUTO,
     SOIL_CREATE_NEW_ID,
     SOIL_FLAG_POWER_OF_TWO
     );
  
  return true;
}

void changeSize(int w, int h) {

  // Prevent a divide by zero, when window is too short
  // (you cant make a window of zero width).
  if (h == 0)
    h = 1;
  float ratio =  w * 1.0 / h;
  
  // Use the Projection Matrix
  glMatrixMode(GL_PROJECTION);
  
  // Reset Matrix
  glLoadIdentity();
  
  // Set the viewport to be the entire window
  glViewport(0, 0, w, h);
  
  // Set the correct perspective.
  gluPerspective(45.0f, ratio, 0.1f, 100.0f);
  
  // Get Back to the Modelview
  glMatrixMode(GL_MODELVIEW);
}

void draw_player(){
  glEnable(GL_TEXTURE_2D);
  if(playerhan->get_flare_shown() > 0)
    glBindTexture(GL_TEXTURE_2D, textures[PLAYER_FIRING]);
  else
    glBindTexture(GL_TEXTURE_2D, textures[PLAYER_NORMAL]);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  vector<POINT> corners = playerhan->get_corners();

  glColor3f(1.0f, 1.0f, 1.0f);
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
  //draw static objects

  for(int i = 0; i < maphan->get_buildings_count(); ++i){
    StaticObject * curr = maphan->get_building_by_idx(i);
    vector<POINT> corners = curr->get_corners();
    glBegin(GL_QUADS);
    for(int j = 0; j < corners.size(); ++j)
      glVertex3f(corners[j].first, corners[j].second, 0.0f);
    glEnd();
  }
}

void draw_enemies(){
  MapHandler * maphan = gamehan->get_maphan();
  //draw moving objects (enemies)
  glEnable(GL_TEXTURE_2D);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glColor3f(1.0f, 1.0f, 1.0f);
  
  for(int i = 0; i < maphan->get_enemies_count(); ++i){
    Enemy * curr = maphan->get_enemy_by_idx(i);
    vector<POINT> corners = curr->get_corners();
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
    glEnd();
  }
  glDisable(GL_TEXTURE_2D);
}

void draw_projectiles(){
  MapHandler * maphan = gamehan->get_maphan();
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, textures[BULLET]);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glColor3f(1.0f, 1.0f, 1.0f);
  
  for(int i = 0; i < maphan->get_projectiles_count(); ++i){
    Projectile * curr = maphan->get_projectile_by_idx(i);
    vector<POINT> corners = curr->get_corners();
    glBegin(GL_QUADS);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(corners[0].first, corners[0].second, 0.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(corners[1].first, corners[1].second, 0.0f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(corners[2].first, corners[2].second, 0.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(corners[3].first, corners[3].second, 0.0f);
    glEnd();
  }
  glDisable(GL_TEXTURE_2D);
}

void draw_lifebar(){
  float player_x = playerhan->get_x_y().first;
  float player_y = playerhan->get_x_y().second;

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

void draw_lose_billboard(){
  if(game_over_billboard_distance < 12.0f)
    game_over_billboard_distance += 0.05f;
  else;
  float player_x = playerhan->get_x_y().first;
  float player_y = playerhan->get_x_y().second;

  glPushMatrix();
  glRotatef(1.0f, 1.0f, 0.0f, 0.0f);
  glBegin(GL_QUADS);
  glVertex3f(-2.0f, 1.0f, camera_distance + 5.0f - game_over_billboard_distance);
  glVertex3f(-2.0f, -1.0f, camera_distance + 5.0f - game_over_billboard_distance);
  glVertex3f(2.0f, -1.0f, camera_distance + 5.0f - game_over_billboard_distance);
  glVertex3f(2.0f, 1.0f, camera_distance + 5.0f - game_over_billboard_distance);
  /*  glVertex3f(player_x-2.0f, player_y, camera_distance + 5.0f - game_over_billboard_distance);
  glVertex3f(player_x, player_y-2.0f, camera_distance + 5.0f - game_over_billboard_distance);
  glVertex3f(player_x+2.0f, player_y, camera_distance + 5.0f - game_over_billboard_distance);
  glVertex3f(player_x, player_y+2.0f, camera_distance + 5.0f - game_over_billboard_distance);*/
  glEnd();
  glPopMatrix();
}

void renderScene(void){
  // Clear Color and Depth Buffers
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);
  // Reset transformations
  glLoadIdentity();
 
  float player_x = playerhan->get_x_y().first;
  float player_y = playerhan->get_x_y().second;
  // Set the camera
  gluLookAt(player_x, player_y, camera_distance,
	    player_x, player_y,  0.0f,
	    0.0f, 1.0f,  0.0f);

  gamehan->update();

  draw_floor();
  draw_player();
  draw_buildings();
  draw_enemies();
  draw_projectiles();
  draw_lifebar();
  if(gamehan->check_game_condition() < 0)
    draw_lose_billboard();

  glutSwapBuffers();
}

void normal_key_handler(unsigned char key, int x, int y) {
  if (key == 27)
    exit(0);
  if(gamehan->check_game_condition() != 0)
    return;
  switch(key){
  case 'w' :
    playerhan->push_forward(); break;
  case 's' :
    playerhan->push_back(); break;
  case 'a' :
    playerhan->push_left(); break;
  case 'd' : 
    playerhan->push_right(); break;
  case ' ' : 
    playerhan->push_space(); break;
  }
}

void special_key_handler(int key, int x, int y) {
  switch(key){
  case GLUT_KEY_UP :
    camera_distance--; break;
  case GLUT_KEY_DOWN :
    camera_distance++; break;
  case GLUT_KEY_LEFT :
    playerhan->push_left(); break;
  case GLUT_KEY_RIGHT : 
    playerhan->push_right(); break;
    /*  case GLUT_KEY_SPACE : 
	playerhan->push_space(); break;*/
  }
}

void normal_release_handler(unsigned char key, int x, int y) {
  switch(key){
  case 'w' :
    playerhan->release_forward(); break;
  case 's' :
    playerhan->release_back(); break;
  case 'a' :
    playerhan->release_left(); break;
  case 'd' : 
    playerhan->release_right(); break;
  }
}

void special_release_handler(int key, int x, int y) {
  switch(key){
  case GLUT_KEY_UP :
    break;
  case GLUT_KEY_DOWN :
    break;
  case GLUT_KEY_LEFT :
    playerhan->release_left(); break;
  case GLUT_KEY_RIGHT : 
    playerhan->release_right(); break;
  }
}

int main(int argc, char **argv) {
  game_over_billboard_distance = 0.0f;
  camera_distance = 10.f;

  playerhan = new PlayerHandler(PLAYER_STARTING_X,
				PLAYER_STARTING_Y,
				PLAYER_MAX_SPEED,
				PLAYER_ACCELERATION,
				PLAYER_FRICTION,
				PLAYER_SIZE,
				PLAYER_STARTING_ANGLE,
				PLAYER_STARTING_HEALTH);
  gamehan = new GameHandler(playerhan);

  gamehan->spawn_enemy();

  vector< pair<float, float> > building;
  building.push_back(pair<float, float>(5,3));
  building.push_back(pair<float, float>(5,-3));
  building.push_back(pair<float, float>(11,-3));
  building.push_back(pair<float, float>(11,3));
  gamehan->spawn_building(building);
  building.clear();

  building.push_back(pair<float, float>(-51,51));
  building.push_back(pair<float, float>(-51,-51));
  building.push_back(pair<float, float>(-50,-51));
  building.push_back(pair<float, float>(-50,51));
  gamehan->spawn_building(building);
  building.clear();

  building.push_back(pair<float, float>(-50,-50));
  building.push_back(pair<float, float>(-50,-51));
  building.push_back(pair<float, float>(51,-51));
  building.push_back(pair<float, float>(51,-50));
  gamehan->spawn_building(building);
  building.clear();

  building.push_back(pair<float, float>(50,51));
  building.push_back(pair<float, float>(50,-50));
  building.push_back(pair<float, float>(51,-50));
  building.push_back(pair<float, float>(51,51));
  gamehan->spawn_building(building);
  building.clear();

  building.push_back(pair<float, float>(-50,51));
  building.push_back(pair<float, float>(-50,50));
  building.push_back(pair<float, float>(50,50));
  building.push_back(pair<float, float>(50,51));
  gamehan->spawn_building(building);
  building.clear();

  // init GLUT and create window
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowPosition(-1, -1);
  glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
  glutCreateWindow("Carritos en acidos");

  LoadGLTextures();

  // register callbacks
  glutDisplayFunc(renderScene);
  glutReshapeFunc(changeSize);
  glutIdleFunc(renderScene);

  //ignore repeats
  glutIgnoreKeyRepeat(1);

  // here are the new entries
  glutKeyboardFunc(normal_key_handler);
  glutSpecialFunc(special_key_handler);
  glutKeyboardUpFunc(normal_release_handler);
  glutSpecialUpFunc(special_release_handler);

  // enter GLUT event processing cycle
  glutMainLoop();
  
  return 0;
}
