#include <stdlib.h>
#include <vector>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include "game_handler.h"
#include "drawer.h"
#include "SOIL.h"

GameHandler * gamehan;
PlayerHandler * playerhan;
ObjectDrawer * drawer;

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

  drawer->draw_floor();
  drawer->draw_player();
  drawer->draw_buildings();
  drawer->draw_enemies();
  drawer->draw_projectiles();
  drawer->draw_pickups();

  drawer->draw_item_counter(gamehan->get_items_collected());
  drawer->draw_item_counter_icon();
  drawer->draw_lifebar();

  if(gamehan->check_game_condition() != 0)
    drawer->draw_endgame_billboard();

  glutSwapBuffers();
}

void normal_key_handler(unsigned char key, int x, int y) {
  if (key == 27)
    exit(0);
  if(gamehan->check_game_condition() != 0)
    if(key == 13){
      gamehan->set_game();
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
				PLAYER_HEIGHT,
				PLAYER_MAX_SPEED,
				PLAYER_ACCELERATION,
				PLAYER_FRICTION,
				PLAYER_SIZE,
				PLAYER_STARTING_ANGLE,
				PLAYER_STARTING_HEALTH);
  gamehan = new GameHandler(playerhan);
  drawer = new ObjectDrawer(gamehan, playerhan);

  gamehan->set_game();

  // init GLUT and create window
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowPosition(-1, -1);
  glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
  glutCreateWindow("Carritos en acidos");

  drawer->load_textures();

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
