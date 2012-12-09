#include <math.h>
#include <unistd.h>
#include <iostream>
#include "constants.h"
#include "p_moving_object.h"

#ifndef CAMERA_HANDLER_H
#define CAMERA_HANDLER_H

using namespace std;

class CameraHandler{
 protected:
  Point3D center;
  MovingObject * focus_object;
  Point3D focus_point;
  Point3D up_vector;

 public:
  CameraHandler(MovingObject * focus_obj){
    focus_object = focus_obj;
    focus_point = Point3D(focus_object->get_center().x,
			  focus_object->get_center().y,
			  focus_object->get_center().z);

    up_vector = Point3D(UP_X, UP_Y, UP_Z);
    center = Point3D(focus_object->get_center().x + CENTER_DIFF_X,
		     focus_object->get_center().y + CENTER_DIFF_Y,
		     focus_object->get_center().z + CENTER_DIFF_Z);
  }

  Point3D get_center(){ return center; };
  Point3D get_focus(){ return focus_point; };
  Point3D get_up(){ return up_vector; };

  void update(){
    up_vector = Point3D(focus_object->get_mov_vector().x,
			focus_object->get_mov_vector().y,
			UP_Z);

    focus_point = Point3D(focus_object->get_center().x,
			  focus_object->get_center().y,
			  focus_object->get_center().z);

    // cambiar CENTER_DIFF_Y a camera distance o algo
    center = Point3D(focus_object->get_center().x + focus_object->get_mov_vector().x*CENTER_DIFF_Y,
		     focus_object->get_center().y + focus_object->get_mov_vector().y*CENTER_DIFF_Y,
		     focus_object->get_center().z + CENTER_DIFF_Z);
  }

  float distance(Point3D a, Point3D b){
    return sqrt((a.x - b.x)*(a.x - b.x) +
		(a.y - b.y)*(a.y - b.y) + 
		(a.z - b.z)*(a.z - b.z));
  }
};

#endif
