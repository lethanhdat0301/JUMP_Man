#include"Object.h"

Object::Object()
{
	mSpriteWidth = 0;
	mSpriteHeight = 0;
	mPosX =0;
	mPosY =0;
	//mVobX = 2;
}

Object :: Object(SDL_Renderer* renderer, std::string path, int w, int h){
    mSpriteHeight = h;
	mSpriteWidth = w;
	mCollider.w=w;
	mCollider.h=h;
	gObTexture.loadFromFile(path);
}

void Object :: render(){
	gObTexture.render(mPosX,mPosY,NULL);
}
void Object::move(){
    mPosX-=mVobX;
    if(mPosX < -100) {
        mPosX = SCREEN_WIDTH;
    }
    mCollider.x = mPosX;
    mCollider.y = mPosY;
}

void Object::SetDimension(int x, int y) {
    mPosX=x;
    mPosY=y;
    mCollider.x=x;
    mCollider.y=y;
}

int Object::GetWidth()
{
	return mSpriteWidth;
}

int Object::GetHeight()
{
	return mSpriteHeight;
}


SDL_Rect Object::getCollider(){
    return mCollider;
}
