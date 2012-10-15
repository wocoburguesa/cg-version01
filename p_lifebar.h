#include <math.h>
#include <unistd.h>
#include <iostream>
#include "constants.h"

using namespace std;

class LifeBar{
 private:
  float max_health;
  float remaining_health;

 public:
  LifeBar(PlayerHandler * p){
    max_health = remaining_health = PLAYER_STARTING_HEALTH;
  }

  float get_remaining_health(){ return remaining_health; };
  float get_max_health(){ return remaining_health; };

  float get_remaining_percentage(){
    return remaining_health;
  };

};
