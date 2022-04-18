#include<SDL2/SDL.h>
#include<SDL2/SDL_ttf.h>
#include<stdio.h>
#include"life.h"

SDL_Window* window = NULL;
SDL_Renderer *renderer = NULL;
TTF_Font *font = NULL;
SDL_Texture *message_texture = NULL;
SDL_Surface *message_surface = NULL;
SDL_Rect message_rect;
SDL_Color textColor;

int speed;          // Speed at which the world evolves. Lower is faster. 0 = fastest.
int paused;         // Are we showing a static generation?
int step;           // Are we advancing step by step?

int **world;        // Current world.

char *hud;          // Message seen on screen
int generation;

int world_to_screen(int world_n) {
  int screen_n = 1 + world_n * (CELL_SIZE + CELL_GAP);
  return screen_n;
}

void draw_world(int **world) {
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

  sprintf(hud, "Status: %s | Generation: %d | Speed: %d", paused ? "PAUSED" : "RUNNING", generation, 1000 - speed);
  message_surface = TTF_RenderText_Solid(font, hud, textColor);
  message_texture = SDL_CreateTextureFromSurface(renderer, message_surface);

  SDL_RenderCopy(renderer, message_texture, NULL, &message_rect);

  SDL_FreeSurface(message_surface);
  SDL_DestroyTexture(message_texture);
}

void draw(int **world) {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
  SDL_RenderClear(renderer);
  draw_world(world);
  SDL_RenderPresent(renderer);
 }

SDL_bool treat_events() {

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
      else if (event.key.repeat==0 && event.key.keysym.sym==SDLK_MINUS && speed <= (MAX_SPEED - SPEED_INTERVAL))
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
	world = history_backwards();
	paused = 1;
      } else if (event.key.repeat==0 && event.key.keysym.sym==SDLK_RIGHT) {
	world = history_forwards();
	paused = 1;
      }
    }
  }
  
  return loopShouldStop;
}

int main(int argc, char **argv) {
  
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    printf("Error SDL: %s\n", SDL_GetError());
    return -1;
  }

  if (TTF_Init() != 0) {
    printf("Error TTF: %s\n", SDL_GetError());
    return -1;
  }

  int screenWidth = WORLD_WIDTH * CELL_SIZE + (WORLD_WIDTH+1) * CELL_GAP;
  int screenHeight = WORLD_HEIGHT * CELL_SIZE + (WORLD_HEIGHT+1) * CELL_GAP + TEXT_HEIGHT;
  window = SDL_CreateWindow("Life",
			    SDL_WINDOWPOS_CENTERED,
			    SDL_WINDOWPOS_CENTERED,
			    screenWidth, screenHeight, 0);

  if (window == NULL) {
    printf("Error SDL: %s\n", SDL_GetError());
    return -1;
  }

  font = TTF_OpenFont("/usr/share/fonts/truetype/freefont/FreeMono.ttf", 36);

  if (font == NULL) {
    printf("Error TTF: %s\n", SDL_GetError());
    return -1;
  }

  textColor.r = 255;
  textColor.g = 255;
  textColor.b = 255;

  message_rect.x = 0;
  message_rect.y = screenHeight - TEXT_HEIGHT;
  message_rect.w = screenWidth;
  message_rect.h = TEXT_HEIGHT + 1;

  hud = malloc(sizeof(char) * 100);

  renderer = SDL_CreateRenderer(window, -1, 0);

  world = life_init();
  generation = 0;

  Uint64 lastTime = 0;
  speed = 0;
  paused = 0;
  step = 0;
  
  while (!treat_events()) {
    Uint64 currentTime = SDL_GetTicks64();
    if ((currentTime > lastTime + speed && !paused) || step) {
      world = world_next();
      generation++;
      lastTime = currentTime;
      if (step) step = 0;
    }
    draw(world);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
