#include <SDL2/SDL.h>
#include <stdio.h>

const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;

const int MAX_ITERATIONS = 64;

SDL_Window* window = NULL;
SDL_Surface* screenSurface = NULL;
unsigned int *ptr = NULL;

void init_SDL(){
	if(SDL_Init(SDL_INIT_VIDEO) < 0){
		printf("SDL init failed: %s\n", SDL_GetError());
	}
	
	window = SDL_CreateWindow(
        "Tandelbrom Tes",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        SDL_WINDOW_OPENGL
    );

    screenSurface = SDL_GetWindowSurface(window);
    ptr = (unsigned int*)screenSurface->pixels;
}

void cleanup(){
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void editpixel(int x, int y, int color){
	int lineoffset = y*(screenSurface->pitch / 4);
	ptr[lineoffset + x] = color;
}


int main(int argc, char** argv){
	init_SDL();
	SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));
	SDL_UpdateWindowSurface(window);
	
	double c_re = 0.0, c_im = 0.0, x= 0.0, y= 0.0, x_new = 0.0;
	int iters = 0;

	if(SDL_MUSTLOCK(screenSurface)){
		SDL_LockSurface(screenSurface);
	}

	for(int row = 0; row < SCREEN_HEIGHT; row++){
		for(int col = 0; col < SCREEN_WIDTH; col++){
			c_im = (row - (SCREEN_HEIGHT/2.0))*4.0/SCREEN_WIDTH;
			c_re = (col - (SCREEN_WIDTH/2.0))*4.0/SCREEN_WIDTH;
			x = 0.0, y = 0.0;

			iters = 0, x_new = 0;
			while((x*x + y*y) <= 4 && iters < MAX_ITERATIONS){
				x_new = x*x - y*y + c_re;
				y = 2.0 * x * y + c_im;
				x = x_new;
				iters++;
			}
			editpixel(col, row, iters*8192);
		}
	}

	if(SDL_MUSTLOCK(screenSurface)){
		SDL_UnlockSurface(screenSurface);
	}

	SDL_UpdateWindowSurface(window);

	SDL_Delay(10000);

	cleanup();

	return 0;
}