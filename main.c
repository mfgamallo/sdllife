#include<SDL2/SDL.h>
#include<stdio.h>
#include"life.h"

void draw_world(int **world, SDL_Renderer *renderer) {
  int x, y;

  for (x=0; x<WORLD_WIDTH; x++) {
    for (y=0; y<WORLD_HEIGHT; y++) {
      SDL_SetRenderDrawColor(renderer, 200, 200, 200, SDL_ALPHA_OPAQUE);
      if (world[x][y]) {
	SDL_RenderDrawPoint(renderer, x, y);
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

  window = SDL_CreateWindow("Life",
			    SDL_WINDOWPOS_CENTERED,
			    SDL_WINDOWPOS_CENTERED,
			    WORLD_WIDTH, WORLD_HEIGHT, 0);

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
