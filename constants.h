#define POINT pair<float, float>
#define INF 0xffffffff

using namespace std;

#ifndef CONSTANTS_H
#define CONSTANTS_H

/********** PLAYER CONSTANTS **********/
const float PLAYER_ORIGINAL_X = 0.0f;
const float PLAYER_ORIGINAL_Y = 0.0f;
const float PLAYER_MAX_SPEED = 0.1f;
const float PLAYER_ACCELERATION = 0.0000001f;
const float PLAYER_FRICTION = 0.00000005f;
const float PLAYER_SIZE = 1.154700538f;
const float PLAYER_ORIGINAL_ANGLE = 90.0;

/********** ENEMY CONSTANTS **********/
const float MIN_ENEMY_SIZE = PLAYER_SIZE * 0.8f;
const float MAX_ENEMY_SIZE = PLAYER_SIZE * 1.4f;
const float MIN_ENEMY_SPEED = PLAYER_MAX_SPEED * 0.8f;
const float MAX_ENEMY_SPEED = PLAYER_MAX_SPEED * 1.2f;

/********** PROJECTILE CONSTANTS **********/
const float PROJECTILE_BASE_SPEED = 0.2f;
const float PROJECTILE_MAX_SPEED = 0.2f;
const float PROJECTILE_ACCELERATION = 0.0f;
const float PROJECTILE_SIZE = 0.1f;
const float PROJECTILE_FRICTION = 0.0f;

/********** MISC CONSTANTS **********/
const float PI = 3.141592654f;
const float TH = 30.0f;  //angle formed between the diagonal of the car and the front

#endif
