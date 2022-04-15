#define WORLD_WIDTH 200
#define WORLD_HEIGHT 100

#define CELL_SIZE 4
#define CELL_GAP 2

#define SPEED_INTERVAL 10

#define HISTORY_LENGTH 100

int **life_init();
int **world_next(int **);
int **world_new();
void world_print(int **world);
int **history_backwards();
int **history_forwards();
