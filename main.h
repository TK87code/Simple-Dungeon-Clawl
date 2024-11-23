#ifndef MAIN_H
#define MAIN_H

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 800
#define TILE_SIZE 32
// The size of map drawing area in entire screen
#define MAP_AREA_WIDTH (TILE_SIZE * 29)
#define MAP_AREA_HEIGHT (TILE_SIZE * 19)
//How far we draw map on map area of screen
#define MAP_RENDER_RANGE_Y 9
#define MAP_RENDER_RANGE_X 14

typedef struct player player;
typedef struct monster monster;

static int resolve_input(int *v_dx, int *v_dy);
static void start_player_turn(player *Player, int dx, int dy);
static void start_monster_turn(monster **monsters, player *Player);
static void process_combat(player *Player, monster *Monster, int is_player_attacking);
#endif
