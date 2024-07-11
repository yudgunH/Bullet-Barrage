#include "Setting.h"
#include <iostream>
#include <SDL_image.h>
#include <SDL_mixer.h>

Setting::Setting(SDL_Renderer* renderer)
    : dragging(false), sliderHandleHover(false), leftButtonHover(false), rightButtonHover(false), currentTrack(0) {

    if (TTF_Init() == -1) {
        std::cerr << "TTF_Init: " << TTF_GetError() << std::endl;
        exit(1);
    }

    font = TTF_OpenFont("../assets/fonts/dlxfont_.ttf", 24); // Font file path
    if (font == NULL) {
        std::cerr << "Failed to load font! TTF_Error: " << TTF_GetError() << std::endl;
        exit(1);
    }

    // Set the text color to black
    textColor = { 0, 0, 0 };
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

    // Load slider frame texture
    SDL_Surface* sliderFrameSurface = IMG_Load("../assets/img/UI/WideSlider_WhiteOutline_Frame.png");
    sliderFrameTexture = SDL_CreateTextureFromSurface(renderer, sliderFrameSurface);
    SDL_FreeSurface(sliderFrameSurface);

    // Load new slider handle texture and hover texture
    SDL_Surface* sliderSurface = IMG_Load("../assets/img/UI/WideSlider_Scroller.png");
    sliderHandleTexture = SDL_CreateTextureFromSurface(renderer, sliderSurface);
    SDL_FreeSurface(sliderSurface);

    SDL_Surface* sliderHoverSurface = IMG_Load("../assets/img/UI/WideSlider_Blank_Scroller.png");
    sliderHandleHoverTexture = SDL_CreateTextureFromSurface(renderer, sliderHoverSurface);
    SDL_FreeSurface(sliderHoverSurface);

    // Load new track button textures and hover textures
    SDL_Surface* leftButtonSurface = IMG_Load("../assets/img/UI/ScrollSlider_Arrow_Left.png");
    leftButtonTexture = SDL_CreateTextureFromSurface(renderer, leftButtonSurface);
    SDL_FreeSurface(leftButtonSurface);

    SDL_Surface* leftButtonHoverSurface = IMG_Load("../assets/img/UI/ScrollSlider_Blank_Arrow_Left.png");
    leftButtonHoverTexture = SDL_CreateTextureFromSurface(renderer, leftButtonHoverSurface);
    SDL_FreeSurface(leftButtonHoverSurface);

    SDL_Surface* rightButtonSurface = IMG_Load("../assets/img/UI/ScrollSlider_Arrow_Right.png");
    rightButtonTexture = SDL_CreateTextureFromSurface(renderer, rightButtonSurface);
    SDL_FreeSurface(rightButtonSurface);

    SDL_Surface* rightButtonHoverSurface = IMG_Load("../assets/img/UI/ScrollSlider_Blank_Arrow_Right.png");
    rightButtonHoverTexture = SDL_CreateTextureFromSurface(renderer, rightButtonHoverSurface);
    SDL_FreeSurface(rightButtonHoverSurface);

    // Initialize slider frame and slider
    sliderFrameRect = { 1000, 380, 200, 40 }; // Position and size of the slider frame
    sliderRect = { 1000, 390, 200, 20 }; // Position and size of the slider
    sliderHandleRect = { 1200, 370, 20, 40 }; // Position and size of the slider handle (100% volume at far right)

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
    SDL_DestroyTexture(sliderFrameTexture);
    SDL_DestroyTexture(sliderHandleTexture);
    SDL_DestroyTexture(sliderHandleHoverTexture);
    SDL_DestroyTexture(leftButtonTexture);
    SDL_DestroyTexture(leftButtonHoverTexture);
    SDL_DestroyTexture(rightButtonTexture);
    SDL_DestroyTexture(rightButtonHoverTexture);
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

        // Check if mouse is over slider handle
        sliderHandleHover = (x > sliderHandleRect.x && x < sliderHandleRect.x + sliderHandleRect.w &&
            y > sliderHandleRect.y && y < sliderHandleRect.y + sliderHandleRect.h);

        // Check if mouse is over track buttons
        leftButtonHover = (x > prevButtonRect.x && x < prevButtonRect.x + prevButtonRect.w &&
            y > prevButtonRect.y && y < prevButtonRect.y + prevButtonRect.h);
        rightButtonHover = (x > nextButtonRect.x && x < nextButtonRect.x + nextButtonRect.w &&
            y > nextButtonRect.y && y < nextButtonRect.y + nextButtonRect.h);

        // Change cursor to hand if hovering over slider handle or buttons
        if (sliderHandleHover || leftButtonHover || rightButtonHover) {
            SDL_SetCursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND));
        }
        else {
            SDL_SetCursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW));
        }
    }
}

