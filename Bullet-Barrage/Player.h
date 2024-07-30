#ifndef PLAYER_H
#define PLAYER_H

#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <string>

class Player {
public:
    Player(SDL_Renderer* renderer, const std::string& base_path);
    ~Player();

    void handleEvent(SDL_Event& e);
    void move();
    void render(SDL_Renderer* renderer);
    void reset();

private:
    float posX;
    float posY;
    float velY;
    int frame;
    int animationSpeed;
    Uint32 lastFrameTime;
    enum Direction { LEFT, RIGHT } direction;
    enum State { IDLE, RUNNING, JUMPING } state;
    bool onGround;
    bool canDoubleJump;
    bool reachedPeak;
    float jumpStartY;
    float jumpTargetY;
    float groundY;
    float jumpForce;
    int playerStep;
    Uint32 lastStepTime;
    Uint32 stepDelay;
    Uint32 lastJumpTime;
    Uint32 jumpDelay;
    Uint32 peakDelay;
    Uint32 peakTime;
    bool moveLeft;
    bool moveRight;
    Uint32 lastMoveTime;

    std::vector<SDL_Texture*> idleLeftTextures;
    std::vector<SDL_Texture*> idleRightTextures;
    std::vector<SDL_Texture*> runLeftTextures;
    std::vector<SDL_Texture*> runRightTextures;
    std::vector<SDL_Texture*> jumpLeftTextures;
    std::vector<SDL_Texture*> jumpRightTextures;

    int idleWidth;
    int idleHeight;
    int runWidth;
    int runHeight;
    int jumpWidth;
    int jumpHeight;

    void loadTextures(SDL_Renderer* renderer, const std::string& path, std::vector<SDL_Texture*>& textures, int frameCount);
};

#endif // PLAYER_H
