#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include"life.h"

void world_print(int **world) {
  int x, y;
  for (x=0; x<WORLD_WIDTH; x++) {
    for (y=0; y<WORLD_HEIGHT; y++) {
      printf("%d", world[x][y]);
    }
    printf("\n");
  }
}

int is_alive(int **world, int x, int y) {
  int alive;
  int neighbours = 0;
  if (x < WORLD_WIDTH-1 && y > 0 && world[x+1][y-1]) neighbours++;
  if (x < WORLD_WIDTH-1 && world[x+1][y]) neighbours++;
  if (x < WORLD_WIDTH-1 && y < WORLD_HEIGHT-1 && world[x+1][y+1]) neighbours++;
  if (y < WORLD_HEIGHT-1 && world[x][y+1]) neighbours++;
  if (x > 0 && y < WORLD_HEIGHT-1 && world[x-1][y+1]) neighbours++;
  if (x > 0 && world[x-1][y]) neighbours++;
  if (x > 0 && y > 0 && world[x-1][y-1]) neighbours++;
  if (y > 0 && world[x][y-1]) neighbours++;

  if (world[x][y]) {
    if (neighbours < 2 || neighbours > 3) alive = 0;
    else alive = 1;
  } else {
    if (neighbours == 3) alive = 1;
    else alive = 0;
  }

  return alive;
}

int** world_random_create() {
  time_t t;
  srand((unsigned) time(&t));
  
  int **world = (int**)malloc(sizeof(int *) * WORLD_WIDTH);

  int x, y;
  for (x=0; x<WORLD_WIDTH; x++) {
    int *row = (int *)malloc(sizeof(int) * WORLD_HEIGHT);
    world[x] = row;
    for (y=0; y<WORLD_HEIGHT; y++) {
      world[x][y] = rand()%2;
    }
  }

  return world;
}

int** world_empty_create() {
  int **world = (int **)malloc(sizeof(int *) * WORLD_WIDTH);

  int x, y;
  for (x=0; x<WORLD_WIDTH; x++) {
    int *row = (int *)malloc(sizeof(int) * WORLD_HEIGHT);
    world[x] = row;
    for (y=0; y<WORLD_HEIGHT; y++) {
      world[x][y] = 0;
    }
  }

  return world;
}

void world_destroy(int **world) {
  int x;
  for (x=0; x<WORLD_WIDTH; x++) {
    free(world[x]);
  }
  free(world);
}

int** world_next(int **world) {
  int **new_world = (int**)malloc(sizeof(int *) * WORLD_WIDTH);

  int x, y;
  for (x=0; x<WORLD_WIDTH; x++) {
    int *row = (int *)malloc(sizeof(int) * WORLD_HEIGHT);
    new_world[x] = row;
    for (y=0; y<WORLD_HEIGHT; y++) {
      new_world[x][y] = is_alive(world, x, y);
    }
  }

  return new_world;
}
