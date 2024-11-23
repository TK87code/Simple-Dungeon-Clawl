#ifndef MAP_H
#define MAP_H

// Overall map size
#define MAP_HEIGHT 50
#define MAP_WIDTH 50

//tile id
enum {
	MAP_EMPTY,
	MAP_WALL,
	MAP_CLOSED_DOOR,
	MAP_OPEND_DOOR,
	MAP_ENEMY,
};

extern void map_init(void);
extern void map_draw(int center_x, int center_y, int tile_size, int render_range_x, int render_range_y);
extern int map_get_cell(int x, int y);
extern int map_check_collision(int map_id);
extern void map_open_door(int x, int y);


#endif