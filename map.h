#ifndef MAP_H
#define MAP_H

#include "raylib.h" // TODO: erase this later

/* Entity data*/
typedef enum entity_type{
    E_EMPTY,
    E_MONSTER,
    E_PLAYER,
} entity_type;

typedef struct entity_data{
    entity_type type;
    Color sprite;
} entity_data;

/* Tile Data */
typedef struct tile_data{
    int id;
    //char * sprite; //TODO: contain a path to tile image
    Color sprite; // TODO: erase this later
    int is_collidable;
} tile_data;

/* Cell Data */
typedef struct cell_data{
    int id;
    tile_data *tile;
    entity_data *entity;
} cell_data;

/**
@brief Take map data(2d array) as a pointer, create level data.
@param map_width The number of tiles contains in width
@param map_height The number of tiles contains in height
*/
extern int level_init(int map_width, int map_height, int* map_data);

/**
@brief Register tiles.
@param id ID
@param sprite The string for the path to sprite image.
@param is_collidable If the tile should be collidable or not.
@return 0 on success, non-zero value on errors.
*/
extern int level_register_tile(int id, Color sprite, int is_collidable);

extern void level_get_random_empty_pos(int* v_x, int* v_y);

extern void map_draw(int center_x, int center_y, int tile_size, int render_range_x, int render_range_y);

/**
@brief Return cell data contained in "level" 2d array.
@param x The x coordinated on the map
@param y The y coordinate on the map
@return A pointer to the cell_data struct contained in "level" 2d array
*/
extern cell_data level_get_cell(int x, int y);

extern int map_check_collision(int map_id);

extern void map_open_door(int x, int y);

void level_update_entity(int old_x, int old_y, int new_x, int new_y, entity_type entity_type);

extern void level_set_tile(int x, int y, int tile_id);

/**
@brief Free all memory that allocated for each cell in the level
*/
extern void level_close(void);

#endif