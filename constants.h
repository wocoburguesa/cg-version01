#include "utils.h"

#define POINT pair<float, float>
#define INF 0xffffffff

using namespace std;

#ifndef CONSTANTS_H
#define CONSTANTS_H

/********** PLAYER CONSTANTS **********/
const float PLAYER_STARTING_X = 21.213204f;
const float PLAYER_STARTING_Y = 21.213204f;
const float PLAYER_HEIGHT = 1.8f;
const float PLAYER_MAX_SPEED = 0.01f;
const float PLAYER_ACCELERATION = 0.00003f;
const float PLAYER_FRICTION = 0.000005f;
const float PLAYER_SIZE = 1.154700538f;
const float PLAYER_STARTING_ANGLE = 45.0;
const float PLAYER_STARTING_HEALTH = 1000.0f;
const float PLAYER_TURNING_SPEED = 0.6f;
const int PLAYER_UBER_HIT_TARGET = 10;
const int UBER_RESET_CONSTANT = 500;

/********** ENEMY CONSTANTS **********/
const float MIN_ENEMY_SIZE = PLAYER_SIZE * 0.8f;
const float MAX_ENEMY_SIZE = PLAYER_SIZE * 1.4f;
const float MIN_ENEMY_SPEED = PLAYER_MAX_SPEED * 0.8f;
const float MAX_ENEMY_SPEED = PLAYER_MAX_SPEED * 1.2f;
const float MIN_ENEMY_HEIGHT = PLAYER_HEIGHT * 0.8f;
const float MAX_ENEMY_HEIGHT = PLAYER_HEIGHT * 1.2f;
const float ENEMY_STARTING_HEALTH = PLAYER_STARTING_HEALTH * 0.4f;
const float ENEMY_SHOT_COOLDOWN = 300.0f;

/********** PROJECTILE CONSTANTS **********/
const float PROJECTILE_BASE_SPEED = 0.02f;
const float PROJECTILE_MAX_SPEED = 0.02f;
const float PROJECTILE_ACCELERATION = 0.0f;
const float PROJECTILE_SIZE = 0.12f;
const float PROJECTILE_HEIGHT = PROJECTILE_SIZE * 0.6f;
const float PROJECTILE_FRICTION = 0.0f;
const float PROJECTILE_STRENGTH = 100.0f;

/********** MISC CONSTANTS **********/
const float MIN_BUILDING_HEIGHT = PLAYER_HEIGHT * 3.0f;
const float MAX_BUILDING_HEIGHT = PLAYER_HEIGHT * 4.0f;
const float PI = 3.141592654f;
const float TH = 30.0f;  //angle formed between the diagonal of the car and the front
const float CRASH_DAMAGE = 10.0f;
const float FLARE_SHOWN_CONSTANT = 50.0f;
const float SCREEN_WIDTH = 1366;
const float SCREEN_HEIGHT = 780;
const float COUNTER_WIDTH = 1.0f;;
const float COUNTER_HEIGHT = 0.5f;
const int ITEMS_NEEDED = 7;
const float MAP_WIDTH = 30.0f;
const float MAP_HEIGHT = 20.0f;
const float GROUND_LEVEL = 0.0f;
const float BOUND_HEIGHT = 6.0f;

/********** CAMERA CONSTANTS **********/
const float UP_X = 0.0f;
const float UP_Y = 0.5f;
const float UP_Z = 0.2f;
const float CENTER_DIFF_X = 0.0f;
const float DEFAULT_CAMERA_DISTANCE = -10.0f;
const float MIN_CAMERA_DISTANCE = -4.0f;
const float MAX_CAMERA_DISTANCE = -15.0f;
const float CAMERA_HEIGHT = 10.0f;
const float CAMERA_MAX_SPEED = 0.1f;
const float CAMERA_ACCELERATION = 0.0003f;
const float CAMERA_FRICTION = 0.000005f;
//camera plane is upside-down
const float CAMERA_STARTING_ANGLE = 45.0f;
const float CAMERA_TURNING_SPEED = 0.6f;
const float CAMERA_RESET_TIME = 500.0f;

/********** PICKUP CONSTANTS **********/
const float PICKUP_SIZE = 0.5f;
const float PICKUP_HEIGHT = 1.0f;

#endif
