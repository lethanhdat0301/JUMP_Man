#include"Dot.h"

Dot::Dot()
{
    //Initialize the offsets
    mPosX = 0;
    mPosY = 0;

    //Set collision box dimension
	mCollider.w = 30;
	mCollider.h = 92;

    //Initialize the velocity
    mVelX = 0;
    mVelY = 0;
}

void Dot::handleEvent( SDL_Event& e ) {
    //If a key was pressed
	if( e.type == SDL_KEYDOWN && e.key.repeat == 0 )
        {
        //Adjust the velocity
        if( e.key.keysym.sym == SDLK_UP){

                if(isJumping==false){
                isJumping = true;
                mVelY = DOT_VEL;
                ay= DOT_VEL / 15;
                }
        }
    }
}

void Dot::move() {
    //Move the dot up or down
    mPosY -= mVelY;
    mVelY -= ay;

    //If the dot went too far up or down
    if( ( mPosY < 0 ) || ( mPosY + mDotTexture->getHeight() > SCREEN_HEIGHT ) )
        {
        //Move back
        mPosY -= mVelY;
        }
     if(mPosY > 187){
                    mVelY=0;
                    ay =0;
                    isJumping = false;
                    mPosY=187;
                    }
    if(mPosY < 187) {
        isJumping=true;
    }
    mCollider.x = mPosX;
    mCollider.y = mPosY;
}

void Dot::render(){
    //Show the dot
	gDotTexture.render( mPosX, mPosY,&mFrame);
}
void Dot::SetDefaultFrame(int x, int y, double w, double h){
    mFrame.x=x;
    mFrame.y=y;
    mFrame.w=w;
    mFrame.h=h;
}

void Dot::SetFrame(int frameX,int frameY, int speed){
    mFrame.x=mFrame.w * (int)(frameX / speed);
    mFrame.y=mFrame.h *(int)(frameY);
    //Làm chậm
}
SDL_Rect Dot::GetFrame(){
    return mFrame;
}

SDL_Rect Dot::getCollider(){
    return mCollider;
}

void Dot::SetDimension(int x, int y) {
    mPosX=x;
    mPosY=y;
}
