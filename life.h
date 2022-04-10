#define WORLD_WIDTH 200
#define WORLD_HEIGHT 100

#define CELL_SIZE 4
#define CELL_GAP 2

#define SPEED_INTERVAL 10

int** world_random_create();
int** world_next(int **);
void world_print(int **world);
