#include "../includes/abilities.h"

typedef struct Ability
{
	int unlocked;
	int cost;
	
}Ability;

typedef struct Abilities
{
	Ability psx;
	Ability kill;
}Abilities;

Abilities* get_abilities()
{
	static Abilities a;
	return &a;
}

void init_abilities()
{	
	Abilities * abl = get_abilities();
	abl->psx.unlocked = 1;
	abl->psx.cost = PSX_COST;
	abl->kill.unlocked = 0;
	abl->kill.cost = KILL_COST;
}
	
void unlock_ability(AbilityID id)
{
	char unlockstring[17] = "Ability unlocked";
	Abilities *abl = get_abilities();
	switch(id)
	{
		case PSX:
		{
			abl->psx.unlocked = 1;
			break;
		}
		case KILL:
		{
			abl->kill.unlocked = 1;
			break;
		}
		default: 
		{
			fprintf(stderr, "Ability Switch Error\n");
			exit(1);
		}
	}
	updateTowerMonitor(unlockstring);		
}

int is_available_ability(AbilityID id)
{

	Ability *a;
	switch(id)
	{
		case PSX:
		{
			a = &(get_abilities()->psx);
			break;
		}
		case KILL:
		{
			a = &(get_abilities()->kill);
			break;
		}
		default: 
		{
			fprintf(stderr, "Is avail Ability Switch Error\n");
			exit(1);
		}
	}
	if(a->unlocked == 1 && getTotalMemory() > a->cost)
	{
		return 1;
	}
	return 0;
}

void psx_ability()
{
	char *psxlist = (char*) calloc(101,sizeof(char));	
	char template[15] = "EnemyID Health";
	char newline[2] = "\n";
	char IDstr[9];
	char healthstr[10];
	char clear[500] = {'\0'};
	int enemy_number = getNumberOfEnemies();

	int health = 0, ID = 0, i, j, tmp = 0, tmp2 = 0;
	int healtha[200] = {0};
	int enemyIDa[200] = {0};
	{
		for(j = 1; j <= enemy_number; j++)
		{
			if(healtha[j] <= getEnemyHealth(j))
			{
				tmp = healtha[j];
				healtha[j] = getEnemyHealth(j);
				healtha[j+1] = tmp;
				tmp2 = enemyIDa[j];
				enemyIDa[j+1] = tmp2;
				enemyIDa[j] = j;
			}
			else
			{
				healtha[j+1] = getEnemyHealth(j);
				enemyIDa[j+1] = j;
			}
		}
		strcat(psxlist, template);
		for(i = 1; i <= enemy_number; i++)
		{
			strcpy(psxlist, template);
			if(!isDead(i))
			{
				ID = enemyIDa[i];
				health = healtha[i];
				sprintf(IDstr, "%d ", ID);
				sprintf(healthstr, "%d", health); 
				strcat(psxlist, newline);
				strcat(psxlist, IDstr);
				strcat(psxlist, healthstr);
				strcat(psxlist, newline);
			}
		}
		textToTowerMonitor(psxlist);
  
		strcpy(psxlist, clear);
	}
}



void kill_ability(int enemyID)
{
	if(is_available_ability(KILL) == 1)
	{
		killEnemy(enemyID);
	}
}

int kill_all_ability()
{
	int i;
	int enemy_number = getNumberOfEnemies();

	if(is_available_ability(KILL) == 1)
	{
		for(i = 1; i <= enemy_number; i++)
		{
			drawKillAll();
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


