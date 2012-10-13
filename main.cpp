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

#define UP 0
#define LEFT 1
#define DOWN 2
#define RIGHT 3

GameHandler * gamehan;
PlayerHandler * playerhan;
float camera_distance;

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

void renderScene(void){
  // Clear Color and Depth Buffers
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Reset transformations
  glLoadIdentity();
 
  /*  movhan->update();
  float car_x = movhan->get_x_y().first;
  float car_y = movhan->get_x_y().second;
  pair<float, float> top_left = movhan->get_top_left();
  pair<float, float> bottom_left = movhan->get_bottom_left();
  pair<float, float> bottom_right = movhan->get_bottom_right();
  pair<float, float> top_right = movhan->get_top_right();*/

  gamehan->update();
  float player_x = playerhan->get_x_y().first;
  float player_y = playerhan->get_x_y().second;
  pair<float, float> top_left = playerhan->get_top_left();
  pair<float, float> bottom_left = playerhan->get_bottom_left();
  pair<float, float> bottom_right = playerhan->get_bottom_right();
  pair<float, float> top_right = playerhan->get_top_right();

  // Set the camera
  /*  gluLookAt(0.0f, 0.0f, 10.0f,
	    0.0f, 0.0f,  0.0f,
	    0.0f, 1.0f,  0.0f);*/
  gluLookAt(player_x, player_y, camera_distance,
	    player_x, player_y,  0.0f,
	    0.0f, 1.0f,  0.0f);

  glColor3f(0.0f, 0.0f, 0.9f);

  //draw player
  glBegin(GL_QUADS);
  glVertex3f(top_left.first, top_left.second, 0.0f);        //top left
  glVertex3f(bottom_left.first, bottom_left.second, 0.0f);  //bottom left
  glVertex3f(bottom_right.first, bottom_right.second, 0.0f);//bottom right
  glVertex3f(top_right.first, top_right.second, 0.0f);      //top right
  glEnd();

  MapHandler maphan = gamehan->get_maphan();
  //draw static objects
  glColor3f(0.0f, 1.0f, 0.0f);
  
  for(int i = 0; i < maphan.get_sobjects_count(); ++i){
    StaticObject curr = maphan.get_sobject_by_idx(i);
    vector< pair<float, float> > corners = curr.get_corners();
    glBegin(GL_QUADS);
    for(int j = 0; j < corners.size(); ++j)
      glVertex3f(corners[j].first, corners[j].second, 0.0f);
    glEnd();
  }

  //draw moving objects (enemies)
  glColor3f(1.0f, 0.0f, 0.0f);
  
  for(int i = 0; i < maphan.get_mobjects_count(); ++i){
    MovingObject curr = maphan.get_mobject_by_idx(i);
    vector< pair<float, float> > corners = curr.get_corners();
    glBegin(GL_QUADS);
    for(int j = 0; j < corners.size(); ++j)
      glVertex3f(corners[j].first, corners[j].second, 0.0f);
    glEnd();
  }

  glutSwapBuffers();
}

void normal_key_handler(unsigned char key, int x, int y) {
  if (key == 27)
    exit(0);
  switch(key){
  case 'w' :
    playerhan->push_forward(); break;
  case 's' :
    playerhan->push_back(); break;
  case 'a' :
    playerhan->push_left(); break;
  case 'd' : 
    playerhan->push_right(); break;
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
  case GLUT_KEY_SPACE : 
    playerhan->push_space(); break;
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
  camera_distance = 10.f;

  playerhan = new PlayerHandler(0.1f,
				0.0000001f,
				0.00000005f,
				1.154700538f);
  gamehan = new GameHandler(playerhan);

  gamehan->spawn_enemy();

  vector< pair<float, float> > building;
  building.push_back(pair<float, float>(5,3));
  building.push_back(pair<float, float>(5,-3));
  building.push_back(pair<float, float>(11,-3));
  building.push_back(pair<float, float>(11,3));
  gamehan->add_static_object(building);
  building.clear();

  /*  building.push_back(pair<float, float>(-3,3));
  building.push_back(pair<float, float>(-3,-3));
  building.push_back(pair<float, float>(-2,-3));
  building.push_back(pair<float, float>(-2,3));
  maphan->add_static_object(building);
  building.clear();*/

  // init GLUT and create window
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowPosition(10, 10);
  glutInitWindowSize(320, 320);
  glutCreateWindow("Carritos en acidos");

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
