/*
TODO
. map.c refactoring
. combat.c refactoring
. message color for enemy action
. implement closing doors (what if there are multiple door adjecent?)
. simple enemy AI (move randomly and come toward player when they see)
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

enum map_tiles{
    TILE_GROUND,
    TILE_WALL,
    TILE_CLOSED_DOOR,
    TILE_OPENED_DOOR,
};

static int map[MAP_HEIGHT][MAP_WIDTH] =
{
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,1,1,1,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,2,0,2,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
};


static bool debug_mode = false; // TODO: Erase this after production
static int is_player_turn = 1;

int main(void){
	// Window Init
	const char *title = "Dungeon Clawl Classic";
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, title);
	SetTargetFPS(60);
    
	/* Level initializations */
    if (level_register_tile(TILE_WALL, GRAY, 1) != 0){
        printf("Failed to registering tiles.");
        exit(1);
    }
    
    if (level_register_tile(TILE_GROUND, LIGHTGRAY, 0) != 0){
        printf("Failed to registering tiles.");
        exit(1);
    }
    
    if (level_register_tile(TILE_CLOSED_DOOR, DARKBROWN, 1) != 0){
        printf("Failed to registering tiles.");
        exit(1);
    }
    
    if (level_register_tile(TILE_OPENED_DOOR, BROWN, 0) != 0){
        printf("Failed to registering tiles.");
        exit(1);
    }
    
    level_init(MAP_WIDTH, MAP_HEIGHT,(int*)map);
    
	// Player init
	player *Player;
	Player = player_init();
    level_update_entity(Player->map_pos_x, Player->map_pos_y, Player->map_pos_x, Player->map_pos_y, E_PLAYER);
    
	// Monster init
    for (int i = 0; i < MAX_MONSTER_NUMBER; i++){
        int x, y;
        SetRandomSeed((unsigned int)time(NULL));
        
        level_get_random_empty_pos(&x, &y);
        if (monster_create(x, y) != 0){
            printf("Failed to create a monster");
            exit(1);
        }
        
        level_update_entity(x, y, x, y, E_MONSTER);
    }
    
    
	message_insert("Welcome to the dungeon of Ayukat.", MESSAGE_NARATIVE);
	message_insert("Press '?' to show help.", MESSAGE_SYSTEM);
    
    
	while (!WindowShouldClose())
	{
        // Input
        int dx, dy;
        dx = 0;
        dy = 0;
        
        int turn_consumed = 0;
        turn_consumed = resolve_input(&dx, &dy);
        
        // Player Action 
        if (turn_consumed != 0){
            start_player_turn(Player, dx, dy);
            is_player_turn = 0;
        }
        
        // Monster Action
        monster **monsters = monster_get_all();
        if (is_player_turn == 0){
            start_monster_turn(monsters, Player);
        }
        
        
        BeginDrawing();
        
        ClearBackground(BLACK);
        
        // --- TODO: Erase this later ---
        if (debug_mode == true){
            DrawRectangle(MAP_AREA_WIDTH, 0, SCREEN_WIDTH - MAP_AREA_WIDTH, MAP_AREA_HEIGHT, GREEN); // Info Area
            DrawRectangle(0, MAP_AREA_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT - MAP_AREA_HEIGHT, RED); // Message Area
        }
        // -------------
        
        level_draw(Player->map_pos_x, Player->map_pos_y, TILE_SIZE, MAP_RENDER_RANGE_X, MAP_RENDER_RANGE_Y);
        
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

void start_player_turn(player* Player, int dx, int dy){
    cell_data cell = level_get_cell(Player->map_pos_x + dx, Player->map_pos_y + dy);
    
	if (cell.tile->is_collidable != 0)
	{
		switch (cell.tile->id) {
            case TILE_WALL:{
                message_insert("You hit the wall. \"OUCH!\"", MESSAGE_NARATIVE);
            }break;
            
            case TILE_CLOSED_DOOR:{
                level_set_tile(Player->map_pos_x + dx, Player->map_pos_y + dy, TILE_OPENED_DOOR); // Opening door
                message_insert("The door opens quietly.", MESSAGE_NARATIVE);
            }break;
        }
    }else if (cell.entity->type == E_MONSTER){
        
        // Get monster at the position where is about to move.
        int monster_id = monster_get_id_from_xy(Player->map_pos_x + dx, Player->map_pos_y + dy);
        monster *Monster = monster_get_from_id(monster_id);
        if (Monster == NULL)
        {
            // TODO: logging
            printf("Could not find a monster!");
            exit(1);
        }
        
        process_combat(Player, Monster, 1);
    }else{
        // Update player position if no collision or combat
        level_update_entity(Player->map_pos_x, Player->map_pos_y,
                            Player->map_pos_x + dx, Player->map_pos_y + dy, E_PLAYER);
        Player->map_pos_x += dx;
        Player->map_pos_y += dy;
    }
    
}

// Return 0 if the player comsumed the turn, set dx&dy in the address given.
int resolve_input(int *v_dx, int *v_dy)
{
    int turn_consumed = 0;
    
	if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_KP_6))
	{
		*v_dx = 1;
        turn_consumed = 1;
	}
	else if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_KP_4))
	{
		*v_dx = -1;
        turn_consumed = 1;
	}
	else if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_KP_8))
	{
		*v_dy = -1;
        turn_consumed = 1;
	}
	else if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_KP_2))
	{
		*v_dy = 1;
        turn_consumed = 1;
	}
	else if (IsKeyPressed(KEY_KP_7))
	{
		*v_dx = -1;
		*v_dy = -1;
        turn_consumed = 1;
	}
	else if (IsKeyPressed(KEY_KP_9))
	{
		*v_dx = 1;
		*v_dy = -1;
        turn_consumed = 1;
	}
	else if (IsKeyPressed(KEY_KP_1))
	{
		*v_dx = -1;
		*v_dy = 1;
        turn_consumed = 1;
	}
	else if (IsKeyPressed(KEY_KP_3))
	{
		*v_dx = 1;
		*v_dy = 1;
        turn_consumed = 1;
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
    
    return turn_consumed;
}

