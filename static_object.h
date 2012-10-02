#include <math.h>
#include <unistd.h>
#include <iostream>
#define POINT pair<float, float>
#define INF 0xffffffff

using namespace std;

class StaticObject{
 private:
  float x;
  float y;
  int vertices;
  float radius;
  vector< POINT > corners;
  vector< pair<float, float> > equations;

 public:
  StaticObject(){ vertices = 0; };

  StaticObject(vector< POINT > &c){
    corners = c;
    vertices = c.size();
    x = (corners[0].first + corners[3].first)/2;
    y = (corners[0].second + corners[1].second)/2;
    radius = sqrt((corners[0].first - x)*(corners[0].first - x) +
		  (corners[0].second - y)*(corners[0].second - y));
    for(int i = 1; i < corners.size(); ++i){
      float buff = sqrt((corners[i].first - x)*(corners[i].first - x) +
			(corners[i].second - y)*(corners[i].second - y));
      if(buff > radius)
	radius = buff;
    }

    for(int i = 0; i < corners.size(); ++i){
      float slope, intercept, x1, y1, x2, y2;
      x1 = corners[i].first;
      y1 = corners[i].second;
      x2 = corners[(i+1)%corners.size()].first;
      y2 = corners[(i+1)%corners.size()].second;
      if(x1 != x2){
	slope = (y1 - y2)/(x1 - x2);
	intercept = y1 - slope * x1;
	equations.push_back(pair<float, float>(slope, intercept));
      }
      else{
	equations.push_back(pair<float, float>(x1, INF));
      }
    }
  }

  void set_vertices(vector< POINT > &c){
    corners = c;
    vertices = c.size();
    x = (corners[0].first + corners[3].first)/2;
    y = (corners[0].second + corners[1].second)/2;
    radius = sqrt((corners[0].first - x)*(corners[0].first - x) +
		  (corners[0].second - y)*(corners[0].second - y));
    for(int i = 1; i < corners.size(); ++i){
      float buff = sqrt((corners[i].first - x)*(corners[i].first - x) +
			(corners[i].second - y)*(corners[i].second - y));
      if(buff > radius)
	radius = buff;
    }
  }

  vector< POINT > get_corners(){
    return corners;
  }

  POINT get_x_y(){
    return POINT(x, y);
  }

  float get_radius(){
    return radius;
  }

  vector<POINT> get_equations(){
    return equations;
  }
};
