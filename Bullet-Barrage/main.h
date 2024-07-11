#pragma once
#ifndef MAIN_H
#define MAIN_H

#include <SDL.h>
#include <iostream> // Thêm dòng này
#include "Menu.h"
#include "Setting.h"
#include "Player.h"
#include "Background.h"


enum Screen {
    MENU,
    GAME,
    SCORE,
    SETTING
};


#endif // MAIN_H
