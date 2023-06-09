#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <random>
#include <time.h>
#include <fstream>
#include <SDL_mixer.h>

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

		//Creates image from font string
		bool loadFromRenderedText( std::string textureText, SDL_Color textColor );

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

        void SetDimension(int w, int h);

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

        LTexture GetDotTexture() {
            return mDotTexture;
		}

		SDL_Rect getCollider();

		bool isJumping = false;

        int mPosX, mPosY;

        //Dot's collision box
        SDL_Rect mCollider;

    private:
        //The velocity of the dot
		int mVelX, mVelY;

        int ay=0;

        SDL_Rect mFrame;

        LTexture mDotTexture;


};
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

TTF_Font *gFont = NULL;

TTF_Font *gFont1 = NULL;

//The music that will be played
Mix_Music *gMusic = NULL;

LTexture gDotTexture;
LTexture gBGTexture;
LTexture gObTexture;
LTexture gTextTexture;
LTexture gButtonTexture;
LTexture gText1Texture;
LTexture gLoseTexture;
LTexture gText2Texture;
LTexture gGameTexture;
LTexture gStartTexture;

static int score =0;
static int highscore=0;


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

bool LTexture::loadFromRenderedText( std::string textureText, SDL_Color textColor )
{
	//Get rid of preexisting texture
	free();

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid( gFont, textureText.c_str(), textColor );
	if( textSurface == NULL )
	{
		printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
	}
	else
	{
		//Create texture from surface pixels
        mTexture = SDL_CreateTextureFromSurface( gRenderer, textSurface );
		if( mTexture == NULL )
		{
			printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface( textSurface );
	}

	//Return success
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

void LTexture::SetDimension(int w, int h){
    mWidth = w;
    mHeight = h;

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
    mPosX = 0;
    mPosY = 0;

    //Set collision box dimension
	mCollider.w = 30;
	mCollider.h = 92;

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

void Dot::move()
{
    //Move the dot up or down
    mPosY -= mVelY;
    mVelY -= ay;

    //If the dot went too far up or down
    if( ( mPosY < 0 ) || ( mPosY + mDotTexture.getHeight() > SCREEN_HEIGHT ) )
    {
        //Move back
        mPosY -= mVelY;
    }

     if(mPosY > 187) {
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

void Dot::render()
{
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



Object::Object()
{
	/*mSpriteWidth = 0;
	mSpriteHeight = 0;*/
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
    //Show the object
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

				 //Initialize SDL_ttf
				if( TTF_Init() == -1 )
				{
					printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
					success = false;
				}

                 //Initialize SDL_mixer
				if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
				{
					printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
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
	if( !gDotTexture.loadFromFile( "image/20.png" ) )
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

	gFont = TTF_OpenFont( "image/Alodraca.otf", 35 );
	if( gFont == NULL )
	{
		printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
		success = false;
	}
	else
	{
		//Render text
		std::string s = "score: " + std::to_string(score);
		SDL_Color textColor = { 255,255,255 };
		if( !gTextTexture.loadFromRenderedText( s.c_str(), textColor ) )
		{
			printf( "Failed to render text texture!\n" );
			success = false;
		}
	}

    //Load music
	gMusic = Mix_LoadMUS( "image/music.mp3" );
	if( gMusic == NULL )
	{
		printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}

    if( !gButtonTexture.loadFromFile( "image/start.png" ) )
	{
		printf( "Failed to load dot texture!\n" );
		success = false;
	}

    gFont1 = TTF_OpenFont( "image/font.ttf", 20 );
	if( gFont1 == NULL )
	{
		printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
		success = false;
	}
	else
	{
		//Render text
		std::string s = "Highscore: " + std::to_string(highscore/5);
		SDL_Color textColor = { 255, 255, 255};
		if( !gText1Texture.loadFromRenderedText( s.c_str(), textColor ) )
		{
			printf( "Failed to render text texture!\n" );
			success = false;
		}
	}

        if( !gLoseTexture.loadFromFile( "image/22.png" ) )
	{
		printf( "Failed to load dot texture!\n" );
		success = false;
	}

	gFont1 = TTF_OpenFont( "image/font.ttf", 20 );
	if( gFont1 == NULL )
	{
		printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
		success = false;
	}
	else
	{
		//Render text
		SDL_Color textColor = { 255, 255, 255};
		if( !gText2Texture.loadFromRenderedText( "Press SPACEBACK to start game!", textColor ) )
		{
			printf( "Failed to render text texture!\n" );
			success = false;
		}
	}

	if( !gGameTexture.loadFromFile( "image/23.png" ) )
	{
		printf( "Failed to load background texture!\n" );
		success = false;
	}

	if( !gStartTexture.loadFromFile( "image/24.png" ) )
	{
		printf( "Failed to load background texture!\n" );
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
    gTextTexture.free();
    gButtonTexture.free();
    gText1Texture.free();
    gLoseTexture.free();
    gText2Texture.free();
    gGameTexture.free();

    //Free global font
	TTF_CloseFont( gFont );
	gFont = NULL;

    TTF_CloseFont( gFont1);
	gFont1 = NULL;

	//Free the music
	Mix_FreeMusic( gMusic );
	gMusic = NULL;

	//Destroy window
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	Mix_Quit();
	TTF_Quit();
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

void updateHighScore()
{
    if (score > highscore) {
        highscore = score;
    }
}

void saveHighScore()
{
    std::ofstream file("highscore.txt");
    file << highscore;
    file.close();
}

void loadHighScore()
{
    std::ifstream file("highscore.txt");
    if (file.is_open()) {
        file >> highscore;
        file.close();
    }
}
void resetHighScore()
{
    std::remove("highscore.txt");
    std::FILE* file = std::fopen("highscore.txt", "w");
    std::fclose(file);
}

    static int frameX =0;
    static int frameY =0;
    static int numberofframe=4;
    bool gStartgame = false;
    std::vector<Object> v;
    bool isResetgame = true;
    bool button_visible = true;
    bool press = true;

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

			//Object object;

			for(int i=0;i<3;i++){
                srand((unsigned int) time(NULL)) ;
                int k = rand()%3;
                if(k==0){
                    v.push_back(Object(gRenderer,"image/3.png",42,88));
                } else if(k==1){
                    v.push_back(Object(gRenderer,"image/4.png",60,88));
                } else {
                    v.push_back(Object(gRenderer,"image/5.png",50,88));
                }
                v[i].SetDimension(SCREEN_WIDTH + i* 300 ,185);
                v[i].mVobX =7;
			}

            //The background scrolling offset
                int scrollingOffset = 0;


            dot.SetDefaultFrame(0,0,67.9,92);
            dot.SetDimension(190,187);

            resetHighScore();
            loadHighScore();


			//While application is running
			while( !quit )
			{
			    Uint32 start = SDL_GetTicks();
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                          int mouse_x = e.button.x;
                          int mouse_y = e.button.y;
                          // Kiểm tra xem click chuột trái vào ô (x,y) hay không
                          if (mouse_x >= 400 && mouse_x < 400 + 96 && mouse_y >= 170 && mouse_y < 170 + 49) {
                            // Xử lý sự kiện click chuột trái vào ô tại tọa độ (x,y)
                                button_visible = false;
                            }
					} else if (e.key.keysym.sym == SDLK_SPACE) {
                        if(!button_visible){
                            gStartgame = true;
                            press = false;
                            if( Mix_PlayingMusic() == 0 )
							{
								//Play the music
								Mix_PlayMusic( gMusic, -1 );
							}
                        }


					} else if(e.key.keysym.sym == SDLK_r){
                              if(isResetgame) {
                            score=0;
                            dot.SetDefaultFrame(0,0,67.9,92);
                            dot.mPosX = 190;
                            dot.mPosY=187;

                            for(int i=0;i<3;i++){
                                srand((unsigned int) time(NULL)) ;
                                int k = rand()%3;
                                if(k==0){
                                    v.push_back(Object(gRenderer,"image/3.png",42,88));
                                } else if(k==1){
                                    v.push_back(Object(gRenderer,"image/4.png",60,88));
                                } else {
                                    v.push_back(Object(gRenderer,"image/5.png",50,88));
                                }
                                v[i].SetDimension(SCREEN_WIDTH + i* 300 ,185);
                                v[i].mVobX =7;
                                    }
                                  }
					}

					//Handle input for the dot
					dot.handleEvent( e );
                    }
                    if (gStartgame == true){

                        if(dot.isJumping==true){
                            if(frameX==3){
                                dot.isJumping=false;
                            }
                                frameY=1;
                                numberofframe=3;
                                ++frameX;
                            if (frameX/7 >= numberofframe){
                                frameX =0;
                            }
                        } else {
                                frameY=0;
                                numberofframe=6;
                                frameX++;
                            if (frameX/7 >= numberofframe){
                                frameX=0;
                            }
                        }
                        dot.SetFrame(frameX,frameY, 7);


                        dot.move();
                        for(int i =0;i<3;i++){
                        v[i].move();
                        }
                    //Scroll background
                    scrollingOffset-=3;
                    if( scrollingOffset < -gBGTexture.getWidth() )
                        {
                        scrollingOffset = 0;
                        }

                    for(int i=0;i<3;i++){
                    if(checkCollision(dot.getCollider(),v[i].getCollider())){
                            gStartgame = false;
                        }
                    }
                    for(int i=0;i<3;i++){
                        if(v[i].mPosX < - v[i].GetWidth() ){
                            score++;
                            }
                        }
                    }



                //Clear screen
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( gRenderer );

				//Render background
				gBGTexture.render( scrollingOffset, 0 ,NULL);
				gBGTexture.render( scrollingOffset + gBGTexture.getWidth(), 0, NULL );


                if(button_visible){
                gButtonTexture.render(4*SCREEN_WIDTH/9,SCREEN_HEIGHT/2,NULL);
                 gGameTexture.render(220,80,NULL);
                    }

                        if(!button_visible){
                            dot.render();
                            //object.render();
                            for(int i=0;i<3;i++){
                                v[i].render();
                                }

                            std::string s = "score: " + std::to_string(score/8);
                            SDL_Color textColor = { 255,255,255 };
                            gTextTexture.loadFromRenderedText( s.c_str(), textColor );

                            gTextTexture.render( 10, 280 , NULL );
                        }
                     updateHighScore();

                    if(!button_visible){
                         std::string s = "Highscore: " + std::to_string(highscore/5);
                            SDL_Color textColor = { 255,255,255 };
                            gText1Texture.loadFromRenderedText( s.c_str(), textColor );
                            gText1Texture.render( 10,5 , NULL );

                        if(press){
                        gText2Texture.render(150,110,NULL);
                        }
                        if(!gStartgame){
                            gStartTexture.render(240,175,NULL);
                        }
                    }
                    for(int i =0; i<3;++i){
                    if(checkCollision(dot.getCollider(),v[i].getCollider())){
                         gLoseTexture.render(300,80,NULL);
                        }
                    }
                   saveHighScore();


				//Update screen
				SDL_RenderPresent( gRenderer );

				Uint32 finish = SDL_GetTicks();

				/*Set a frame cap
				Nếu thời gian thực hiện 1 frame mà nhỏ hơn 16 thì sẽ Delay cho đếnn khi máy tính load xong ảnh thì mới thực hiện vòng lặp tiếp.
				*/
				if(finish - start < 16){
                    SDL_Delay(16- (finish - start));
				}
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}




