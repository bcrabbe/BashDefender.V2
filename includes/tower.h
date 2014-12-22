#ifndef TEST_Header_h
#define TEST_Header_h

#include "../includes/Display.h"
#include "../includes/enemy.h"

typedef struct tower *tower;
typedef struct towerGroup *TowerGroup;
typedef struct towerPos *TowerPos;
typedef struct towerPosNode *TowerPosNode;
typedef struct projectileNode *ProjectileNode;
typedef struct projectileList *ProjectileList;


typedef enum firingMethod	{

	laser = 1,
	bullet = 2,
	missile = 3
	
} FiringMethod;

int isTowerPositionAvailable(int position);
void createTowerPos();
int getSpecifiedTowerPosY(int postion);
int getSpecifiedTowerPosX(int postion);
int maxTowerPosition();
char maxTowerPositionChar();

TowerPos getTowerPos(TowerPos tPos);
void addTowerPosNode(int x, int y);
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
int setTowerType(int towerID, int newType);
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
int getLastTowerPositionY();
int getLastTowerPositionX();
double scaleTowerPos(int coord, int scaleAxis, int scaleMax);
int getTowerDamage(int towerID);
void testingTowerModule();
void testGetTower();
void testUpgradeTowerStat();
void getStats(int *range, int *damage, int *speed, int *AOEpower, int *AOErange, unsigned int towerID);
int getNumOfTowerPositions();
void freeAllTowerPositions();
void testTowerCreation();
void testingTowerPositions();
int createTowerFromPositions(int position);
void drawAllTowerPositions();

void createProjectileList();
ProjectileList getProjectileList(ProjectileList pL);
ProjectileNode newProjectileNode();
void addToProjectileList(ProjectileNode newNode);
void removeProjectileNode(ProjectileNode projNode);
void moveProjectiles();
void drawProjectiles();

void launchBullet(int firedX, int firedY, int damage, int targetID, int firingType);
void moveBullet(ProjectileNode bullet);
void launchMissile(int firedX, int firedY, int damage, int targetID, int firingType);
void getBuildUpCoords(int originX, int originY, int *buildUpX, int *buildUpY);
void moveMissile(ProjectileNode missile);


#endif

