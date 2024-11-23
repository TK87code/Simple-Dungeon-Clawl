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

static void resolve_input(player *Player);

#endif
