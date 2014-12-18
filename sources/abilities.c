#include "../includes/abilities.h"

void init_abilities(Ability *psx, Ability *kill, Ability *killall)
{
	psx->unlocked = 0;
	psx->goldcost = PSX_COST;
	kill->unlocked = 0;
	kill->goldcost = KILL_COST;
	killall->unlocked = 0;
	killall->goldcost = KILLALL_COST;
}
	
void unlock_ability(Ability *abilityx)
{
	char unlockstring[16] = "Ability unlocked";
	abilityx->unlocked = 1;
	updateTowerMonitor(unlockstring);		
}

int is_available_ability(Ability *ability)
{
	if(ability->unlocked == 1 && getTotalMemory() > ability->goldcost)
	{
		return 1;
	}
	return 0;
}

void psx_ability(Ability *psx)
{
	char psxlist[200];
	char template[14] = "EnemyID Health";
	char newline[1] = "\n";
	char IDstr[5];
	char healthstr[10];
	int enemy_number = getNumberOfEnemies();
	int health = 0, ID = 0, i;
	if(is_available_ability(psx) == 1)
	{
		for(i = 1; i <= enemy_number; i++)
		{
			strcpy(psxlist, template);
			if(!isDead(i))
			{
				ID = i;
				health = getEnemyHealth(ID);
				sprintf(IDstr, "%d ", ID);
				sprintf(healthstr, "%d", health); 
				strcat(psxlist, newline);
				strcat(psxlist, IDstr);
				strcat(psxlist, healthstr);
				strcat(psxlist, newline);
			}
			updateTowerMonitor(psxlist);
		}
	}
}
/*
void kill_ability(int enemyID, Ability *kill)
{
	if(is_available_ability(kill) == 1)
	{
		killEnemy(enemyID);
	}
}

int kill_all_ability(Ability *killall)
{
	int i;
	int enemy_number = getNumberOfEnemies();

	if(is_available_ability(killall) == 1)
	{
		for(i = 1; i <= enemy_number; i++)
		{
			killEnemy(i);
		}
		return 0;
	}
	else
	{
		return -1;
	}
	return 0;
}
*/

	



