/*
NOTAS:
-frustrum para el dibujado de solo lo necesario
-investigar push y pop matrix
 */

#include <stdlib.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include "movement.h"

#define UP 0
#define LEFT 1
#define DOWN 2
#define RIGHT 3

MovementHandler * movhan;

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

void renderScene(void) {
  // Clear Color and Depth Buffers
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Reset transformations
  glLoadIdentity();
 
  // Set the camera
  gluLookAt(0.0f, 0.0f, 10.0f,
	    0.0f, 0.0f,  0.0f,
	    0.0f, 1.0f,  0.0f);
  
  glColor3f(0.0f, 0.0f, 0.9f);

  movhan->update();
  pair<float, float> top_left = movhan->get_top_left();
  pair<float, float> bottom_left = movhan->get_bottom_left();
  pair<float, float> bottom_right = movhan->get_bottom_right();
  pair<float, float> top_right = movhan->get_top_right();

  //draw point
  glBegin(GL_QUADS);
  glVertex3f(top_left.first, top_left.second, 0.0f);        //top left
  glVertex3f(bottom_left.first, bottom_left.second, 0.0f);  //bottom left
  glVertex3f(bottom_right.first, bottom_right.second, 0.0f);//bottom right
  glVertex3f(top_right.first, top_right.second, 0.0f);      //top right
  glEnd();
  
  glutSwapBuffers();
}

void normal_key_handler(unsigned char key, int x, int y) {
  if (key == 27)
    exit(0);
  switch(key){
  case 'w' :
    movhan->push_forward(); break;
  case 's' :
    movhan->push_back(); break;
  case 'a' :
    movhan->push_left(); break;
  case 'd' : 
    movhan->push_right(); break;
  }
}

void special_key_handler(int key, int x, int y) {
  switch(key){
  case GLUT_KEY_UP :
    movhan->push_forward(); break;
  case GLUT_KEY_DOWN :
    movhan->push_back(); break;
  case GLUT_KEY_LEFT :
    movhan->push_left(); break;
  case GLUT_KEY_RIGHT : 
    movhan->push_right(); break;
  }
}

void normal_release_handler(unsigned char key, int x, int y) {
  switch(key){
  case 'w' :
    movhan->release_forward(); break;
  case 's' :
    movhan->release_back(); break;
  case 'a' :
    movhan->release_left(); break;
  case 'd' : 
    movhan->release_right(); break;
  }
}

void special_release_handler(int key, int x, int y) {
  switch(key){
  case GLUT_KEY_UP :
    movhan->release_forward(); break;
  case GLUT_KEY_DOWN :
    movhan->release_back(); break;
  case GLUT_KEY_LEFT :
    movhan->release_left(); break;
  case GLUT_KEY_RIGHT : 
    movhan->release_right(); break;
  }
}

int main(int argc, char **argv) {
  
  movhan = new MovementHandler(0.1f,
			       0.0000001f,
			       0.00000005f,
			       1.154700538f);

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
