/* 
What this file for?:
A combat system that influenced by a TRPG which uses d100 system. 
*/
#include <math.h>

#include "combat.h"

// Take ptr to a combat_stats of each side of combat,return 0 on miss. Set damage value in the given adrress.
int combat_do(combat_stat *attacker_stat, combat_stat *defender_stat, int* damage)
{
    int success_level = 0;
    int is_hit = 0;
    *damage = 0;
    
    // roll to hit
    is_hit = combat_roll_to_hit(attacker_stat->combat_skill, defender_stat->combat_skill, &success_level);
    if (is_hit != 0)
    {
        // TODO: weapon_base_damage
        *damage = combat_calc_damage(success_level, attacker_stat->strength, attacker_stat->weapon_base_damage, defender_stat->toughness);
        *(defender_stat->current_hp) -= *damage;
        
        if (*(defender_stat->current_hp) <= 0)
        {
            *(defender_stat->is_dead) = 1;
        }
        
    }
    
    return is_hit;
}

//Checking if the attack hit. Return 0 when miss, and return succsess level as a parameter.
static int combat_roll_to_hit(int attacker_combat_skill, int defender_combat_skill, int *winner_success_level)
{	
    int is_hit = 0;
    
    // Roll d100
    int attacker_rolled_number, defender_rolled_number = 0;
    attacker_rolled_number = GetRandomValue(1, 100);
    defender_rolled_number = GetRandomValue(1, 100);
    
    // Calculate success level
    int attacker_sl, defender_sl = 0; 
    attacker_sl = (int)floor((double)attacker_combat_skill / (double)10) - (int)floor((double)attacker_rolled_number / (double)10);
    defender_sl = (int)floor((double)defender_combat_skill / (double)10) - (int)floor((double)defender_rolled_number / (double)10);
    
    // Check if attacker hit defender
    if (attacker_sl > defender_sl)
    {
        is_hit = 1;
        *winner_success_level = attacker_sl;
    }
    else if (attacker_sl < defender_sl)
    {
        is_hit = 0;
    }
    else if (attacker_sl == defender_sl)
    {
        // If both success level is the same, whichever has higher combat skill wins
        if (attacker_combat_skill > defender_combat_skill)
        {
            is_hit = 1;
            *winner_success_level = attacker_sl;
        }
        else if (attacker_combat_skill < defender_combat_skill) 
        {
            is_hit = 0;
        }
        else if (attacker_combat_skill == defender_combat_skill)
        {
            // If combat skill is also the same, Roll again
            combat_roll_to_hit(attacker_combat_skill, defender_combat_skill, winner_success_level);
        }
    }
    
    return is_hit;
}

// Calculate damage and returns the value.
static int combat_calc_damage(int success_level, int attacker_strength, int weapon_base_damage, int defender_toughness)
{
    int dmg =0;
    dmg = (attacker_strength + weapon_base_damage + success_level) - defender_toughness;
    // At least 1 damage
    if (dmg <= 0){
        dmg = 1;
    }
    
    return dmg;
}