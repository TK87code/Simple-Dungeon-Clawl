#ifndef PLAYER_H
#define PLAYER_H

#define MAX_NAME_LENGTH 8
#define MAX_NAME_AND_RACE_LENGTH 20
#define MAX_INFO_MSG_LENGTH 20

enum
{
	RACE_DWARF,
};

typedef struct player
{
	int map_pos_x;
	int map_pos_y;
	char name[MAX_NAME_AND_RACE_LENGTH];
	int race;
	char name_and_race[MAX_NAME_AND_RACE_LENGTH];
	int strength;
	int toughness;
	int combat_skill;
	int hp;
	int current_hp;
	int exp;
    int is_dead;
}player;

extern player* player_init();
extern void player_show_info(player *Player, int text_size, int starts_from_x, int starts_from_y);
extern void player_close(player *Player);

#endif // !PLAYER_H
