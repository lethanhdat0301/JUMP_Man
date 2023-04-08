#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <random>

//Screen dimension constants
const int SCREEN_WIDTH = 900;
const int SCREEN_HEIGHT = 340;

//Texture wrapper class
class LTexture
{
	public:
		//Initializes variables
		LTexture();

		//Deallocates memory
		~LTexture();

		//Loads image at specified path
		bool loadFromFile( std::string path );

		//Deallocates texture
		void free();

		//Set color modulation
		void setColor( Uint8 red, Uint8 green, Uint8 blue );

		//Set blending
		void setBlendMode( SDL_BlendMode blending );

		//Set alpha modulation
		void setAlpha( Uint8 alpha );

		//Renders texture at given point
		void render( int x, int y, SDL_Rect* clip, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );


		//Gets image dimensions
		int getWidth();
		int getHeight();

	private:
		//The actual hardware texture
		SDL_Texture* mTexture;

		//Image dimensions
		int mWidth;
		int mHeight;
        int mSpriteWidth;
        int mSpriteHeight;

};

//The dot that will move around on the screen
class Dot
{
    public:
		//The dimensions of the dot
		static const int DOT_WIDTH = 0;
		static const int DOT_HEIGHT = 0;

		//Maximum axis velocity of the dot
		static const int DOT_VEL = 15;

		//Initializes the variables
		Dot();

		//Takes key presses and adjusts the dot's velocity
		void handleEvent( SDL_Event& e );

		//Moves the dot
		//void move();

		void move( SDL_Rect &Object );

		//Shows the dot on the screen
		void render();

        void SetDefaultFrame(int x, int y, int w, int h);

		void SetFrame(int frameX,int frameY,int speed);
		SDL_Rect GetFrame();
			LTexture GetDotTexture() {
            return mDotTexture;
		}
		bool isJumping = false;

    private:
		//The X and Y offsets of the dot
		int mPosX, mPosY;

		//The velocity of the dot
		int mVelX, mVelY;

        int ay=0;

        SDL_Rect mFrame;
        LTexture mDotTexture;
        //Dot's collision box
        SDL_Rect mCollider;
};
class Object
{
   private:
	int mSpriteWidth, mSpriteHeight;
    int mPosX,mPosY;
    int mVobX;
public:
	Object();

	Object(SDL_Renderer* renderer, std::string path, int w, int h);

    void SetDimension(int w, int h);

    void move();

    int GetWidth();
	int GetHeight();

	void render();

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

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//Box collision detector
bool checkCollision( SDL_Rect a, SDL_Rect b );

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Scene textures
LTexture gDotTexture;
LTexture gBGTexture;
LTexture gObTexture;

LTexture::LTexture()
{
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

LTexture::~LTexture()
{
	//Deallocate
	free();
}

bool LTexture::loadFromFile( std::string path )
{
	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
		//Color key image
		SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );

		//Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			mWidth = loadedSurface->w;
            mHeight = loadedSurface-> h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}

void LTexture::free()
{
	//Free texture if it exists
	if( mTexture != NULL )
	{
		SDL_DestroyTexture( mTexture );
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void LTexture::setColor( Uint8 red, Uint8 green, Uint8 blue )
{
	//Modulate texture rgb
	SDL_SetTextureColorMod( mTexture, red, green, blue );
}

void LTexture::setBlendMode( SDL_BlendMode blending )
{
	//Set blending function
	SDL_SetTextureBlendMode( mTexture, blending );
}

void LTexture::setAlpha( Uint8 alpha )
{
	//Modulate texture alpha
	SDL_SetTextureAlphaMod( mTexture, alpha );
}

void LTexture::render( int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip )
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	//Set clip rendering dimensions
	if( clip != NULL )
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopyEx( gRenderer, mTexture, clip, &renderQuad, angle, center, flip );
}
void Dot::SetDefaultFrame(int x, int y, int w, int h){
    mFrame.x=x;
    mFrame.y=y;
    mFrame.w=w;
    mFrame.h=h;
    /*mSpriteWidth = w;
	mSpriteHeight = h;*/
}

void Dot::SetFrame(int frameX,int frameY, int speed){
    mFrame.x=mFrame.w * (int)(frameX / speed);
    mFrame.y=mFrame.h *(int)(frameY/speed);
    //Làm chậm
}
SDL_Rect Dot::GetFrame(){
    return mFrame;
}

int LTexture::getWidth()
{
	return mWidth;
}

int LTexture::getHeight()
{
	return mHeight;
}


Dot::Dot()
{
    //Initialize the offsets
    mPosX = 25;
    mPosY = 182;

    //Set collision box dimension
	mCollider.w = DOT_WIDTH;
	mCollider.h = DOT_HEIGHT;

    //Initialize the velocity
    mVelX = 0;
    mVelY = 0;
}

void Dot::handleEvent( SDL_Event& e )
{
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

void Dot::move(SDL_Rect &Object)
{
    //Move the dot up or down
    mPosY -= mVelY;
    mVelY -= ay;
    mCollider.y = mPosY;

    //If the dot went too far up or down
    if( ( mPosY < 0 ) || ( mPosY + DOT_HEIGHT > SCREEN_HEIGHT ) )
    {
        //Move back
        mPosY -= mVelY;
        mCollider.y = mPosY;
    }

     if(mPosY > 182) {
                    mVelY=0;
                    ay =0;
                    isJumping = false;
                    mPosY=182;
                }
    if(mPosY < 182) {
        isJumping=true;
    }

}

void Dot::render()
{
    //Show the dot
	gDotTexture.render( mPosX, mPosY,&mFrame);
}

Object::Object()
{
	mSpriteWidth =0;
	mSpriteHeight = 0;
	mPosX =400;
	mPosY =170;
	mVobX = 2;
}

Object :: Object(SDL_Renderer* renderer, std::string path, int w, int h){
    mSpriteHeight = h;
	mSpriteWidth = w;
}

void Object :: render(){
    //Show the object
	gObTexture.render(mPosX,mPosY,NULL);
}
void Object::move(){
    mPosX-=mVobX;
    if(mPosX < -100) {
        srand(SDL_GetTicks());
        int k = rand()%2;
    if(k==1){
        gObTexture.loadFromFile("image/4.png");
    } else if(k==2){
        gObTexture.loadFromFile("image/5.png");
    }
        mPosX = SCREEN_WIDTH;
    }

}

/*void Object::SetDimension(int w, int h) {
	mPipeHigh->GetTexturedRectangle().SetDimension(w, h);
	mPipeLow->GetTexturedRectangle().SetDimension(w, h);
	mSpriteHeight = h;
	mSpriteWidth = w;
}*/

int Object::GetWidth()
{
	return mSpriteWidth;
}

int Object::GetHeight()
{
	return mSpriteHeight;
}


bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create vsynced renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
			}
		}
	}

	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load dot texture
	if( !gDotTexture.loadFromFile( "image/8.png" ) )
	{
		printf( "Failed to load dot texture!\n" );
		success = false;
	}

	//Load background texture
	if( !gBGTexture.loadFromFile( "image/bgr.jpg" ) )
	{
		printf( "Failed to load background texture!\n" );
		success = false;
	}

	//Load object texture
	if( !gObTexture.loadFromFile( "image/3.png" ) )
	{
		printf( "Failed to load object texture!\n" );
		success = false;
	}
	return success;
}

