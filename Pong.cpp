#include "master.h"
#include "paddle.h"
#include "ball.h"

#include <SDL3_mixer/SDL_mixer.h>
#include <SDL3_ttf/SDL_ttf.h>

void RespawnBall(Ball& ball, float& timer) {
    ball.ballPos.x = SCREEN_WIDTH*0.5;
    ball.ballPos.y = SCREEN_HEIGHT*0.5;
    timer = 2.0f;
}

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    MIX_Init();
    TTF_Font* font = TTF_OpenFont("build/ARIAL.TTF", 64);
    if(!font) {
        std::cout << "Failed to load font: " << SDL_GetError() << std::endl;
        return -1;
    }
    if (!MIX_Init()) {
        std::cout << "Mixer Init Error: " << SDL_GetError() << std::endl;
        return -1;
    }
    MIX_Mixer* mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
    if (mixer == NULL) {
        std::cout << "Mixer Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    const char* audiopath = "Sounds/Hit_sound.wav";
    // Load audio from file
    MIX_Audio* mixedAudio = MIX_LoadAudio(mixer, audiopath, true);
    // Put audio on playback
    MIX_Track* mixedTrack = MIX_CreateTrack(mixer);

    SDL_Window* window = SDL_CreateWindow("PONG", SCREEN_WIDTH, SCREEN_HEIGHT, 0);

    SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);

    Paddle player1(5.0f, 300.0f, PADDLE_WIDTH, PADDLE_HEIGHT,SCREEN_WIDTH,SCREEN_HEIGHT, true);
    Paddle player2(SCREEN_WIDTH - 15.0f, 300.0f, PADDLE_WIDTH, PADDLE_HEIGHT, SCREEN_WIDTH , SCREEN_HEIGHT, false);
    Ball ball( 350.0f, 250.0f, 10.0f, 10.0f, BALL_SPEED);

    float roundDelayTimer = 2.0f;
    float AIDelayTimer = EASY_DIFFICULTY_MODIFIER;


    // stride is the amount of data per line object stored in the array
    int const stride = 4;
    int const totalLines = 30;

    // lines is the array holding the info of the each line
    float lines[totalLines * stride];

    SDL_FRect dashLines[totalLines];

    float yOffset = 5.0f;
    float gap = SCREEN_HEIGHT/(float)totalLines;

    for (int i = 0; i < totalLines; i ++) {
        dashLines[i].x = SCREEN_WIDTH/2;
        dashLines[i].y = yOffset;
        dashLines[i].w = 5;
        dashLines[i].h = 5;
        yOffset += gap;
    }

    SDL_Event event;

    // Need loop to finish before exiting.
    bool running = true;

    // Gets the time in order to get deltaTime in loop.
    Uint64 lastTime = SDL_GetTicks();

    //Get frame rate
    float fpsTimer = 0.0f;
    int frameCount = 0;
    bool fpsCheck = false;

    int player1Score = 0;
    int player2Score = 0;



    while (running) {

        std::string scoreText = std::to_string(player1Score) + "        " + std::to_string(player2Score);
        SDL_Color white = {255, 255, 255, 255};

        Uint64 now = SDL_GetTicks();
        float dt = (now - lastTime) / 1000.0f;
        lastTime = now;
        fpsTimer += dt;
        frameCount++;

        if (fpsTimer >= 1.0f) {
            if (fpsCheck) {
                std::cout << "FPS: " << frameCount << std::endl;
                fpsTimer = 0.0f;
                frameCount = 0;
            }
        }

        while (SDL_PollEvent(&event)) 
            if (event.type == SDL_EVENT_QUIT)
                running = false;

        const bool* keys = SDL_GetKeyboardState(NULL);

        if (roundDelayTimer > 0) {
            roundDelayTimer -= dt;
        }

        player1.movePadde(PADDLE_SPEED, dt, keys);
        player2.movePadde(PADDLE_SPEED, dt, keys);

        if (roundDelayTimer <= 0) {
            ball.moveBall(dt);
        }

        if (ball.ballPos.y > SCREEN_HEIGHT - 5.0f) {
            ball.ballPos.y = SCREEN_HEIGHT - 5.0f - 0.1f;
            ball.ballVy = -ball.ballVy;
        } 
        if (ball.ballPos.y < 0) {
            ball.ballPos.y = 0 + 0.1f;
            ball.ballVy  = -ball.ballVy;
        }

        if (ball.ballPos.x > SCREEN_WIDTH) {
            RespawnBall(ball, roundDelayTimer);
            player1Score++;
        }

        if  (ball.ballPos.x < 0) {
            RespawnBall(ball, roundDelayTimer);
            player2Score++;
        }

        if (player2.player.y - EASY_DIFFICULTY_MODIFIER < ball.ballPos.y) {
             player2.player.y += 1.0f;
        } 

        if (player2.player.y + EASY_DIFFICULTY_MODIFIER > ball.ballPos.y) {
                player2.player.y -= 1.0f;             
        }

        if (ball.ballPos.x > player1.player.x && ball.ballPos.x < player1.player.x + player1.width) {
            if (ball.ballPos.y > player1.player.y && ball.ballPos.y < player1.player.y + player1.height)
                {
                    MIX_PlayAudio(mixer, mixedAudio);
                    float intersect = (ball.ballPos.y + (ball.height /2)) - (player1.player.y + (player1.height /2.0f)); 
                    float relIntersect = intersect / (player1.height/2.0f);
                    float bounceAngle = relIntersect * (M_PI /4.0f);
                    ball.ballPos.x = player1.player.x + player1.width + 1.0f;
                    ball.deflectBall(bounceAngle, true);
                }    
        } 

        if (ball.ballPos.x < player2.player.x && ball.ballPos.x > player2.player.x - player2.width) {
            if (ball.ballPos.y > player2.player.y && ball.ballPos.y < player2.player.y + player2.height) 
                {
                    MIX_PlayAudio(mixer, mixedAudio);
                    float intersect = (ball.ballPos.y + (ball.height /2.0f)) - (player2.player.y + (player2.height /2.0f)); 
                    float relIntersect = intersect / (player2.height/2.0f);
                    float bounceAngle = relIntersect * (M_PI /4.0f);
                    ball.ballPos.x = player2.player.x - player2.width - 1.0f;
                    ball.deflectBall(bounceAngle, false);
                }
        } 

        // Draw a dark grey background
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_Surface* textSurface = TTF_RenderText_Solid(font, scoreText.c_str(), 0, white);
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_FRect scoreRect = { (SCREEN_WIDTH / 2.0F) - 100.0f, 20.0F, (float)textSurface->w, (float)textSurface->h};
        SDL_RenderTexture(renderer, textTexture, NULL, &scoreRect);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &player1.player);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &player2.player);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &ball.ballPos);

        for (int i = 0; i < totalLines; i ++) {
            
            SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
            SDL_RenderFillRect(renderer, &dashLines[i]);
        }

        SDL_RenderPresent(renderer);
        SDL_DestroySurface(textSurface);
        SDL_DestroyTexture(textTexture);

    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;

}
