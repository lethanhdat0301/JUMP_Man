#ifndef OBJECT_H_INCLUDED
#define OBJECT_H_INCLUDED

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
#include"SDL2_Utils.h"
#include"Constant.h"
class Object
{
    private:
	int mSpriteWidth, mSpriteHeight;

    public:
	Object();

	Object(SDL_Renderer* renderer, std::string path, int w, int h);


    void SetDimension(int x, int y);

    void move();

    int GetWidth();
	int GetHeight();

    int mVobX;

    int mPosX,mPosY;

	void render();

	SDL_Rect getCollider();

	SDL_Rect mCollider;

};

//Key press surfaces constants
enum KeyPressSurfaces
{
	KEY_PRESS_SURFACE_DEFAULT,
	KEY_PRESS_SURFACE_UP,
	KEY_PRESS_SURFACE_DOWN,
	KEY_PRESS_SURFACE_LEFT,
	KEY_PRESS_SURFACE_RIGHT,
	KEY_PRESS_SURFACE_TOTAL
};



#endif // OBJECT_H_INCLUDED
