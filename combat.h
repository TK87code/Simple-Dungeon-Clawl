#ifndef COMBAT_H
#define COMBAT_H

typedef struct combat_stat
{
    int combat_skill;
    int strength;
    int weapon_base_damage;
    int toughness;
    int *current_hp;
    int *is_dead;
}combat_stat;

// Public funcs
extern int combat_do(combat_stat *attacker, combat_stat *monster, int* damage);

// Internal funcs
static int combat_roll_to_hit(int attacker_combat_skill, int defender_combat_skill, int *winner_success_level);
static int combat_calc_dmg(int success_level, int attacker_strength, int weapon_base_damage, int defender_toughness);
#endif