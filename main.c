#include<SDL2/SDL.h>
#include<stdio.h>
#include"life.h"

int world_to_screen(int world_n) {
  int screen_n = 1 + world_n * (CELL_SIZE + CELL_GAP);
  return screen_n;
}

void draw_world(int **world, SDL_Renderer *renderer) {
  int x, y;

  for (x=0; x<WORLD_WIDTH; x++) {
    for (y=0; y<WORLD_HEIGHT; y++) {
      SDL_SetRenderDrawColor(renderer, 100, 200, 100, SDL_ALPHA_OPAQUE);
      if (world[x][y]) {
	SDL_Rect rect;
	rect.x = world_to_screen(x);
	rect.y = world_to_screen(y);
	rect.w = CELL_SIZE;
	rect.h = CELL_SIZE;
	SDL_RenderFillRect(renderer, &rect);
      }
    }

  }
}

int main(int argc, char **argv) {

  SDL_Window* window = NULL;
  SDL_Renderer *renderer = NULL;

  SDL_bool loopShouldStop = SDL_FALSE;

  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    printf("Error SDL: %s", SDL_GetError());
    return -1;
  }

  int screenWidth = WORLD_WIDTH * CELL_SIZE + (WORLD_WIDTH+1) * CELL_GAP;
  int screenHeight = WORLD_HEIGHT * CELL_SIZE + (WORLD_HEIGHT+1) * CELL_GAP;
  window = SDL_CreateWindow("Life",
			    SDL_WINDOWPOS_CENTERED,
			    SDL_WINDOWPOS_CENTERED,
			    screenWidth, screenHeight, 0);

  if (window == NULL) {
    printf("Error SDL: %s", SDL_GetError());
    return -1;
  }

  renderer = SDL_CreateRenderer(window, -1, 0);

  int **world = world_random_create();
  
  while (!loopShouldStop) {
    
    SDL_Event event;
    while(SDL_PollEvent(&event)) {
      switch(event.type) {
      case SDL_QUIT:
	loopShouldStop = SDL_TRUE;
      }
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    draw_world(world, renderer);
    SDL_RenderPresent(renderer);
    //SDL_Delay(10000);
    world = world_next(world);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
