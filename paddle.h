#include "master.h"

class Paddle {
    public: 
    float width, height, screen_width, screen_height;
    bool isPlayer1;
    SDL_FRect player;

    Paddle(float x, float y, const float w, const float h, float sw, float sh, bool p1)
        : screen_width(sw) , screen_height(sh), isPlayer1(p1)
     {
        player = SDL_FRect{x, y, w, h};
        width = w;
        height = h;

    }

    float getCenter() {
        return player.y + (height / 2.0f);
    }

    void getScreenDimensions(float sw, float sh) {
        screen_width = sw;
        screen_height = sh;
    }

    void movePadde(float paddleSpeed, float dt, const bool* keys) {
        if (isPlayer1) {
            if (keys[SDL_SCANCODE_W] && player.y > 0) {
            player.y -= paddleSpeed *dt;
        }

        if (keys[SDL_SCANCODE_S]&& player.y < screen_height - height) {
            player.y += paddleSpeed *dt;
        }
        } else {
            if (keys[SDL_SCANCODE_UP] && player.y > 0) {
                player.y -= paddleSpeed *dt;
            }

            if (keys[SDL_SCANCODE_DOWN]&& player.y < screen_height - height) {
                player.y += paddleSpeed *dt;
            }
        }
    }




};