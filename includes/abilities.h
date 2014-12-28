#ifndef _abilities_h
#define _abilities_h

#include <stdio.h>
#include <stdlib.h>
#include "enemy.h"

#define PSX_COST 10
#define KILL_COST 2000
#define KILLALL_COST 10000

typedef enum AbilityID 
{
	PSX,
	KILL
}AbilityID;


//void kill_ability(int enemyID, Ability *kill);

//int kill_all_ability(Ability *killall);

void psx_ability();

int is_available_ability(AbilityID id);

void unlock_ability(AbilityID id);

void init_abilities();

#endif
