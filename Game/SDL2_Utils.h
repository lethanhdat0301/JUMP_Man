#ifndef SDL2_UTILS_H_INCLUDED
#define SDL2_UTILS_H_INCLUDED

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
#include"Constant.h"

static int score =0;

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//Box collision detector
bool checkCollision( SDL_Rect a, SDL_Rect b );

#endif // SDL2_UTILS_H_INCLUDED
