#include <stdlib.h>
#include <vector>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include "game_handler.h"
#include "camera_handler.h"
#include "drawer.h"
#include "SOIL.h"

GameHandler * gamehan;
PlayerHandler * playerhan;
ObjectDrawer * drawer;
CameraHandler * camera;

float camera_distance;

float game_over_billboard_distance;

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

void init(){
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);

  // Create light components
  GLfloat ambientLight[] = { 0.5f, 0.5f, 0.5f, 0.0f };
  GLfloat diffuseLight[] = { 0.5f, 0.5f, 0.5f, 0.0f };
  GLfloat specularLight[] = { 0.5f, 0.5f, 0.5f, 0.0f };
  GLfloat position[] = { 20.0f, 20.0f, 2.0f, 1.0f };

  // Assign created components to GL_LIGHT0
  glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
  glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
  glLightfv(GL_LIGHT0, GL_POSITION, position);

  // enable color tracking
  glEnable(GL_COLOR_MATERIAL);
  // set material properties which will be assigned by glColor
  glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
}

void renderScene(void){
  // Clear Color and Depth Buffers
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);

  glMatrixMode(GL_MODELVIEW);
  // Reset transformations
  glLoadIdentity();
 
  gluLookAt(camera->get_center().x,
	    camera->get_center().y,
	    camera->get_center().z,
	    camera->get_focus().x,
	    camera->get_focus().y,
	    camera->get_focus().z,
	    camera->get_up().x,
	    camera->get_up().y,
	    camera->get_up().z);

  gamehan->update();
  camera->update();

  drawer->draw_floor();
  drawer->draw_player();
  drawer->draw_buildings();
  drawer->draw_bounds();
  drawer->draw_enemies();
  drawer->draw_projectiles();
  drawer->draw_pickups();

  if(gamehan->check_game_condition() != 0)
    //    drawer->draw_endgame_billboard();
    cout << "game over" << endl;

  glutSwapBuffers();
}

void normal_key_handler(unsigned char key, int x, int y) {
  if (key == 27)
    exit(0);
  if(gamehan->check_game_condition() != 0)
    if(key == 13){
      gamehan->set_game("octagon.lvl");
      game_over_billboard_distance = 0.0f;      
    }
    else
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
  if(gamehan->check_game_condition() != 0)
    return;
  switch(key){
  case GLUT_KEY_UP :
    camera->push_forward(); break;
  case GLUT_KEY_DOWN :
    camera->push_back(); break;
  case GLUT_KEY_LEFT :
    camera->push_left(); break;
  case GLUT_KEY_RIGHT : 
    camera->push_right(); break;
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
    camera->release_forward(); break;
  case GLUT_KEY_DOWN :
    camera->release_back(); break;;
  case GLUT_KEY_LEFT :
    camera->release_left(); break;
  case GLUT_KEY_RIGHT : 
    camera->release_right(); break;
  }
}

int main(int argc, char **argv) {
  game_over_billboard_distance = 0.0f;
  camera_distance = 10.f;

  gamehan = new GameHandler();
  drawer = new ObjectDrawer(gamehan, gamehan->get_player());

  camera = new CameraHandler(gamehan->get_player());

  gamehan->set_game("octagon.lvl");
  playerhan = gamehan->get_player();

  // init GLUT and create window
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowPosition(-1, -1);
  glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
  glutCreateWindow("Carritos en acidos");

  init();

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
