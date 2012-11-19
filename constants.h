#include "utils.h"

#define INF 0xffffffff
#define POINT pair<int, int>

using namespace std;

#ifndef CONSTANTS_H
#define CONSTANTS_H

/********** PLAYER CONSTANTS **********/
const float PLAYER_STARTING_X = 0.0f;
const float PLAYER_STARTING_Y = 0.0f;
const float PLAYER_STARTING_Z = 0.0f;
const float PLAYER_MAX_SPEED = 0.01f;
const float PLAYER_ACCELERATION = 0.00003f;
const float PLAYER_FRICTION = 0.000005f;
const float PLAYER_SIZE = 1.154700538f;
const float PLAYER_STARTING_ANGLE = 90.0;
const float PLAYER_STARTING_HEALTH = 1000.0f;
const float PLAYER_TURNING_SPEED = 0.6f;
const int PLAYER_UBER_HIT_TARGET = 10;
const int UBER_RESET_CONSTANT = 500;

/********** ENEMY CONSTANTS **********/
const float MIN_ENEMY_SIZE = PLAYER_SIZE * 0.8f;
const float MAX_ENEMY_SIZE = PLAYER_SIZE * 1.4f;
const float MIN_ENEMY_SPEED = PLAYER_MAX_SPEED * 0.8f;
const float MAX_ENEMY_SPEED = PLAYER_MAX_SPEED * 1.2f;
const float ENEMY_STARTING_HEALTH = PLAYER_STARTING_HEALTH * 0.4f;
const float ENEMY_SHOT_COOLDOWN = 300.0f;

/********** PROJECTILE CONSTANTS **********/
const float PROJECTILE_BASE_SPEED = 0.02f;
const float PROJECTILE_MAX_SPEED = 0.02f;
const float PROJECTILE_ACCELERATION = 0.0f;
const float PROJECTILE_SIZE = 0.12f;
const float PROJECTILE_FRICTION = 0.0f;
const float PROJECTILE_STRENGTH = 100.0f;

/********** MISC CONSTANTS **********/
const float PI = 3.141592654f;
const float TOP_ANGLE = 30.0f;  //angle formed between the diagonal of the car and the front
const float FRONT_ANGLE = 30.0f;
const float CRASH_DAMAGE = 10.0f;
const float FLARE_SHOWN_CONSTANT = 50.0f;
const float SCREEN_WIDTH = 1366;
const float SCREEN_HEIGHT = 780;
const float COUNTER_WIDTH = 1.0f;;
const float COUNTER_HEIGHT = 0.5f;
const float PICKUP_SIZE = 0.5f;
const int ITEMS_NEEDED = 7;
const float MAP_WIDTH = 30.0f;
const float MAP_HEIGHT = 20.0f;

#endif
