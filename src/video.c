#include "chip_8.h"

int window_scale = 8;
int window_width = DISPLAY_WIDTH;
int window_height = DISPLAY_HEIGHT;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

int init_video() {
	window_width = DISPLAY_WIDTH * window_scale;
	window_height = DISPLAY_HEIGHT * window_scale;

    if(SDL_Init( SDL_INIT_VIDEO ) != 0) {
        printf("ERROR: Could not initialize SDL!\n");
        return 1;
    }

    window = SDL_CreateWindow("CHIP-8", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, window_width, window_height, SDL_WINDOW_SHOWN);
    if(window == NULL) {
        printf("ERROR: Could not initialize window!\n");
        return 1;
    }

	renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL) {
		printf("ERROR: Could not initialize renderer!\n");
		return 1;
	}

	SDL_RenderSetLogicalSize(renderer, DISPLAY_WIDTH, DISPLAY_HEIGHT);

	return 0;
}


void draw_screen(u_int8_t* display_data) {
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x0);
	SDL_RenderClear(renderer);

	SDL_Rect rect = { 4 * window_scale, 6 * window_scale, window_scale, window_scale };
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderFillRect(renderer, &rect);

	for (int x = 0; x < DISPLAY_WIDTH; x++) {
		for (int y = 0; y < DISPLAY_HEIGHT; y++) {
			if (display_data[x + (DISPLAY_WIDTH * y)] != 0) {
				SDL_Rect pixel = { x, y, 1, 1 };
				SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderFillRect(renderer, &pixel);
			}
		}
	}

	SDL_RenderPresent(renderer);
}


void close_video() {
	SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
}