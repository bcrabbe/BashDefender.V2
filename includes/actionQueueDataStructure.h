#ifndef actionQueueDataStructure_h
#define actionQueueDataStructure_h

#include "../includes/debug.h"
#include "../includes/gameProperties.h"
#include "../includes/tower.h"

/*----------Enumerated Types-----------*/

/*----------Symbolic Constants-----------*/

/*----------TypeDefs-----------*/

typedef struct queueNode *QueueNode;
typedef struct actionQueueStructure *ActionQueueStructure;

/*----------Function Prototypes-----------*/
int checkTime(GameProperties Game);
int checkQueue(ActionQueueStructure queue, GameProperties Game, int needed);
ActionQueueStructure createActionQueue();
int createNode(ActionQueueStructure queue);
int pushToQueue(ActionQueueStructure queue, commandType command, upgradeStat option, int target);
int popFromQueue(ActionQueueStructure queue, commandType *cmd, upgradeStat *stat, int *target);
int checkGold(int needed, GameProperties Game);
commandType getFirstCommand(ActionQueueStructure queue);
commandType getLastCommand(ActionQueueStructure queue);
upgradeStat getFirstOption(ActionQueueStructure queue);
upgradeStat getLastOption(ActionQueueStructure queue);
ActionQueueStructure getQueue(ActionQueueStructure queue);
int getLastTarget();
int getFirstTarget();
int calulateCosts(commandType cmd, upgradeStat stat, int target);
int getCurrentStat(upgradeStat stat,int target);
int popToTower();
void testcheckGold();
void testingActionQueue();
void testPopFromQueue();
void testPushToQueue();

#endif
