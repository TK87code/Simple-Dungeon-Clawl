/*
TODO
. GitHub
. monster.h dependecy in map.c
. basic turn system (player -> enemy)
. implement closing doors (what if there are multiple door adjecent?)
. HP (and HP bar)
. EXP (and EXP bar)
. implement "key pressing down movement"

<Low Prio>
. change font
. Name input, Title screen (Max 8 characters)

*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "raylib.h"

#include "main.h"
#include "map.h"
#include "player.h"
#include "message.h"
#include "combat.h"
#include "monster.h"


// TODO: Erase this later
static bool debug_mode = false;
static bool is_player_turn = true;

int main(void){
	// Window Init
	const char *title = "Dungeon Clawl Classic";
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, title);
	SetTargetFPS(60);
    
	// Map init
	map_init();
    
	// Player init
	player *Player;
	Player = player_init();
    
	// Monster init
	monster_create();
    
	message_insert("Welcome to the dungeon of Ayukat.", MESSAGE_NARATIVE);
	message_insert("Press '?' to show help.", MESSAGE_SYSTEM);
    
    
	while (!WindowShouldClose())
	{
		int offset_x = TILE_SIZE * MAP_RENDER_RANGE_X;
		int offset_y = TILE_SIZE * MAP_RENDER_RANGE_Y;
		
		// movement key 
		resolve_input(Player);
        
		BeginDrawing();
        
		ClearBackground(BLACK);
        
		// --- TODO: Eras this later ---
		if (debug_mode == true)
		{
			DrawRectangle(MAP_AREA_WIDTH, 0, SCREEN_WIDTH - MAP_AREA_WIDTH, MAP_AREA_HEIGHT, GREEN); // Info Area
			DrawRectangle(0, MAP_AREA_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT - MAP_AREA_HEIGHT, RED); // Message Area
		}
		// -------------
        
		map_draw(Player->map_pos_x, Player->map_pos_y, TILE_SIZE, MAP_RENDER_RANGE_X, MAP_RENDER_RANGE_Y);
        
		//Draw Player at center of map area
		DrawRectangle(TILE_SIZE * MAP_RENDER_RANGE_X, TILE_SIZE * MAP_RENDER_RANGE_Y, TILE_SIZE, TILE_SIZE, BLUE);
        
		//Draw monsters
		//monster_draw(TILE_SIZE, offset_x, offset_y);
        
		// Show texts
		message_show(TEXT_SIZE , 10, MAP_AREA_HEIGHT + 10);
		player_show_info(Player, TEXT_SIZE, MAP_AREA_WIDTH + 10, 10);
		
		EndDrawing();
	}
    
	player_close(Player);
	monster_close();
	CloseWindow();
    
	return 0;
}

void resolve_input(player* Player)
{
	int dx = 0;
	int dy = 0;
    
	if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_KP_6))
	{
		dx += 1;
	}
	else if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_KP_4))
	{
		dx -= 1 ;
	}
	else if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_KP_8))
	{
		dy -= 1;
	}
	else if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_KP_2))
	{
		dy += 1;
	}
	else if (IsKeyPressed(KEY_KP_7))
	{
		dx -= 1;
		dy -= 1;
	}
	else if (IsKeyPressed(KEY_KP_9))
	{
		dx += 1;
		dy -= 1;
	}
	else if (IsKeyPressed(KEY_KP_1))
	{
		dx -= 1;
		dy += 1;
	}
	else if (IsKeyPressed(KEY_KP_3))
	{
		dx += 1;
		dy += 1;
	}
    
	int map_cell = map_get_cell(Player->map_pos_x + dx, Player->map_pos_y + dy);
    
	if (map_check_collision(map_cell))
	{
		switch (map_cell) {
            case MAP_WALL:{
                message_insert("You hit the wall. \"OUCH!\"", MESSAGE_NARATIVE);
            }break;
            
            case MAP_CLOSED_DOOR:{
                map_open_door(Player->map_pos_x + dx, Player->map_pos_y + dy);
                message_insert("The door opens quietly.", MESSAGE_NARATIVE);
            }break;
            
            case MAP_ENEMY:{
                
                // Get monster at the position where is about to move.
                int monster_id = monster_get_id_from_xy(Player->map_pos_x + dx, Player->map_pos_y + dy);
                monster *Monster = monster_get_from_id(monster_id);
                if (Monster == NULL)
                {
                    // TODO: logging
                    printf("Could not find a monster!");
                    exit(1);
                }
                
                // Process combat
                // TODO(Takuya): weapon base damage
                combat_stat PlayerCombatStat = {0};
                PlayerCombatStat.combat_skill = Player->combat_skill;
                PlayerCombatStat.strength = Player->strength;
                PlayerCombatStat.weapon_base_damage = 4;
                PlayerCombatStat.toughness = Player->toughness;
                PlayerCombatStat.current_hp = &Player->current_hp;
                PlayerCombatStat.is_dead = &Player->is_dead;
                
                combat_stat MonsterCombatStat = {0};
                MonsterCombatStat.combat_skill = Monster->combat_skill;
                MonsterCombatStat.strength = Monster->strength;
                MonsterCombatStat.weapon_base_damage = 4;
                MonsterCombatStat.toughness = Monster->toughness;
                MonsterCombatStat.current_hp = &Monster->current_hp;
                MonsterCombatStat.is_dead = &Monster->is_dead;
                
                int is_hit = 0;
                int damage = 0;
                is_hit = combat_do(&PlayerCombatStat, &MonsterCombatStat, &damage);
                
                if (is_hit != 0)
                {
                    char txt[MAX_MESSAGE_LENGTH] = { 0 };
                    sprintf_s(txt, sizeof(char) * MAX_MESSAGE_LENGTH, "You hit a %s. %d damage!", Monster->name, damage);
                    message_insert(txt, MESSAGE_COMBAT);
                    // If monster is dead after the attack, write a message.
                    if (Monster->is_dead != 0)
                    {
                        memset(txt, 0, sizeof(txt));
                        sprintf_s(txt, sizeof(char) * MAX_MESSAGE_LENGTH, "You defeated a %s", 
                                  Monster->name);
                        message_insert(txt, MESSAGE_COMBAT);
                    }
                }
                else
                {
                    message_insert("You missed.", MESSAGE_COMBAT);
                }
            }break;
            
        }
    }
    else
    {
        // Update player position if not collide
        Player->map_pos_x += dx;
        Player->map_pos_y += dy;
    }
    
    // Exit Game
    if (IsKeyPressed(KEY_ESCAPE))
    {
        WindowShouldClose();
    }
    
    // DEBUG Mode TODO: Erace this later
    if (IsKeyPressed(KEY_F1))
    {
        if (debug_mode == true)
        {
            debug_mode = false;
        }
        else
        {
            debug_mode = true;
        }
    }
}
