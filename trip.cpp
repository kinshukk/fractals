#include <SDL2/SDL.h>
#include <stdio.h>
#include <string>

const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 1000;
//max iterations run on a pixel
const int MAX_ITERS = 100;

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

        double c_re = 0.0, c_im = 0.0, x = 0.0, y = 0.0;
        int iters = 0;

        while(!quit){
            while(SDL_PollEvent(&e) != 0){
                if(e.type == SDL_QUIT){
                    quit = true;
                }else{
                    if(SDL_MUSTLOCK(gScreenSurface)){
                        SDL_LockSurface(gScreenSurface);
                    }

                    double c_theta = 0.0, s_theta = 0.0, mod = 0.0, power = 2;
                    while(power < 10.0){
                        for(int i=0; i<SCREEN_WIDTH; i++){
                            for(int j=0; j<SCREEN_HEIGHT; j++){
                                c_im = (j - (SCREEN_HEIGHT/2.0))*4.0/SCREEN_WIDTH;
                                c_re = (i - (SCREEN_WIDTH/2.0))*4.0/SCREEN_WIDTH;

                                double k2 = 0.0, k1 = 0.0;

                                x = 0.0, y = 0.0;

                                mod = 0.0;
                                iters = 0;

                                while(mod <= 4 && iters < MAX_ITERS){
                                    if(y == 0.0){
                                        c_theta = 1.0;
                                        s_theta = 0.0;
                                    }else if(x == 0){
                                        c_theta = 0.0;
                                        s_theta = 1.0;
                                    }else{
                                        k1 = x/y;
                                        k2 = y/x;
                                        c_theta = k1/sqrt(1+k1*k1);
                                        s_theta = k2/sqrt(1+k2*k2);
                                    }

                                    c_theta = cos(power*acos(c_theta));
                                    s_theta = sin(power*acos(s_theta));

                                    x = mod*c_theta + c_re;
                                    y = mod*s_theta + c_im;

                                    iters++;

                                    mod = pow(sqrt(x*x + y*y), power);
                                }
                                //Find a way to control the color
                                editpixel(i, j, iters*2987654);
                            }
                        }
                        power += 0.5;
                        SDL_UpdateWindowSurface( gWindow );
                    }

                    if(SDL_MUSTLOCK(gScreenSurface)){
                        SDL_UnlockSurface(gScreenSurface);
                    }
                }
            }            
        }
    }

    close();

    return 0;
}