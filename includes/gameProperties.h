#ifndef gameProperties_h
#define gameProperties_h
#include <time.h>

#include "./../includes/debug.h"
#include "../includes/tower.h"

/*----------Enumerated Types-----------*/
enum commandType
{
    commandError=-1,
    upgrade=0,
    execute=1,
    set=2,
    man=3,
    cat=4,
    mktwr=5
};


enum upgradeStat
{
    statError=-1,
    power=0,
    range=1,
    speed=2,
    AOErange=3,
    AOEpower=4,
    level=5,
	INT=6, 
	CHAR=7
};

/*----------Symbolic Constants-----------*/

#define ACTIONCOOLDOWN	1		//! minimum time between actions being removed from queue
#define ENEMYSPAWNCOOLDOWN 5 	//!minimum time between enemy spawns
/*----------TypeDefs----------*/

typedef enum commandType commandType;
typedef enum upgradeStat upgradeStat;
typedef struct gameProperties *GameProperties;
typedef struct gameClock *GameClock;

/*----------Function Prototypes-----------*/
GameProperties createGame();
int getGold(GameProperties game);
int takeGold(GameProperties game,int gold);
int getWave(GameProperties game);
int getHealth(GameProperties game);
clock_t delayGame(int delayN);
int lastAction(GameProperties Game);
int setlastAction(GameProperties Game);
GameProperties getGame(GameProperties createdGame);
void damageHealth(int damage);
int addGold(int gold);
int getCostOfNewTower();
int getDeathCnt();
void increaseDeathCnt();
void setTotalWaveNo(int totalW);
int getTotalWaveNo();

void testingGameStructure();
void CreateGameTest();
void TestGetGold();
void TestAddGold();
void TestTakeGold();

#endif
