#include "vmio.h"

#define SCREEN_WIDTH		vm_graphic_get_screen_width()
#define SCREEN_HEIGHT		vm_graphic_get_screen_height()

#define BIRD_WIDTH			34
#define BIRD_HEIGHT			24

#define BIRD_X				(SCREEN_WIDTH / 4)
#define BIRD_SCALE 0.5

#define GRAVITY				1
#define FLAP_STRENGTH		-10
#define PIPE_WIDTH			30
#define PIPE_GAP			60
#define PIPE_SPEED			3
#define PIPE_SPACING		(SCREEN_WIDTH / 2 + 20)
#define NUM_PIPES			3
#define GROUND_HEIGHT       10
#define GAME_SPEED_MS       50

extern const VMUINT16 bird_pixel_data[BIRD_HEIGHT][BIRD_WIDTH];

VMINT       bird_y;
VMINT       bird_vel;
VMINT       pipe_x[NUM_PIPES];
VMINT       pipe_gap_y[NUM_PIPES];
VMINT       score;
VMBOOL      game_over;

void draw_game(void);
void reset_game(void);
void timer_callback(VMINT tid);
