#include "master.h"

class Ball {

    public: 
    float width, height, speed;
    float ballVx, ballVy;
    SDL_FRect ballPos;

    Ball(float x, float y, float w, float h, float s)
     {
        ballPos = SDL_FRect{x, y, w, h};
        width = w;
        height = h;
        speed = s;
        ballVx = speed;
        ballVy = speed;

       
    }

    void moveBall(float deltaTime) {
        ballPos.x += ballVx * deltaTime;
        ballPos.y += ballVy * deltaTime;
    }

    void deflectBall(float angle, bool isPlayer1) {
        ballVx = speed * cos(angle) * ((isPlayer1) ? 1 : -1);
        ballVy = speed * sin(angle);
    }

};