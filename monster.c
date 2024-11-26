/*
Monster related functions.
Dependency: map.h
*/
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "monster.h"
#include "map.h"

static monster **monsters;
static unsigned int monster_count;

int monster_create(int x, int y)
{	
    monsters = realloc(monsters, sizeof(monster) * (monster_count + 1));
	
	if (monsters == NULL)
	{
		// TODO: log
        printf("Failed to malloc");
		return (-1);
	}
	
    monster *Monster;
    Monster = malloc(sizeof(monster));
    
    if (Monster == NULL)
    {
        //TODO: logging
        printf("Failed to malloc");
        return (-1);
    }
    
    strcpy_s(Monster->name, sizeof(char) * MAX_MONSTER_NAME, "Giant Spider");
    // TODO: decide randomly
    Monster->id = monster_count;
    Monster->hp = 2;
    Monster->current_hp = Monster->hp;
    Monster->strength = 1;
    Monster->toughness = 2;
    Monster->combat_skill = 35;
    Monster->base_damage = 2;
    Monster->is_dead = 0;
    Monster->map_pos_x = x;
    Monster->map_pos_y = y;
    
	monster_count++;
    
    monsters[Monster->id] = Monster;
    
    return (0);
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

