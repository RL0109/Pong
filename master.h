#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <cmath>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifndef CONSTANTS
#define CONSTANTS

inline const int SCREEN_WIDTH = 800;
inline const int SCREEN_HEIGHT = 600;
inline const float PADDLE_SPEED = 300.0f;
inline const float BALL_SPEED = 500.0f; 

inline const float PADDLE_HEIGHT = 100.0f;
inline const float PADDLE_WIDTH = 10.0f;

inline const float EASY_DIFFICULTY_MODIFIER = 50.0f;
inline const float MEDIUM_DIFFICULTY_MODIFIER = 25.0f;
inline const float HARD_DIFFICULTY_MODIFIER = 10.0f;

#endif