void close()
{
	//Free loaded images
	gDotTexture.free();
	gBGTexture.free();
    gObTexture.free();
	//Destroy window
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

bool checkCollision( SDL_Rect a, SDL_Rect b )
{
    //The sides of the rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    //Calculate the sides of rect A
    leftA = a.x;
    rightA = a.x + a.w;
    topA = a.y;
    bottomA = a.y + a.h;

    //Calculate the sides of rect B
    leftB = b.x;
    rightB = b.x + b.w;
    topB = b.y;
    bottomB = b.y + b.h;

    //If any of the sides from A are outside of B
    if( bottomA <= topB )
    {
        return false;
    }

    if( topA >= bottomB )
    {
        return false;
    }

    if( rightA <= leftB )
    {
        return false;
    }

    if( leftA >= rightB )
    {
        return false;
    }

    //If none of the sides from A are outside B
    return true;
}


    static int frameX =0;
    static int frameY =0;
    static int numberofframe=4;

int main( int argc, char* args[] )
{
	//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Load media
		if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			//The dot that will be moving around on the screen
			Dot dot;

			Object object;
			SDL_Rect Object;


			//The background scrolling offset
			int scrollingOffset = 0;

            dot.SetDefaultFrame(0,0,67,95);

			//While application is running
			while( !quit )
			{
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}

					//Handle input for the dot
					dot.handleEvent( e );
                    }
                    if(dot.isJumping==true){
                            if(frameX==4){
                                dot.isJumping=false;
                            }
                            frameY=1;
                            numberofframe=5;
                            ++frameX;
                            if (frameX/5 >= numberofframe){
                            frameX =0;
                            }
                    } else {
                            frameY=0;
                            numberofframe=4;
                            frameX++;
                            if (frameX/5 >= numberofframe){
                                frameX=0;
                            }
                    //isJumping=false;
                    }


                    dot.SetFrame(frameX,frameY, 5);

                    //Move the dot
                    dot.move(Object);
                    object.move();


				//Scroll background
				--scrollingOffset;
				if( scrollingOffset < -gBGTexture.getWidth() )
				{
					scrollingOffset = 0;
				}


				//Clear screen
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( gRenderer );

				//Render background
				gBGTexture.render( scrollingOffset, 0 ,NULL);
				gBGTexture.render( scrollingOffset + gBGTexture.getWidth(), 0, NULL );

				//Render objects
				dot.render();
				object.render();

				//Update screen
				SDL_RenderPresent( gRenderer );
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}