void Setting::handleSliderEvent(SDL_Event& e, int& volume) {
    int x, y;
    SDL_GetMouseState(&x, &y);

    if (e.type == SDL_MOUSEBUTTONDOWN) {
        if (sliderHandleHover) {
            dragging = true;
        }
    }
    else if (e.type == SDL_MOUSEBUTTONUP) {
        dragging = false;
    }

    if (dragging) {
        sliderHandleRect.x = x - sliderHandleRect.w / 2;

        if (sliderHandleRect.x < sliderFrameRect.x) {
            sliderHandleRect.x = sliderFrameRect.x;
        }
        if (sliderHandleRect.x > sliderFrameRect.x + sliderFrameRect.w - sliderHandleRect.w) {
            sliderHandleRect.x = sliderFrameRect.x + sliderFrameRect.w - sliderHandleRect.w;
        }

        volume = (sliderHandleRect.x - sliderFrameRect.x) * 100 / (sliderFrameRect.w - sliderHandleRect.w); // Assuming volume is in percentage
        Mix_VolumeMusic(volume);
    }
}

void Setting::handleButtonEvent(SDL_Event& e, int x, int y, int& currentTrack) {
    if (e.type == SDL_MOUSEBUTTONDOWN) {
        if (leftButtonHover) {
            currentTrack = (currentTrack - 1 + trackNames.size()) % trackNames.size();
            // Handle music change logic
        }
        if (rightButtonHover) {
            currentTrack = (currentTrack + 1) % trackNames.size();
            // Handle music change logic
        }
    }
}

void Setting::render(SDL_Renderer* renderer) {
    SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);

    // Render slider (filled part of the slider bar)
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black color for slider
    sliderRect.w = sliderHandleRect.x + sliderHandleRect.w / 2 - sliderRect.x; // Adjust slider length based on handle position
    SDL_RenderFillRect(renderer, &sliderRect);

    // Render slider frame on top of slider
    SDL_RenderCopy(renderer, sliderFrameTexture, NULL, &sliderFrameRect);

    // Render slider handle with hover effect
    if (sliderHandleHover) {
        SDL_RenderCopy(renderer, sliderHandleHoverTexture, NULL, &sliderHandleRect);
    }
    else {
        SDL_RenderCopy(renderer, sliderHandleTexture, NULL, &sliderHandleRect);
    }

    // Render track buttons with hover effect
    if (leftButtonHover) {
        SDL_RenderCopy(renderer, leftButtonHoverTexture, NULL, &prevButtonRect);
    }
    else {
        SDL_RenderCopy(renderer, leftButtonTexture, NULL, &prevButtonRect);
    }

    if (rightButtonHover) {
        SDL_RenderCopy(renderer, rightButtonHoverTexture, NULL, &nextButtonRect);
    }
    else {
        SDL_RenderCopy(renderer, rightButtonTexture, NULL, &nextButtonRect);
    }

    // Render current track name
    std::string trackName = trackNames[currentTrack];
    SDL_Rect trackNameRect = { 1050, 540, 100, 40 }; // Position of the track name
    SDL_Texture* trackNameTexture = createTextTexture(renderer, trackName, textColor);
    SDL_RenderCopy(renderer, trackNameTexture, NULL, &trackNameRect);
    SDL_DestroyTexture(trackNameTexture);

    // Render volume level
    std::string volumeText = std::to_string((sliderHandleRect.x - sliderFrameRect.x) * 100 / (sliderFrameRect.w - sliderHandleRect.w)) + "%";
    SDL_Texture* volumeTexture = createTextTexture(renderer, volumeText, textColor);
    SDL_Rect volumeRect = { sliderFrameRect.x + sliderFrameRect.w + 10, sliderFrameRect.y - 10, 50, 30 }; // Position of the volume text
    SDL_RenderCopy(renderer, volumeTexture, NULL, &volumeRect);
    SDL_DestroyTexture(volumeTexture); 
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
