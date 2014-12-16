//
// sputFunctions.c
// Created by bclarke on 2014/09/11
// tags: stack, abstraction, struct pointers
//
/*---------- Standard Headers -----------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

/*---------- Custom Headers	-----------*/
#include "../includes/sput.h"
#include "./../includes/gameProperties.h"

/*---------- Data Types -----------*/

struct gameClock	{

	clock_t start_t; //! time game started.  Must be ran at startup. 
	clock_t lastAction;
	ClockNode first;
	ClockNode last;
};

struct clockNode	{

	clockType type;
	clock_t time;
	ClockNode next;

};


struct gameProperties {

	int gold;
	int currWaveNo;
	int totalWaveNo;
	int health;
    int costOfNewTower;
	GameClock clock;
	int deathCount;
};

/*---------- Functions ----------*/

/*
 *Delays game by specified amount
 */
clock_t delayGame(int delayN)	{

	clock_t ticks1, ticks2,timeWaited;
    ticks1=clock();
    timeWaited = ticks2=ticks1;
    while((ticks2/CLOCKS_PER_SEC-ticks1/CLOCKS_PER_SEC)<delayN){
        ticks2=clock();
		timeWaited = (ticks2/CLOCKS_PER_SEC-ticks1/CLOCKS_PER_SEC);
	}
	return timeWaited;
}


/*
 *Sets lastAction member to current clock time
 */
int setlastAction(GameProperties Game)	{
	Game->clock->lastAction = clock()/CLOCKS_PER_SEC;
	return (int) Game->clock->lastAction;
}

void setClock(clockType clockToSet)	{

		//*clockToSet = clock()/CLOCKS_PER_SEC;
}

/*
 *Increases Death Count
 */
void increaseDeathCnt()	{

	GameProperties game = getGame(NULL);
	game->deathCount++;
}


/*
 *Returns Death Count
 */
int getDeathCnt()	{

	return getGame(NULL)->deathCount;
}

/*
 *Returns total waves for current level
 */
int getTotalWaveNo()	{

	return getGame(NULL)->totalWaveNo;
}

/*
 *Sets total Waves this level
 */
void setTotalWaveNo(int totalW)	{

		getGame(NULL)->totalWaveNo = totalW;
	
}

void testSetLastAction()	{

 	GameProperties newGame = createGame();
	clock_t currTime = clock()/CLOCKS_PER_SEC;
	delayGame(2);
	sput_fail_unless(setlastAction(newGame) == (currTime + 2),"Setting Last Action to current time");
	delayGame(2);
	sput_fail_unless(setlastAction(newGame) == (currTime + 4),"Setting Last Action to current time");
	free(newGame->clock);
	free(newGame);
}

/*
 * returns true is last action time is more than global cooldown
 */
int lastAction(GameProperties Game)	{

	clock_t currTime = (double) clock() / CLOCKS_PER_SEC;
	clock_t timeSinceLastAction = currTime - Game->clock->lastAction;
	if (timeSinceLastAction >= ACTIONCOOLDOWN)	{
		Game->clock->lastAction = currTime;
		return 1;	
	}
		return 0;
}



/*
 *Damages play health with specified amount of damage
 */
void damageHealth(int damage)	{

	getGame(NULL)->health -= damage;

}



void testlastAction()	{

	GameProperties newGame = createGame();
	delayGame(ACTIONCOOLDOWN);
	sput_fail_unless(lastAction(newGame) == 1,"Checking delay more than Cooldown returns true");
	delayGame(ACTIONCOOLDOWN-1);
	sput_fail_unless(lastAction(newGame) == 0,"Checking delay less than Cooldown returns false");
	free(newGame->clock);
	free(newGame);

}

void testingGameStructure()	{
	sput_start_testing();
	
	sput_set_output_stream(NULL);

	sput_enter_suite("testlastAction(): Cooldown checking");
	sput_run_test(testlastAction);
	sput_leave_suite();
	
	sput_enter_suite("testSetLastAction(): Setting last action to current clock");
	sput_run_test(testSetLastAction);
	sput_leave_suite();

	sput_enter_suite("CreateGameTest(): Creation & Initialization");
	sput_run_test(CreateGameTest);
	sput_leave_suite();

	sput_enter_suite("TestGetGold(): Current Gold Value");
	sput_run_test(TestGetGold);
	sput_leave_suite();

	sput_enter_suite("TestAddGold(): Adding Gold");
	sput_run_test(TestAddGold);
	sput_leave_suite();

	sput_enter_suite("TestSubtractGold(): Subtracting Gold");
	sput_run_test(TestTakeGold);
	sput_leave_suite();

	sput_finish_testing();
}

void CreateGameTest()	{

	GameProperties testGame;
	testGame = createGame();
	sput_fail_if((createGame()) == NULL,"Creating Game");
	sput_fail_unless(getGold(testGame) == 0,"Initializing Gold");
	sput_fail_unless(getWave(testGame) == 0,"Initializing WaveNo");
	sput_fail_unless(getHealth(testGame) == 0,"Initializing Health");
	free(testGame);
}

