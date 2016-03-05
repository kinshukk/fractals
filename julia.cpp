#include <SDL2/SDL.h>
#include <stdio.h>
#include <string>

const int SCREEN_WIDTH = 1020;
const int SCREEN_HEIGHT = 1020;

const int MAX_ITERS = 400;

int offsetCount = 0;

//Starts up SDL and creates window
bool init();

//shuts down SDL
void close();

SDL_Window* gWindow = NULL;
SDL_Surface* gScreenSurface = NULL;

//pointer to pixels
unsigned int *ptr = NULL;

void editpixel(int x, int y, int color){
    int lineoffset = y*(offsetCount);
    ptr[lineoffset + x] = color;
}

bool init(){
    bool success = true;

    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        printf("SDL init failed! SDL_Error: %s\n", SDL_GetError());
        success = false;
    }else{
        gWindow = SDL_CreateWindow("DDDDDDDDXDDDDDDDD", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if(gWindow == NULL){
            printf("Window creation failed! SDL_Error: %s\n", SDL_GetError());
            success = false;
        }else{
            gScreenSurface = SDL_GetWindowSurface(gWindow);
        }
    }

    ptr = (unsigned int*)gScreenSurface->pixels;
    offsetCount = gScreenSurface->pitch / 4;

    return success;
}

void close(){
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    SDL_Quit();
}

int main(int argc, char* argv[]){
    if(!init()){
        printf("init failed!\n");
    }else{
        //Main loop flag
        bool quit = false;

        //Event handler
        SDL_Event e;

        //c_im and c_re are the real magic stuff
        double c_re = -0.70176, c_im = -0.3842, x = 0.0, y = 0.0, x_new = 0.0;
        int iters = 0;
        bool rendered = false;
        while(!quit){
            while(SDL_PollEvent(&e) != 0){
                if(e.type == SDL_QUIT){
                    quit = true;
                }else if(!rendered){
                    if(SDL_MUSTLOCK(gScreenSurface)){
                        SDL_LockSurface(gScreenSurface);
                    }
                    
                    for(int col=0; col<SCREEN_WIDTH; col++){
                        for(int row=0; row<SCREEN_HEIGHT; row++){
                            iters = 0;
                            x = (col-(SCREEN_WIDTH/2.0))*4.0/SCREEN_WIDTH, y = (row-(SCREEN_HEIGHT/2.0))*4.0/SCREEN_WIDTH;

                            while(iters<MAX_ITERS && (x*x + y*y)<2.0){
                                x_new = x*x - y*y + c_re;
                                y = 2*x*y + c_im;
                                x = x_new;

                                iters++;
                            }
                            //some arbitrary const
                            editpixel(col, row, iters*12341234);
                        }
                    }
                    SDL_UpdateWindowSurface( gWindow );

                    if(SDL_MUSTLOCK(gScreenSurface)){
                        SDL_UnlockSurface(gScreenSurface);
                    }
                }else{
                    SDL_Delay(200);
                }
            }            
        }
    }

    close();

    return 0;
}