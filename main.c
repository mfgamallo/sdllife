#include<SDL2/SDL.h>
#include<stdio.h>
#include"life.h"

int speed;          // Speed at which the world evolves. Lower is faster. 0 = fastest.
int paused;         // Are we showing a static generation?
int step;           // Are we advancing step by step?

int **world;        // Current world.

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

void draw(int **world, SDL_Renderer *renderer) {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
  SDL_RenderClear(renderer);
  draw_world(world, renderer);
  SDL_RenderPresent(renderer);
 }

SDL_bool treat_events(SDL_Renderer *renderer) {

  SDL_bool loopShouldStop = SDL_FALSE;
  SDL_Event event;

  while(SDL_PollEvent(&event)) {
    switch(event.type) {
    case SDL_QUIT:
      loopShouldStop = SDL_TRUE;
      break;
    case SDL_KEYDOWN:
      if (event.key.repeat==0 && event.key.keysym.sym==SDLK_EQUALS && speed >= SPEED_INTERVAL)
	speed -= SPEED_INTERVAL;
      else if (event.key.repeat==0 && event.key.keysym.sym==SDLK_MINUS)
	speed += SPEED_INTERVAL;
      else if (event.key.repeat==0 && event.key.keysym.sym==SDLK_n)
	world = world_new();
      else if (event.key.repeat==0 && event.key.keysym.sym==SDLK_SPACE)
	paused = !paused;
      else if (event.key.repeat==0 && event.key.keysym.sym==SDLK_s) {
	paused = 1;
	step = 1;
      } else if (event.key.repeat==0 && event.key.keysym.sym==SDLK_1)
	speed = 220;
      else if (event.key.repeat==0 && event.key.keysym.sym==SDLK_0)
	speed = 0;
      else if (event.key.repeat==0 && event.key.keysym.sym==SDLK_LEFT) {
	draw(history_backwards(), renderer);
	paused = 1;
      } else if (event.key.repeat==0 && event.key.keysym.sym==SDLK_RIGHT) {
	draw(history_forwards(), renderer);
	paused = 1;
      }
    }
  }
  
  return loopShouldStop;
}

int main(int argc, char **argv) {

  SDL_Window* window = NULL;
  SDL_Renderer *renderer = NULL;

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

  world = life_init();

  Uint64 lastTime = 0;
  speed = 0;
  paused = 0;
  step = 0;
  
  while (!treat_events(renderer)) {
    Uint64 currentTime = SDL_GetTicks64();
    if ((currentTime > lastTime + speed && !paused) || step) {
      world = world_next(world);
      draw(world, renderer);
      lastTime = currentTime;
      if (step) step = 0;
    }
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