/*
 *Returns or sets memory address of game structure`
 */
GameProperties getGame(GameProperties createdGame)	{

	static GameProperties game;

	if (createdGame != NULL)	{
		game = createdGame;
	}
	return game;

}

/*
 *Returns the current wave number
 */
int getWave(GameProperties game)	{

	return game->currWaveNo;

}

int getHealth(GameProperties game)	{

	return game->health;
}

/*
 *Returns cost of new tower
 */
int getCostOfNewTower() {

    return getGame(NULL)->costOfNewTower;
}
/*
 *Creating Game Clock linked list
 */
GameClock createClock()	{
	GameClock clock = (GameClock) malloc(sizeof(*clock));	
	clock->first = NULL;
	clock->last = NULL;
	getClock(clock);
	return clock;
}

GameClock getClock(GameClock clock)	{

		static GameClock currClock;
		if(clock != NULL)	{
			currClock = clock;
		}

		return currClock;

}

/*
 * Add Clock Node
 */
void addClock(clockType type)	{
	
	GameClock clock = getClock(NULL);	
	if(checkUniqueClockType(type))	{
		if(clock->first == NULL)	{
			clock->first = clock->last = createClockNode(type);
		} else {
			clock->last->next = createClockNode(type);
			clock->last = clock->last->next;
		}
	}
}

ClockNode createClockNode(clockType type)	{
	ClockNode newNode;
	newNode = (ClockNode) malloc(sizeof(*newNode));
	newNode->next = NULL;
	newNode->time = clock();
	newNode->type = type;
	return newNode;
}

/*
 *Checks Clock Type is unique
 */

int checkUniqueClockType(clockType type)	{

	GameClock clock = getClock(NULL);
	ClockNode currNode;
	currNode = clock->first;
	while(currNode!= NULL)	{
		if (currNode->type == type)	{
			return 0;
		}
		currNode = currNode->next;
	}

	return 1;

}

int checkClock(clockType cType,int coolDown)	{
	GameClock gClock = getClock(NULL);
	ClockNode currNode;
	currNode = gClock->first;

	clock_t currTime = clock() / CLOCKS_PER_SEC;
	clock_t timeSinceLastUse;

	while(currNode!= NULL)	{
		if (currNode->type == cType)	{
			timeSinceLastUse = currTime - currNode->time;
			if(timeSinceLastUse >= coolDown)	{
				setCurrTime(currNode);
				return 1;
			} else {
				return 0;
			}	
		}
		currNode = currNode->next;
	}

	fprintf(stderr,"clock does not exist\n");
	return 0;
}

void setCurrTime(ClockNode node)	{
	node->time = (double) clock() / CLOCKS_PER_SEC;
}

/*
 *Creates game structure.  Needs to be run in level init
 */
GameProperties createGame()	{

	GameProperties newGame = (GameProperties) malloc(sizeof(*newGame));
	newGame->clock = createClock();
	newGame->gold=1000;
	newGame->currWaveNo=0;
	newGame->totalWaveNo = 0;
	newGame->health=100;
    newGame->costOfNewTower = 300;
	newGame->deathCount = 0;
	newGame->clock->start_t  = newGame->clock->lastAction = (double) clock()/CLOCKS_PER_SEC;
	getGame(newGame);
	return newGame;

}

/*
 *Returns amount of gold available
 */
int getGold(GameProperties game)	{

	return game->gold;
}

int getTotalGold()	{

	return(getGold(getGame(NULL)));
}

void TestGetGold()	{

	GameProperties testGame;
    testGame = createGame();
	testGame->gold = 10;
	sput_fail_unless(getGold(testGame) == 10,"Getting Gold");	
	free(testGame);
}

/*
 *Adds specified amount of gold to pot
 */
int addGold(int gold)	{
	GameProperties game = getGame(NULL);
	if(gold > 0)	{
		game->gold+=gold;	
		return 1;
	} 

	return 0;
}

void TestAddGold()	{

	GameProperties testGame;
    testGame = createGame();
	addGold(100);
	sput_fail_unless(getGold(testGame) == 100,"Adding Gold");
	sput_fail_unless(addGold(-100) == 0,"Adding Negative Gold");
	free(testGame);
}

/*
 *Subtracts specified amount of gold
 */
int takeGold(GameProperties game,int gold)	{

	if (game->gold >= gold)	{
		game->gold-=gold;
		return 1;
	} else {
		return 0;
	}
}

void TestTakeGold()	{

	GameProperties testGame;
    testGame = createGame();
	testGame->gold = 100;
	takeGold(testGame,50);
	sput_fail_unless(getGold(testGame) == 50,"Subtracting Gold");
	sput_fail_unless(takeGold(testGame,100) == 0,"Subtracting too much Gold");
	free(testGame);
}
