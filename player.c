#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "raylib.h"
#include "player.h"


player* player_init()
{
	player *Player = malloc(sizeof(player));
	if (Player == NULL)
	{
		printf("Failed to malloc.");
		exit(1);
	}
	
	strcpy_s(Player->name, MAX_NAME_LENGTH * sizeof(char), "Test");
	Player->race = RACE_DWARF;
    
	// creating the name and race combined text
    
	switch (Player->race)
	{
		case RACE_DWARF:
        sprintf_s(Player->name_and_race, MAX_NAME_AND_RACE_LENGTH * sizeof(char), "%s, the Dwarf", Player->name);
        break;
	}
	
	//Player->name_and_race = *name_and_race;
	// TODO: initialize for each races
	Player->strength = 3;
	Player->toughness = 4;
	Player->combat_skill = 40;
	Player->hp = 16;
	Player->current_hp = Player->hp;
	Player->exp = 0;
	Player->map_pos_x = 3;
	Player->map_pos_y = 3;
    
	return Player;
}

void player_show_info(player *Player, int text_size, int starts_from_x, int starts_from_y)
{
	const int textPadding = 7;
    
	// Player name & race
	DrawText(Player->name_and_race, starts_from_x, starts_from_y, text_size, YELLOW);
    
	// HP
	char hp[MAX_INFO_MSG_LENGTH] = { 0 };
	DrawText("HP: ", starts_from_x, starts_from_y + text_size + textPadding, text_size, BEIGE);
	sprintf_s(hp, MAX_INFO_MSG_LENGTH * sizeof(char), "%d (%d)", Player->current_hp, Player->hp);
	DrawText(hp, starts_from_x + 40, starts_from_y + text_size + textPadding, text_size, WHITE);
    
	// EXP
	char exp[MAX_INFO_MSG_LENGTH] = { 0 };
	DrawText("EXP: ", starts_from_x, starts_from_y + (text_size * 2) + textPadding * 2, text_size, BEIGE);
	sprintf_s(exp, MAX_INFO_MSG_LENGTH * sizeof(char), "%d", Player->exp);
	DrawText(exp, starts_from_x + 55, starts_from_y + (text_size * 2) + textPadding * 2, text_size, WHITE);
    
	// Strength
	char strength[MAX_INFO_MSG_LENGTH] = { 0 };
	DrawText("STR: ", starts_from_x, starts_from_y + (text_size * 3) + textPadding * 3, text_size, BEIGE);
	sprintf_s(strength, MAX_INFO_MSG_LENGTH * sizeof(char), "%d", Player->strength);
	DrawText(strength, starts_from_x + 55, starts_from_y + (text_size * 3) + textPadding * 3, text_size, WHITE);
}

void player_close(player* Player)
{
	free(Player);
}