void start_monster_turn(monster **monsters, player *Player)
{
    int dx, dy;
    dx = 0;
    dy = 0;
    
    //check if any monster is adjecent to a player
    for (int i = 0; i < MAX_MONSTER_NUMBER; i++)
    {
        if (monsters[i]->is_dead == 0){
            
            if (monsters[i]->map_pos_x == Player->map_pos_x ||
                monsters[i]->map_pos_x == Player->map_pos_x + 1 ||
                monsters[i]->map_pos_x == Player->map_pos_x - 1){
                
                if (monsters[i]->map_pos_y == Player->map_pos_y ||
                    monsters[i]->map_pos_y == Player->map_pos_y + 1 ||
                    monsters[i]->map_pos_y == Player->map_pos_y -1){
                    
                    // monster attack player
                    process_combat(Player, monsters[i], 0);
                }
            }
            level_update_entity(monsters[i]->map_pos_x, monsters[i]->map_pos_y,
                                monsters[i]->map_pos_x + dx, monsters[i]->map_pos_y + dy,
                                E_MONSTER);
            
        }
    }
    
    is_player_turn = 1;
}

void process_combat(player* Player, monster *Monster, int is_player_attacking)
{
    // TODO(Takuya): weapon base damage
    // Set combat stats for passing to combat function
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
    
    // Call combat function
    if (is_player_attacking != 0)
    {
        is_hit = combat_do(&PlayerCombatStat, &MonsterCombatStat, &damage);
    }
    else
    {
        is_hit = combat_do(&MonsterCombatStat,&PlayerCombatStat, &damage);
    }
    
    // Name setting for both attacker & defender
    char *attacker_name = '\0';
    char *defender_name = '\0';
    if (is_player_attacking != 0)
    {
        attacker_name = "You";
        defender_name = Monster->name;
    }
    else
    {
        attacker_name = Monster->name;
        defender_name = "You";
    }
    
    // A process on hit
    if (is_hit != 0)
    {
        char txt[MAX_MESSAGE_LENGTH] = { 0 };
        sprintf_s(txt, sizeof(char) * MAX_MESSAGE_LENGTH, "%s hit %s. %d damage!",attacker_name ,defender_name, damage);
        message_insert(txt, MESSAGE_COMBAT);
        
        // TODO(Takuya): add process for character death
        // If monster is dead after the attack, write a message.
        if (Monster->is_dead != 0)
        {
            memset(txt, 0, sizeof(txt));
            level_update_entity(Monster->map_pos_x,Monster->map_pos_y,
                                Monster->map_pos_x,Monster->map_pos_y, E_EMPTY);
            sprintf_s(txt, sizeof(char) * MAX_MESSAGE_LENGTH, "You defeated a %s", 
                      Monster->name);
            message_insert(txt, MESSAGE_COMBAT);
        }
    }
    else
    {
        char txt[MAX_MESSAGE_LENGTH] = { 0 };
        sprintf_s(txt, sizeof(char) * MAX_MESSAGE_LENGTH, "%s missed.", attacker_name);
        message_insert(txt, MESSAGE_COMBAT);
    }
}