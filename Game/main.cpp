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
#include"Constant.h"


static int frameX =0;
static int frameY =0;
static int numberofframe=4;
bool gStartgame = false;
bool isResetgame = true;
bool button_visible = true;
std::vector<Object> v;

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
                                srand((unsigned int) time(NULL));
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
                            gTextTexture.render( 90, 15 , NULL );
                            }

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




