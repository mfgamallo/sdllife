#define WORLD_WIDTH 200
#define WORLD_HEIGHT 100

#define CELL_SIZE 4
#define CELL_GAP 2

#define MAX_SPEED 1000
#define SPEED_INTERVAL 10

#define HISTORY_LENGTH 100

#define TEXT_HEIGHT 20

int **life_init();
int **world_next();
int **world_new();
int **history_backwards();
int **history_forwards();
