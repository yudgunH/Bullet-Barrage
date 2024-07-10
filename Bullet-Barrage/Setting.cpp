#include "Setting.h"
#include <iostream>
#include <SDL_image.h>
#include <SDL_mixer.h> // Include SDL_mixer

Setting::Setting(SDL_Renderer* renderer) : dragging(false), currentTrack(0) {
    if (TTF_Init() == -1) {
        std::cerr << "TTF_Init: " << TTF_GetError() << std::endl;
        exit(1);
    }

    font = TTF_OpenFont("../assets/fonts/dlxfont_.ttf", 24); // Font file path
    if (font == NULL) {
        std::cerr << "Failed to load font! TTF_Error: " << TTF_GetError() << std::endl;
        exit(1);
    }

    textColor = { 255, 255, 255 };
    hoverColor = { 0, 0, 0, 255 };

    // Load background texture for setting
    SDL_Surface* bgSurface = IMG_Load("../assets/img/Setting.png");
    if (bgSurface == NULL) {
        std::cerr << "Unable to load background image! SDL_image Error: " << IMG_GetError() << std::endl;
        exit(1);
    }
    else {
        backgroundTexture = SDL_CreateTextureFromSurface(renderer, bgSurface);
        SDL_FreeSurface(bgSurface);
    }

    // Initialize slider
    sliderRect = { 1000, 380, 200, 20 }; // Position and size of the slider
    sliderHandleRect = { 1000, 370, 20, 40 }; // Position and size of the slider handle

    // Initialize track buttons
    prevButtonRect = { 1000 - 50, 540, 40, 40 }; // Position and size of the previous button
    nextButtonRect = { 1200 + 10, 540, 40, 40 }; // Position and size of the next button

    // Initialize track names
    trackNames.push_back("Swift Shooters");
    // Add more tracks if needed

    // Initialize SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
        exit(1);
    }

    // Load music
    music = Mix_LoadMUS("../assets/audio/Soundtrack - Swift Shooters.mp3");
    if (music == NULL) {
        std::cerr << "Failed to load music! SDL_mixer Error: " << Mix_GetError() << std::endl;
        exit(1);
    }

    // Play music
    Mix_PlayMusic(music, -1);
}

Setting::~Setting() {
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyTexture(backgroundTexture);
    Mix_FreeMusic(music);
    Mix_CloseAudio();
}

void Setting::handleEvent(SDL_Event& e, bool& quit, int& volume, int& currentTrack) {
    if (e.type == SDL_QUIT) {
        quit = true;
    }
    else if (e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP) {
        int x, y;
        SDL_GetMouseState(&x, &y);
        handleSliderEvent(e, volume);
        handleButtonEvent(e, x, y, currentTrack);
    }
}

void Setting::handleSliderEvent(SDL_Event& e, int& volume) {
    int x, y;
    SDL_GetMouseState(&x, &y);

    if (e.type == SDL_MOUSEBUTTONDOWN) {
        if (x > sliderHandleRect.x && x < sliderHandleRect.x + sliderHandleRect.w &&
            y > sliderHandleRect.y && y < sliderHandleRect.y + sliderHandleRect.h) {
            dragging = true;
        }
    }
    else if (e.type == SDL_MOUSEBUTTONUP) {
        dragging = false;
    }

    if (dragging) {
        sliderHandleRect.x = x - sliderHandleRect.w / 2;

        if (sliderHandleRect.x < sliderRect.x) {
            sliderHandleRect.x = sliderRect.x;
        }
        if (sliderHandleRect.x > sliderRect.x + sliderRect.w - sliderHandleRect.w) {
            sliderHandleRect.x = sliderRect.x + sliderRect.w - sliderHandleRect.w;
        }

        volume = (sliderHandleRect.x - sliderRect.x) * 100 / (sliderRect.w - sliderHandleRect.w); // Assuming volume is in percentage
        Mix_VolumeMusic(volume);
    }
}

void Setting::handleButtonEvent(SDL_Event& e, int x, int y, int& currentTrack) {
    if (e.type == SDL_MOUSEBUTTONDOWN) {
        if (x > prevButtonRect.x && x < prevButtonRect.x + prevButtonRect.w &&
            y > prevButtonRect.y && y < prevButtonRect.y + prevButtonRect.h) {
            currentTrack = (currentTrack - 1 + trackNames.size()) % trackNames.size();
            // Handle music change logic
        }
        if (x > nextButtonRect.x && x < nextButtonRect.x + nextButtonRect.w &&
            y > nextButtonRect.y && y < nextButtonRect.y + nextButtonRect.h) {
            currentTrack = (currentTrack + 1) % trackNames.size();
            // Handle music change logic
        }
    }
}

void Setting::render(SDL_Renderer* renderer) {
    SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);

    // Render slider
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black color for slider
    SDL_RenderFillRect(renderer, &sliderRect);

    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Green color for slider handle
    SDL_RenderFillRect(renderer, &sliderHandleRect);

    // Render track buttons
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black color for track buttons
    SDL_RenderFillRect(renderer, &prevButtonRect);
    SDL_RenderFillRect(renderer, &nextButtonRect);

    // Render current track name
    std::string trackName = trackNames[currentTrack];
    SDL_Rect trackNameRect = { 1050, 540, 100, 40 }; // Position of the track name
    SDL_Texture* trackNameTexture = createTextTexture(renderer, trackName, textColor);
    SDL_RenderCopy(renderer, trackNameTexture, NULL, &trackNameRect);
    SDL_DestroyTexture(trackNameTexture);
}

SDL_Texture* Setting::createTextTexture(SDL_Renderer* renderer, const std::string& text, SDL_Color color) {
    if (!font) {
        std::cerr << "Font not loaded correctly!" << std::endl;
        return NULL;
    }

    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), color);
    if (textSurface == NULL) {
        std::cerr << "Unable to render text surface! SDL_ttf Error: " << TTF_GetError() << std::endl;
        return NULL;
    }

    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (textTexture == NULL) {
        std::cerr << "Unable to create texture from rendered text! SDL Error: " << SDL_GetError() << std::endl;
    }

    SDL_FreeSurface(textSurface);

    return textTexture;
}
