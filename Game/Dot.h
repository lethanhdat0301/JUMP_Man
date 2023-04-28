#ifndef DOT_H_INCLUDED
#define DOT_H_INCLUDED

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <random>
#include <time.h>
#include <SDL_mixer.h>
#include "LTexture.h"


class Dot
{
    public:

		//Maximum axis velocity of the dot
		static const int DOT_VEL = 15;

		//Initializes the variables
		Dot();

		//Takes key presses and adjusts the dot's velocity
		void handleEvent( SDL_Event& e );

		//Moves the dot
		void move();

		//Shows the dot on the screen
		void render();

		void SetDimension(int x, int y);

        void SetDefaultFrame(int x, int y, double w, double h);

		void SetFrame(int frameX,int frameY,int speed);

		SDL_Rect GetFrame();

        LTexture* GetDotTexture() {
            return mDotTexture;
		};

		SDL_Rect getCollider();

		bool isJumping = false;

        int mPosX, mPosY;

        //Dot's collision box
        SDL_Rect mCollider;

    private:
		int mVelX, mVelY;

        int ay=0;

        SDL_Rect mFrame;

        LTexture* mDotTexture;

};

#endif // DOT_H_INCLUDED
