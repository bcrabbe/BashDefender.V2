#ifndef TEST_Header_h
#define TEST_Header_h

#include "../includes/Display.h"
#include "../includes/enemy.h"
//#include "../includes/gameProperties.h"
//#include "../includes/actionQueueDataStructure.h"

typedef struct tower *tower;
typedef struct towerGroup *TowerGroup;

tower createTower();
void initialiseNewTower(tower newTow, int TowerPositionX, int TowerPositionY );
int userCreateTower(int inputTowerPositionX, int inputTowerPositionY);
int getTowerRange(int towerID);
int getTowerSpeed(int towerID);
int getTowerAOErange(int towerID);
int getTowerAOEpower(int towerID);
int getTowerLevel(int towerID);
int upgradeDmg(int target);
int upgradeRange(int target);
int upgradeSpeed(int target);
int upgradeAOEpower(int target);
int upgradeAOErange(int target);
void createTowerGroup();
TowerGroup getTowerGrp();
tower getTowerID(int target);
tower getTowerPointer(tower updatedT);
void populateTower(tower newTow, int id);
int getRange();
int getTowerX(int towerID);
int getTowerY(int towerID);
int isFiring();
int firingX();
int firingY();
void freeTower(tower t);
void fire();
void printTower(tower t);
unsigned int getNumberOfTowers();
void freeAllTowers();
void present_tower(Display d);
int setTowerX(int towerID,int newX);
int setTowerY(int towerID, int newY);
int setTowerRange(int towerID, int newRange);
int setTowerDamage(int towerID, int newDamage);
int getTowerDamage(int towerID);
void testingTowerModule();
void testGetTower();
void testUpgradeTowerStat();
void getStats(int *range, int *damage, int *speed, int *AOEpower, int *AOErange, unsigned int towerID);
#endif

