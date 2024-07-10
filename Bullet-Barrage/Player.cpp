#include "Player.h"
#include <iostream>

Player::Player(SDL_Renderer* renderer, const std::string& base_path)
    : posX(600), posY(540), velY(0), frame(0), animationSpeed(100), lastFrameTime(0), direction(RIGHT), state(IDLE), onGround(true), gravity(0.5), maxFallSpeed(5), jumpForce(10), playerStep(5), lastStepTime(0), stepDelay(10), moveLeft(false), moveRight(false), lastMoveTime(0) {
    // Load all textures for different states and directions
    loadTextures(renderer, base_path + "/idle_L/Character_1-idle_", idleLeftTextures, 31);
    loadTextures(renderer, base_path + "/idle_R/Character_1-idle_", idleRightTextures, 31);
    loadTextures(renderer, base_path + "/Run_L/Character_1-Run_", runLeftTextures, 27);
    loadTextures(renderer, base_path + "/Run_R/Character_1-Run_", runRightTextures, 27);
    loadTextures(renderer, base_path + "/Jump_L/Character_1-jump_", jumpLeftTextures, 14);
    loadTextures(renderer, base_path + "/Jump_R/Character_1-jump_", jumpRightTextures, 14);

    // Calculate the scaling factor for different animations
    float scaleFactor = 0.35; // Adjust this factor as needed

    // Set the new width and height based on the scaling factor
    idleWidth = static_cast<int>(195 * scaleFactor);
    idleHeight = static_cast<int>(385 * scaleFactor);
    runWidth = static_cast<int>(327 * scaleFactor);
    runHeight = static_cast<int>(381 * scaleFactor);
    jumpWidth = static_cast<int>(361 * scaleFactor);
    jumpHeight = static_cast<int>(421 * scaleFactor);
}

Player::~Player() {
    for (auto texture : idleLeftTextures) SDL_DestroyTexture(texture);
    for (auto texture : idleRightTextures) SDL_DestroyTexture(texture);
    for (auto texture : runLeftTextures) SDL_DestroyTexture(texture);
    for (auto texture : runRightTextures) SDL_DestroyTexture(texture);
    for (auto texture : jumpLeftTextures) SDL_DestroyTexture(texture);
    for (auto texture : jumpRightTextures) SDL_DestroyTexture(texture);
}

void Player::loadTextures(SDL_Renderer* renderer, const std::string& path, std::vector<SDL_Texture*>& textures, int frameCount) {
    for (int i = 0; i < frameCount; ++i) {
        std::string filePath = path + (i < 10 ? "0" : "") + std::to_string(i) + ".png"; // Handles 00-09 naming
        SDL_Surface* loadedSurface = IMG_Load(filePath.c_str());
        if (loadedSurface == NULL) {
            std::cerr << "Unable to load image " << filePath << "! SDL_image Error: " << IMG_GetError() << std::endl;
        }
        else {
            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
            if (texture == NULL) {
                std::cerr << "Unable to create texture from " << filePath << "! SDL_Error: " << SDL_GetError() << std::endl;
            }
            else {
                textures.push_back(texture);
            }
            SDL_FreeSurface(loadedSurface);
        }
    }
}

void Player::handleEvent(SDL_Event& e) {
    if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
        switch (e.key.keysym.sym) {
        case SDLK_UP:
            if (onGround) {
                velY = -jumpForce;
                onGround = false;
                state = JUMPING;
            }
            break;
        case SDLK_LEFT:
            moveLeft = true;
            direction = LEFT;
            state = RUNNING;
            break;
        case SDLK_RIGHT:
            moveRight = true;
            direction = RIGHT;
            state = RUNNING;
            break;
        }
    }
    else if (e.type == SDL_KEYUP && e.key.repeat == 0) {
        switch (e.key.keysym.sym) {
        case SDLK_UP:
            if (velY < 0) velY = 0;
            break;
        case SDLK_LEFT:
            moveLeft = false;
            if (!moveRight) state = IDLE;
            break;
        case SDLK_RIGHT:
            moveRight = false;
            if (!moveLeft) state = IDLE;
            break;
        }
    }
}

void Player::move() {
    Uint32 currentTime = SDL_GetTicks();

    if (currentTime > lastMoveTime + stepDelay) {
        if (moveLeft) {
            posX -= playerStep;
        }
        if (moveRight) {
            posX += playerStep;
        }
        lastMoveTime = currentTime;
    }

    velY += gravity;
    if (velY > maxFallSpeed) velY = maxFallSpeed;

    posY += velY;

    // Clamp the position to the screen bounds
    if (posX < 0) posX = 0;
    if (posX + runWidth > 1881) posX = 1881 - runWidth; // Adjusted for new screen width
    if (posY < 0) posY = 0;
    if (posY + jumpHeight > 918) { // Adjusted for new screen height
        posY = 918 - jumpHeight;
        onGround = true;
        velY = 0;
        state = IDLE;
    }
}

void Player::render(SDL_Renderer* renderer) {
    Uint32 currentTime = SDL_GetTicks();
    if (currentTime > lastFrameTime + animationSpeed) {
        frame = (frame + 1) % 31;
        lastFrameTime = currentTime;
    }

    std::vector<SDL_Texture*>* currentTextures = nullptr;
    int renderWidth = 0;
    int renderHeight = 0;

    if (state == IDLE) {
        currentTextures = (direction == LEFT) ? &idleLeftTextures : &idleRightTextures;
        renderWidth = idleWidth;
        renderHeight = idleHeight;
    }
    else if (state == RUNNING) {
        currentTextures = (direction == LEFT) ? &runLeftTextures : &runRightTextures;
        renderWidth = runWidth;
        renderHeight = runHeight;
    }
    else if (state == JUMPING) {
        currentTextures = (direction == LEFT) ? &jumpLeftTextures : &jumpRightTextures;
        renderWidth = jumpWidth;
        renderHeight = jumpHeight;
    }

    if (currentTextures && !currentTextures->empty()) {
        SDL_Rect renderQuad = { posX, posY, renderWidth, renderHeight };
        SDL_RenderCopy(renderer, (*currentTextures)[frame % currentTextures->size()], NULL, &renderQuad);
    }
}
