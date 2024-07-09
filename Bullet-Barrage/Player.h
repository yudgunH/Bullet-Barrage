#ifndef PLAYER_H
#define PLAYER_H

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <vector>

enum Direction {
    LEFT,
    RIGHT
};

enum State {
    IDLE,
    RUNNING,
    JUMPING
};

class Player {
public:
    Player(SDL_Renderer* renderer, const std::string& base_path);
    ~Player();
    void handleEvent(SDL_Event& e);
    void move();
    void render(SDL_Renderer* renderer);

private:
    std::vector<SDL_Texture*> idleLeftTextures;
    std::vector<SDL_Texture*> idleRightTextures;
    std::vector<SDL_Texture*> runLeftTextures;
    std::vector<SDL_Texture*> runRightTextures;
    std::vector<SDL_Texture*> jumpLeftTextures;
    std::vector<SDL_Texture*> jumpRightTextures;

    int posX, posY;
    float velY;
    int idleWidth, idleHeight;
    int runWidth, runHeight;
    int jumpWidth, jumpHeight;
    int frame;
    int animationSpeed; // Number of milliseconds to wait before switching to the next animation frame
    Uint32 lastFrameTime;
    Direction direction;
    State state;
    bool onGround;
    int gravity;
    int maxFallSpeed;
    int jumpForce;
    int playerStep;
    Uint32 lastStepTime;
    int stepDelay;
    bool moveLeft; // New
    bool moveRight; // New
    Uint32 lastMoveTime; // New

    void loadTextures(SDL_Renderer* renderer, const std::string& path, std::vector<SDL_Texture*>& textures, int frameCount);
};

#endif // PLAYER_H
