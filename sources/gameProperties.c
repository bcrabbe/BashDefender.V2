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
};

struct gameProperties {

	int gold;
	int WaveNo;
	int health;
    int costOfNewTower;
	GameClock clock;
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

	return game->WaveNo;

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
 *Creates game structure.  Needs to be run in level init
 */
GameProperties createGame()	{

	GameProperties newGame = (GameProperties) malloc(sizeof(*newGame));
	newGame->clock = (GameClock) malloc(sizeof(*(newGame->clock)));
	newGame->gold=0;
	newGame->WaveNo=0;
	newGame->health=0;
    newGame->costOfNewTower = 300;
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
