#ifndef _abilities_h
#define _abilities_h

#include <stdio.h>
#include <stdlib.h>
#include "enemy.h"

#define PSX_COST 10
#define KILL_COST 2000
#define KILLALL_COST 10000

typedef struct Ability
{
	int unlocked;
	int cost;
	
}Ability;

//void kill_ability(int enemyID, Ability *kill);

//int kill_all_ability(Ability *killall);

void psx_ability(Ability *psx);

int is_available_ability(Ability *ability);

void unlock_ability(Ability *abilityx);

void init_abilities();

#endif
