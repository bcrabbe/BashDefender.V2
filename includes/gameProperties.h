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

typedef enum clockType	{

	start_t = 0,
	lastCmdAction = 1,
	enemyGroupCreated1 = 2,
	singleEnemyCreated = 3
	
} clockType;

/*----------Symbolic Constants-----------*/

#define ACTIONCOOLDOWN	1		//! minimum time between actions being removed from queue
#define ENEMYSPAWNCOOLDOWN 2 	//!minimum time between enemy spawns
/*----------TypeDefs----------*/

typedef enum commandType commandType;
typedef enum upgradeStat upgradeStat;
typedef struct gameProperties *GameProperties;
typedef struct gameClock *GameClock;
typedef struct clockNode *ClockNode;
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
void addClock(clockType type);
GameClock getClock(GameClock clock);
int checkUniqueClockType(clockType type);
ClockNode createClockNode(clockType type);
void damageHealth(int damage);
int addGold(int gold);
int getCostOfNewTower();
int getDeathCnt();
void increaseDeathCnt();
void setTotalWaveNo(int totalW);
int getTotalWaveNo();
GameClock createClock();
int checkClock(clockType cType,int coolDown);
void setCurrTime(ClockNode node);
void testingGameStructure();
void CreateGameTest();
void TestGetGold();
void TestAddGold();
void TestTakeGold();
void setEnemyCreated1();
int getTotalGold();
#endif
