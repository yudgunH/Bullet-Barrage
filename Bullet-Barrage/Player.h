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
    void loadTextures(SDL_Renderer* renderer, const std::string& path, std::vector<SDL_Texture*>& textures, int frameCount);

    enum Direction { LEFT, RIGHT };
    enum State { IDLE, RUNNING, JUMPING };

    int posX, posY;
    int velY;
    int frame;
    int animationSpeed;
    Uint32 lastFrameTime;
    Direction direction;
    State state;
    bool onGround;
    bool canDoubleJump;
    bool reachedPeak;
    int jumpStartY;
    int jumpTargetY;
    int groundY;
    int jumpForce;
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
    int idleWidth, idleHeight;
    int runWidth, runHeight;
    int jumpWidth, jumpHeight;

    std::vector<SDL_Texture*> idleLeftTextures;
    std::vector<SDL_Texture*> idleRightTextures;
    std::vector<SDL_Texture*> runLeftTextures;
    std::vector<SDL_Texture*> runRightTextures;
    std::vector<SDL_Texture*> jumpLeftTextures;
    std::vector<SDL_Texture*> jumpRightTextures;
};

#endif // PLAYER_H
