#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "monster.h"
//#include "raylib.h"
#include "map.h"

static monster **monsters;

void monster_create(void)
{	
	SetRandomSeed((unsigned int)time(NULL));
    
	monsters = malloc(sizeof(monster) * MAX_MONSTER_NUMBER);
	
	if (monsters == NULL)
	{
		printf("Failed to malloc");
		exit(1);
	}
	
	for (int i = 0; i < MAX_MONSTER_NUMBER; i++)
	{
		monster *Monster;
		Monster = malloc(sizeof(monster));
        
		if (Monster == NULL)
		{
			//TODO: logging
			printf("Failed to malloc");
			exit(1);
		}
        
		strcpy_s(Monster->name, sizeof(char) * MAX_MONSTER_NAME, "Giant Spider");
		// TODO: decide randomly
		Monster->id = i;
		Monster->hp = 2;
		Monster->current_hp = Monster->hp;
		Monster->strength = 1;
		Monster->toughness = 2;
		Monster->combat_skill = 35;
		Monster->base_damage = 2;
		Monster->is_dead = 0;
        
		int reserved_x[MAX_MONSTER_NUMBER] = { -1 };
		int reserved_y[MAX_MONSTER_NUMBER] = { -1 };
		int is_duplicate_xy = 0;
        
		for (;;)
		{
			int randx = GetRandomValue(0, MAP_WIDTH - 1);
			int randy = GetRandomValue(0, MAP_HEIGHT - 1);
            
            
			for (int j = 0; j < MAX_MONSTER_NUMBER; j++)
			{
				if (randx == reserved_x[j] && randy == reserved_y[j]) {
					is_duplicate_xy = 1;
				}
			}
            
			if (map_get_cell(randx, randy) == MAP_EMPTY && is_duplicate_xy == 0)
			{
				Monster->map_pos_x = randx;
				Monster->map_pos_y = randy;
				reserved_x[i] = randx;
				reserved_y[i] = randy;
                break;
			}
		}
        
		monsters[i] = Monster;
	}
}

monster **monster_get_all(void)
{
	return monsters;
}

void monster_close(void)
{	
	for (int i = 0; i < MAX_MONSTER_NUMBER; i++)
	{
		free(monsters[i]);
	}
}

monster *monster_get_from_id(int id)
{
	for (int i = 0; i < MAX_MONSTER_NUMBER; i++){
		if (monsters[i]->id == id) {
			return monsters[i];
		}
	}
    
	return NULL;
}

int monster_get_id_from_xy(int x, int y)
{
	for (int i = 0; i < MAX_MONSTER_NUMBER; i++)
	{
		if (monsters[i]->map_pos_x == x && monsters[i]->map_pos_y == y)
		{
			return monsters[i]->id;
		}
	}
    
	// if can not find any id, return -1
	return -1;
}

