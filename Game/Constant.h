#ifndef CONSTANT_H_INCLUDED
#define CONSTANT_H_INCLUDED

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <random>
#include <time.h>
#include <SDL_mixer.h>
#include"LTexture.h"
#include"Dot.h"
#include"Object.h"
#include"SDL2_Utils.h"

//Screen dimension constants
const int SCREEN_WIDTH = 900;
const int SCREEN_HEIGHT = 340;

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

TTF_Font *gFont = NULL;

//The music that will be played
Mix_Music *gMusic = NULL;


#endif // CONSTANT_H_INCLUDED
