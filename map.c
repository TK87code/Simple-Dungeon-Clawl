#include <string.h>
#include <stdlib.h>
#include "raylib.h"
#include "map.h"

static unsigned int MAP_HEIGHT, MAP_WIDTH;
static cell_data **level;
static tile_data *tiles;
static int tile_count;


int level_register_tile(int id, Color sprite, int is_collidable){
    
    if (id > tile_count){
        tile_count = id;
    }
    
    tiles = realloc(tiles, sizeof(tile_data) * (tile_count + 1)); // Resizing the container
    
    if (tiles == 0){
        //TODO: log
        return (-1);
    }
    
    tiles[id].id = id;
    tiles[id].sprite = sprite;
    tiles[id].is_collidable = is_collidable;
    
    tile_count++;
    
    return (0);
}

int level_init(int map_width, int map_height, int* map_data)
{
    MAP_HEIGHT = map_height;
    MAP_WIDTH = map_width;
    
    /* Allocate memory for level container */
    level = malloc( sizeof(*level) * MAP_HEIGHT);
    if (level){
        for (size_t r = 0; r < MAP_HEIGHT; r++)
        {
            level[r] = malloc(sizeof(*level[r]) * MAP_WIDTH);
        }
    }
    
    int count = 0;
    for (int y = 0; y < MAP_HEIGHT; y++){
        for (int x = 0; x < MAP_WIDTH; x++){
            
            int tile_id = map_data[y * map_width + x];
            
            level[y][x].id = count;
            level[y][x].tile = &tiles[tile_id];
            level[y][x].entity = malloc(sizeof(entity_data));
            if (level[y][x].entity == 0){
                return (-1);
            }
            level[y][x].entity->type = E_EMPTY;
            level[y][x].entity->sprite = PURPLE;
            
            count++;
        }
    }
    
    return (0);
}

void level_draw(int center_x, int center_y, int tile_size, int render_range_x, int render_range_y){
	
	int draw_starts_y = 0;
	int draw_starts_x = 0;
    
	for (int y = center_y - render_range_y; y < center_y + render_range_y + 1; y++){
		for (int x = center_x - render_range_x; x < center_x + render_range_x + 1; x++){
            /* If the specified coordinates is beyond map size, skip */
			if (x < 0 || x > MAP_WIDTH - 1 || y < 0 || y > MAP_HEIGHT - 1){
				draw_starts_x++;
				continue;
			}
            
            /* Drawing map tiles first */
            DrawRectangle(draw_starts_x * tile_size, draw_starts_y * tile_size, tile_size, tile_size, level[y][x].tile->sprite);
            
            /* Drawing entity second */
            if (level[y][x].entity->type != E_EMPTY){
                DrawRectangle(draw_starts_x * tile_size, draw_starts_y * tile_size, tile_size, tile_size, level[y][x].entity->sprite);
            }
            
            draw_starts_x++;
        }
        draw_starts_x = 0;
        draw_starts_y++;
    }
}


cell_data level_get_cell(int x, int y)
{
	return level[y][x];
}

void level_get_random_empty_pos(int* v_x, int* v_y){
    for(;;){
        int randx = GetRandomValue(0, MAP_WIDTH - 1);
        int randy = GetRandomValue(0, MAP_HEIGHT - 1);
        
        if (level[randy][randx].tile->is_collidable == 0 && 
            level[randy][randx].entity->type == E_EMPTY){
            *v_x = randx;
            *v_y = randy;
            break;
        }
    }
}

void level_update_entity(int old_x, int old_y, int new_x, int new_y, entity_type entity_type){
    
    if (entity_type == E_MONSTER){
        level[old_y][old_x].entity->type = E_EMPTY;
        level[old_y][old_x].entity->sprite = PURPLE;
        level[new_y][new_x].entity->type = entity_type;
        level[new_y][new_x].entity->sprite = RED;
    }else if (entity_type == E_PLAYER){
        level[old_y][old_x].entity->type = E_EMPTY;
        level[old_y][old_x].entity->sprite = PURPLE;
        level[new_y][new_x].entity->type = entity_type;
        level[new_y][new_x].entity->sprite = BLUE;
    }
    else if(entity_type == E_EMPTY){
        level[old_y][old_x].entity->type = E_EMPTY;
        level[old_y][old_x].entity->sprite = PURPLE;
    }
}

void level_set_tile(int x, int y, int tile_id){
    level[y][x].tile = &tiles[tile_id];
}