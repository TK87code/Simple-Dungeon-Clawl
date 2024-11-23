#ifndef MONSTER_H
#define MONSTER_H

#define MAX_MONSTER_NAME 15
#define MAX_MONSTER_NUMBER 20

typedef struct monster
{
	char name[MAX_MONSTER_NAME];
	int id;
	int map_pos_x, map_pos_y;
	int combat_skill;
	int strength;
	int toughness;
	int hp;
	int current_hp;
	int base_damage;
	int is_dead;
}monster;

// Public funcs
extern void monster_create(void);
extern int monster_get_id_from_xy(int x, int y);
extern monster **monster_get_all(void);
extern monster *monster_get_from_id(int id);
extern void monster_close(void);

#endif