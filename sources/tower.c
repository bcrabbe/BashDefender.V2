#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "../includes/tower.h"
#include "../includes/sput.h"
#include "../includes/debug.h"

#define MAX_COOLDOWN 100 //  the longest number of ticks that a tower can take between shots

//bullet #defines
#define BULLET_TO_TARGET 20 // the number of steps for a bullet to reach its target
#define BULLET_SIZE 10 // the height and width of the bullet image

//missile #defines
#define BUILDUP_DISTANCE 100 // the distance the missiles travel during their buildup stage
#define BUILDUP_STEPS 50 // the number of steps it takes for a missile to reach the end of its buildup stage
#define MISSILE_BUILDUP_DIVISION 20 // the fraction (e.g 1/20th, 1/30th) that the missile moves towards its buildup position every step
#define MISSILE_TO_TARGET 7 // the number of steps it takes for a missile to reach its target after buildup
#define MISSILE_STARTING_SIZE 3 // the starting height and width of missile projectiles
#define MISSILE_ENDING_SIZE 15 // the size of missiles once they have finished their buildup stage

//laser #defines
#define LASER_DRAW_COUNT 20 // the number of steps that laser beams stay on the screen

//#defines for tower type weighting
#define DAMAGE_MOD 7
#define SPEED_MOD 5
#define RANGE_MOD 2

//#defines for upgrade amounts
#define DAMAGE_UPGR_VAL 5
#define SPEED_UPGR_VAL 5
#define RANGE_UPGR_VAL 10

#define UPGRADES_PER_LEVEL 5 // the number of upgrades that can be done to a tower before its level increases

struct tower {
    int towerType;
    int towerID;
    int x, y;
    int damage;
    int range;
    int speed;
    int AOErange; //! not yet implemented
    int AOEpower; //!not yet implemented
    int targetID;
    int firing;
    int targetPosition[2];
    
    int level;
    int height;
    int upgradesCompleted;
    int width;
    
    FiringMethod firingType;
    int gunX;
    int gunY;
    
    int firingCoolDown;
};

struct towerGroup	{

	tower *listOfTowers;
	unsigned int numOfTowers;
	
};

struct towerPos	{

	int numberOfPositions;
	TowerPosNode *towerPositions;

};

struct towerPosNode	{
	tPosIcon tIcon;
	int x;
	int y;
	BOOL empty; 	// True is empty, false is full 

};

struct projectileNode {
  int x, y;
  int h, w;
  int originX, originY;
  int buildUpX, buildUpY;
  int centreX, centreY;
  
  int damageType;
  FiringMethod whatProjectile;
  
  int targetCoords[2];
  int targetID;
  
  int damage;
  int aoeDamage;
  int aoeRange;
  
  int movesMade;
  int movesToTarget;
  int movesForBuildUp;
  int buildUpH, buildUpW;
  
  int drawLaserCount;
  int drawLaserMaxCount;
  
  
  ProjectileNode next;
};

struct projectileList {
  ProjectileNode start, current, last;
} ;


ProjectileNode newProjectileNode()
{
  ProjectileNode newNode = (ProjectileNode)malloc(sizeof(struct projectileNode) );
  if(newNode == NULL) {
    fprintf(stderr,"****ERROR unable to malloc space for projectile node ****\n");
    exit(1);
  }
  
  newNode->next = NULL;
  
  return newNode;
}

void fireLaser(int gunX, int gunY, int damage, int targetID, int firingType)
{
  
  ProjectileNode newNode = newProjectileNode();
  newNode->whatProjectile = laser;
  
  newNode->damage = damage;
  newNode->damageType = firingType;
  
  newNode->movesToTarget = 5; // added some moves to target to draw line slightly in front of enemy. Visual effect, unused elsewhere
  newNode->drawLaserCount = 0;
  newNode->drawLaserMaxCount = LASER_DRAW_COUNT;
  
  newNode->originX = gunX;
  newNode->originY = gunY;
  
  newNode->targetID = targetID;
  getBulletTargetPos(targetID, newNode->targetCoords, newNode->movesToTarget);
  
  newNode->aoeDamage = 0;
  newNode->aoeRange = 0;
  
    // add it to the list
  addToProjectileList(newNode);
}
  

/*
* creates a new bullet projectile and launches it at where the target will be
*/
void launchBullet(int firedX, int firedY, int damage, int targetID, int firingType)
{
  // make the bullet
  ProjectileNode newNode = newProjectileNode();
  
  newNode->movesMade = 0;
  newNode->movesToTarget = BULLET_TO_TARGET;
  
  newNode->whatProjectile = bullet;
  newNode->damageType = firingType;
  newNode->damage = damage;
  newNode->h = BULLET_SIZE;
  newNode->w = BULLET_SIZE;
  
  newNode->x = firedX-(newNode->w/2);
  newNode->y = firedY-(newNode->h/2);
  newNode->originX = newNode->x;
  newNode->originY = newNode->y;
  
  newNode->aoeDamage = 0;
  newNode->aoeRange = 0;
  
  newNode->targetID = targetID;
  getBulletTargetPos(targetID, newNode->targetCoords, newNode->movesToTarget);
  newNode->targetCoords[0] = newNode->targetCoords[0] - (newNode->w/2);
  newNode->targetCoords[1] = newNode->targetCoords[1] - (newNode->h/2);
  
    // add it to the list
  addToProjectileList(newNode);
  
}

void launchMissile(int firedX, int firedY, int damage, int targetID, int firingType)
{
  // make the missile
  ProjectileNode newNode = newProjectileNode();
  newNode->whatProjectile = missile;
  
  newNode->movesMade = 0;
  newNode->movesForBuildUp = BUILDUP_STEPS;
  newNode->movesToTarget = MISSILE_TO_TARGET;
  
  newNode->h = MISSILE_STARTING_SIZE;
  newNode->w = MISSILE_STARTING_SIZE;
  
  newNode->centreX = firedX;
  newNode->centreY = firedY;
  newNode->x = firedX-(newNode->w/2);
  newNode->y = firedY-(newNode->h/2);
  newNode->originX = firedX;
  newNode->originY = firedY;
  
  newNode->damage = damage;
  newNode->aoeDamage = 0;
  newNode->aoeRange = 0;
  
  newNode->targetID = targetID;
  getBulletTargetPos(targetID, newNode->targetCoords, newNode->movesToTarget+newNode->movesForBuildUp);
  
  
  getBuildUpCoords(newNode->originX, newNode->originY, &newNode->buildUpX, &newNode->buildUpY);
  
    // add it to the list
  addToProjectileList(newNode);
    
}

/*
*  sets the destination of the missile's build up phase
*/
void getBuildUpCoords(int firedX, int firedY, int *buildUpX, int *buildUpY)
{
  
  int randChecker = 0; //counter to check for loop in while statement below
  int x, y, xAdjust, yAdjust;
  
    // set x and y to be random value between -10 & 10
  while( (x = (rand()%21) - 10) == 0 || (y = (rand()%21) - 10) == 0) {
    randChecker++;
    if(randChecker > 1000) {
      fprintf(stderr,"****ERROR missile has looped more than 1000 times trying to find a missile coordinate (tower.c) ****\n");
      exit(1);
    }
  }
  
  double calcX, calcY;
  if(x < 0) {
    calcX = (double)(-x);
  } else {
    calcX = (double) x;
  }
  
  if(y < 0) {
    calcY = (double)(-y);
  } else {
    calcY = (double) y;
  }
  
    // get a build up target that is a set number of pixels away
  double angle = atan(calcY/calcX);
  
  calcY = BUILDUP_DISTANCE * sin(angle);
  calcX = BUILDUP_DISTANCE * cos(angle);
  
  // set x & y to adjusted values
  
    if(x < 0) {
    xAdjust = (int)(-calcX);
  } else {
    xAdjust = (int)calcX;
  }
  
  if(y < 0) {
    yAdjust = (int)(-calcY);
  } else {
    yAdjust = (int)calcY;
  }
  
  // set the build up coordinates based on the calculated values and starting coordinates
    *buildUpX = firedX+xAdjust;
    *buildUpY = firedY+yAdjust;
  
}

void addToProjectileList(ProjectileNode newNode)
{
  ProjectileList pL = getProjectileList(NULL);
  
  if(pL->start == NULL) {
    pL->start = newNode;
    pL->current = pL->start;
    pL->last = pL->start;
  } else {
    pL->last->next = newNode;
    pL->last = newNode;
  }
}

void moveMissile(ProjectileNode missile) {

  missile->movesMade++;
  if(missile->movesMade == missile->movesToTarget+missile->movesForBuildUp) {
    
    missile->x = missile->targetCoords[0] - missile->w;
    missile->y = missile->targetCoords[1] - missile->h;
    damageEnemy(missile->damage, missile->targetID, missile->damageType);
    
    removeProjectileNode(missile);
  } else {
    if(missile->movesMade <= missile->movesForBuildUp) {
      missile->centreX = missile->originX + (int) ((double)(missile->buildUpX-missile->originX)/(double)MISSILE_BUILDUP_DIVISION);
      missile->centreY = missile->originY + (int) ((double)(missile->buildUpY-missile->originY)/(double)MISSILE_BUILDUP_DIVISION);
      missile->originX = missile->centreX;
      missile->originY = missile->centreY;
      
      missile->h = MISSILE_STARTING_SIZE + (int)( (double)(MISSILE_ENDING_SIZE-MISSILE_STARTING_SIZE) * ( (double)missile->movesMade/(double)missile->movesForBuildUp) );
      missile->w = MISSILE_STARTING_SIZE + (int)( (double)(MISSILE_ENDING_SIZE-MISSILE_STARTING_SIZE) * ( (double)missile->movesMade/(double)missile->movesForBuildUp) );
      
      missile->x = missile->centreX-(missile->w/2);
      missile->y = missile->centreY-(missile->h/2);
    } else {
        missile->x = missile->originX + (int) ( ((double)(missile->targetCoords[0]-missile->originX)/(double) (missile->movesToTarget)) * (missile->movesMade-missile->movesForBuildUp));
        missile->y = missile->originY + (int) ( ((double)(missile->targetCoords[1]-missile->originY)/(double) (missile->movesToTarget)) * (missile->movesMade-missile->movesForBuildUp));
    }
  }
}
      
  
void moveBullet(ProjectileNode bullet) {
  
  bullet->movesMade++;
  if(bullet->movesMade == bullet->movesToTarget) {
    
    bullet->x = bullet->targetCoords[0];
    bullet->y = bullet->targetCoords[1];
    damageEnemy(bullet->damage, bullet->targetID, bullet->damageType);
    
    removeProjectileNode(bullet);
  } else {
    
    bullet->x = bullet->originX + (int) ( ((double)(bullet->targetCoords[0]-bullet->originX)/(double) bullet->movesToTarget) * bullet->movesMade);
    bullet->y = bullet->originY + (int) ( ((double)(bullet->targetCoords[1]-bullet->originY)/(double) bullet->movesToTarget) * bullet->movesMade);
  }
}

void updateLaser(ProjectileNode laser) {

  if(laser->drawLaserCount == 0) {
    damageEnemy(laser->damage, laser->targetID, laser->damageType);
  }
  
  laser->drawLaserCount++;
  
  if (laser->drawLaserCount == laser->drawLaserMaxCount) {
    removeProjectileNode(laser);
  }
  
}
    

void removeProjectileNode(ProjectileNode projNode) {

  ProjectileList pL = getProjectileList(NULL);
  ProjectileNode prev;
  
  
  if(projNode == pL->start) {
    pL->start = projNode->next;
    free(projNode);
  } else {
    pL->current = pL->start->next;
    prev = pL->start;
    
    while(pL->current != projNode) {
      prev = pL->current;
      pL->current = pL->current->next;
    }
    
    if(pL->current == pL->last) {
      pL->last = prev;
      free(pL->current);
      pL->last->next = NULL;
    } else {
      prev->next = pL->current->next;
      free(pL->current);
    }
  }
}
    
      
  
void moveProjectiles() {
  int finished = 0;
  ProjectileList pL = getProjectileList(NULL);
  pL->current = pL->start;
  if(pL->current != NULL) {
    while(!finished) {
      switch(pL->current->whatProjectile) {
        case missile :
          moveMissile(pL->current); 
          break;
        case bullet :
          moveBullet(pL->current);
          break;
        case laser :
          updateLaser(pL->current);
          break;
      }
      if(pL->current->next == NULL) {
        finished = 1;
      } else {
        pL->current = pL->current->next;
      }
    }
  }
}

void drawProjectiles() {
  
  ProjectileList pL = getProjectileList(NULL);
  if(pL->start!=NULL) {
    pL->current = pL->start;
    int finished = 0;
    Display d = getDisplayPointer(NULL);
    while(!finished) {
      switch(pL->current->whatProjectile) {
        case missile :
          drawBullet(pL->current->x, pL->current->y, pL->current->w, pL->current->h);
          break;
        case bullet :
          drawBullet(pL->current->x, pL->current->y, pL->current->w, pL->current->h);
          break;
        case laser :
          drawLine(d, pL->current->originX, pL->current->originY, pL->current->targetCoords[0], pL->current->targetCoords[1]);
      }
      if(pL->current->next == NULL) {
        finished = 1;
      } else {
        pL->current = pL->current->next;
      }
    }
  }
}
    
      
  

void createProjectileList() {

  ProjectileList newProjList = (ProjectileList)malloc(sizeof(struct projectileList) );
  if(newProjList == NULL) {
    fprintf(stderr,"****ERROR unable to malloc space for projectile list ****\n");
    exit(1);
  }
  
  newProjList->start = NULL;
  newProjList->current = NULL;
  newProjList->last = NULL;
  
  getProjectileList(newProjList);
  
  
}

ProjectileList getProjectileList(ProjectileList pL)	{

	static ProjectileList newProjList;

	if(pL != NULL)	{
		newProjList = pL;
	}

	return newProjList;
}


/*
 * Creates structure holding array of allowed tower positions
 */
void createTowerPos()	{

	TowerPos newPositions = (TowerPos) malloc(sizeof(*newPositions));
	newPositions->numberOfPositions = 0;
	newPositions->towerPositions = malloc(sizeof(TowerPosNode));
	getTowerPos(newPositions);
}

/*
 *Returns structure holding allowed tower positions
 */
TowerPos getTowerPos(TowerPos tPos)	{

	static TowerPos currTPos;

	if(tPos != NULL)	{
		currTPos = tPos;
	}

	return currTPos;
}

/*
 *Add a new tower position
 */
void addTowerPosNode(int x, int y)	{
		TowerPos tPos = getTowerPos(NULL);
		tPos->numberOfPositions++;
		tPos->towerPositions = (TowerPosNode*) realloc(tPos->towerPositions, (tPos->numberOfPositions+1)*(sizeof(*(tPos->towerPositions))));
		TowerPosNode newTower = (TowerPosNode) malloc(sizeof(*newTower));
		newTower->empty = TRUE; //! Tower position is available
		newTower->x = (int) scaleTowerPos(x,SCREEN_WIDTH_GLOBAL,MAX_TOWER_X);
		newTower->y = (int) scaleTowerPos(y,SCREEN_HEIGHT_GLOBAL,MAX_TOWER_Y);
		newTower->tIcon = tPos->numberOfPositions;
		tPos->towerPositions[tPos->numberOfPositions] = newTower;
}

double scaleTowerPos(int coord, int scaleAxis, int scaleMax)	{
	return ((double) coord * ((double) scaleAxis/ (double) scaleMax) );
}

void drawAllTowerPositions()	{

	TowerPos tPos = getTowerPos(NULL);
	int t;
	for(t = 1; t <= tPos->numberOfPositions;t++)	{
		if(tPos->towerPositions[t]->empty == TRUE)	{
			drawTowerPosition(tPos->towerPositions[t]->x,tPos->towerPositions[t]->y,50,50,tPos->towerPositions[t]->tIcon);
		}
	}

}
void freeAllTowerPositions()	{
	
	TowerPos tPos = getTowerPos(NULL);
	int positions;
	for(positions = 1; positions <= tPos->numberOfPositions; positions++){
		free(tPos->towerPositions[positions]);
	}

	free(tPos);

}

int getNumOfTowerPositions()	{
	
	return getTowerPos(NULL)->numberOfPositions;

}

void testingTowerPositions()	{
    
    sput_start_testing();
	sput_set_output_stream(stderr);

	sput_enter_suite("testTowerCreation():  Checking they exist in group once created");
	sput_run_test(testTowerCreation);
	sput_leave_suite();

	sput_finish_testing();	

}

void testTowerCreation()	{
	
	addTowerPosNode(100,200);
	sput_fail_unless(getLastTowerPositionX() == (int) scaleTowerPos(100,SCREEN_WIDTH_GLOBAL,MAX_TOWER_X), "Newly Added Tower position x coord is correct");
	sput_fail_unless(getLastTowerPositionY() == (int) scaleTowerPos(200,SCREEN_HEIGHT_GLOBAL,MAX_TOWER_Y), "Newly Added Tower position y coord is correct");
}

int getSpecifiedTowerPosX(int postion)	{
	TowerPos tPos = getTowerPos(NULL);
	return tPos->towerPositions[postion]->x;
}

int getSpecifiedTowerPosY(int postion)	{
	TowerPos tPos = getTowerPos(NULL);
	return tPos->towerPositions[postion]->y;
}

int getLastTowerPositionY()	{
	return getSpecifiedTowerPosY(getTowerPos(NULL)->numberOfPositions);
}

int getLastTowerPositionX()	{
	return getSpecifiedTowerPosX(getTowerPos(NULL)->numberOfPositions);
}

int getTowerRange(int towerID)	{
	return getTowerID(towerID)->range;     
}
int getTowerSpeed(int towerID)	{
	return getTowerID(towerID)->speed;     
}
int getTowerDamage(int towerID)	{
	return getTowerID(towerID)->damage;     
}
int getTowerAOErange(int towerID)	{
	return getTowerID(towerID)->level;     
}
int getTowerAOEpower(int towerID)	{
	return getTowerID(towerID)->AOEpower;     
}
int getTowerLevel(int towerID)	{
	return getTowerID(towerID)->level;     
}

void testingTowerModule()	{
	sput_start_testing();
	sput_set_output_stream(NULL);	

	sput_enter_suite("testGetTower(): Tower creation at valid positions and being placed in tower array");
	sput_run_test(testGetTower);
	sput_leave_suite();

	sput_finish_testing();

}


void createTowerGroup()	{

	TowerGroup Group = (TowerGroup) malloc(sizeof(*Group));
	getTowerGrp(Group);
	Group->listOfTowers=malloc(sizeof(tower));
	Group->numOfTowers = 0;
}
/* original create tower may be decomissioned in future
    returns the number of towers if succesful 
    returns 0 if something goes wrong
 */
tower createTower() {
    getTowerGrp(NULL)->numOfTowers++; //!increased number of towers when one is created
    getTowerGrp(NULL)->listOfTowers = realloc(getTowerGrp(NULL)->listOfTowers, (getTowerGrp(NULL)->numOfTowers+1)*sizeof(tower));
    if(getTowerGrp(NULL)->listOfTowers==NULL)
    {
        fprintf(stderr,"ERROR: createTower() \n towergroup array realloc failed\n");
        return 0;
    }
    tower t = malloc(sizeof(*t));
    if(t==NULL)
    {
        fprintf(stderr,"ERROR: createTower()\n tower  malloc failed\n");
        return 0;
    }
    getTowerGrp(NULL)->listOfTowers[getTowerGrp(NULL)->numOfTowers] = t;
    
    getTowerPointer(t);  //! Should no longer be used.  Functions should ID tower that they wish to target.
    
    populateTower(t,getTowerGrp(NULL)->numOfTowers); //! populating tower stats
	

    return t;
	//return getTowerGrp(NULL)->numOfTowers;
    
}

/*
 *Wrapper to pass in allowed tower positions to useCreateTower function
 */
int createTowerFromPositions(int position)	{
	TowerPos tPos = getTowerPos(NULL);
	if((position > 0) && (position <= tPos->numberOfPositions) && (tPos->towerPositions[position]->empty == TRUE))	{
		userCreateTower(tPos->towerPositions[position]->x,tPos->towerPositions[position]->y);
		tPos->towerPositions[position]->empty = FALSE;
		return 1;
	}
	return 0;
}

void createTowerTypeFromPositions(int position, int tType)	{
	iprint(tType);
	TowerGroup TG = getTowerGrp(NULL);
	createTowerFromPositions(position);
	TG->listOfTowers[TG->numOfTowers]->towerType = tType;
}

char maxTowerPositionChar()
{
    TowerPos tPos = getTowerPos(NULL);
    char positionChar = tPos->numberOfPositions + 'a' - 1;
    return toupper(positionChar);
}
int maxTowerPosition() {
    TowerPos tPos = getTowerPos(NULL);
    return  tPos->numberOfPositions;
}

int isTowerPositionAvailable(int position)	{
	TowerPos tPos = getTowerPos(NULL);
    if( position <= tPos->numberOfPositions ) {
        return tPos->towerPositions[position]->empty;
    }
    else {
        return 0;
    }
}

/*
* changes the type of the tower (int/char) to the specified type. Returns 1 if successful, 0 if tower ID doesn't exist.
*/
int setTowerType(int towerID, int newType) {

  tower t;
  if((t = getTowerID(towerID)) == NULL) {
    return 0;
  }
  else {
    t->towerType = newType;
    return 1;
  }
}
  

/* called when create tower command input by player. Places a tower at the specified x y.
    returns total number of towers if succesful
    returns 0 if failled
 */
int userCreateTower(int inputTowerPositionX, int inputTowerPositionY)
{
    TowerGroup TG = getTowerGrp(NULL);

    TG->numOfTowers++; //!increased number of towers when one is created
    TG->listOfTowers = realloc(TG->listOfTowers, (TG->numOfTowers+1)*sizeof(tower));
    if(TG->listOfTowers==NULL)
    {
        fprintf(stderr,"ERROR: createTower() \n towergroup array realloc failed\n");
        return 0;
    }
    tower t = malloc(sizeof(*t));
    if(t==NULL)
    {
        fprintf(stderr,"ERROR: createTower()\n tower  malloc failed\n");
        return 0;
    }
    TG->listOfTowers[TG->numOfTowers] = t;
    initialiseNewTower(t, inputTowerPositionX, inputTowerPositionY);
    
    
    return TG->numOfTowers;
    
}



void initialiseNewTower(tower newTow, int TowerPositionX, int TowerPositionY )
{
    TowerGroup TG = getTowerGrp(NULL);

    newTow->towerID = TG->numOfTowers;//new tower ID is the same as the number of towers in the group
    newTow->x = TowerPositionX;
    newTow->y = TowerPositionY;
    newTow->towerType = INT_TYPE;

    newTow->upgradesCompleted = 0;

    newTow->damage = 20;
    newTow->range = 100;
    newTow->firing = 0;
	  newTow->level = 1;
    newTow->speed = 50;
    newTow->AOEpower = 10;
    newTow->AOErange = 10;
    newTow->height = 80;
    newTow->width = 80;
    newTow->gunX = 40;
    newTow->gunY = 20;
    newTow->firingCoolDown = 0;
    assignCalculatedFiringType(newTow->towerID);
    
}

void assignCalculatedFiringType(int towerID) {

  tower t = getTowerGrp(NULL)->listOfTowers[towerID];
  
  if((t->damage * DAMAGE_MOD) > (t->speed * SPEED_MOD) && (t->damage * DAMAGE_MOD) > (t->range * RANGE_MOD) ) {
    t->firingType = bullet;
  } else {
    if ((t->speed * SPEED_MOD) > (t->damage * DAMAGE_MOD) && (t->speed * SPEED_MOD) > (t->range * RANGE_MOD) ) {
      t->firingType = laser;
    } else {
      t->firingType = missile;
    }
  }
}

void makePostUpgradeChanges(int TowerID) {
  
  tower t = getTowerGrp(NULL)->listOfTowers[TowerID];
  
  assignCalculatedFiringType(TowerID);
  
  t->upgradesCompleted++;
  if(t->upgradesCompleted % UPGRADES_PER_LEVEL == 0) {
    t->level++;
  }
}
      

/*
 * Must be called and created before towers are created.
 */
TowerGroup getTowerGrp(TowerGroup Group)	{

	static TowerGroup newGroup;

	if(Group != NULL)	{
		newGroup = Group;
	}

	return newGroup;
}

int upgradeDmg(int target)
{
	
	tower upgradeT;
	if((upgradeT = getTowerID(target))!= NULL)	{
		upgradeT->damage+=DAMAGE_UPGR_VAL;
    makePostUpgradeChanges(target);
    return upgradeT->damage;
	}
	return 0;
}
int upgradeRange(int target)
{
	
	tower upgradeT;
	if((upgradeT = getTowerID(target))!= NULL)	{
		upgradeT->range+=RANGE_UPGR_VAL;
    makePostUpgradeChanges(target);
    return upgradeT->range;
	}
	return 0;
}
int upgradeSpeed(int target)
{
	
	tower upgradeT;
	if((upgradeT = getTowerID(target))!= NULL)	{
		upgradeT->speed+=SPEED_UPGR_VAL;
    makePostUpgradeChanges(target);
    return upgradeT->speed;
	}
	return 0;
}
int upgradeAOEpower(int target)
{
	
	tower upgradeT;
	if((upgradeT = getTowerID(target))!= NULL)	{
		upgradeT->AOEpower++;
    makePostUpgradeChanges(target);
    return upgradeT->AOEpower;
	}
	return 0;
}
int upgradeAOErange(int target)
{
	
	tower upgradeT;
	if((upgradeT = getTowerID(target))!= NULL)	{
		upgradeT->AOErange++;
    makePostUpgradeChanges(target);
    return upgradeT->AOErange;
	}
	return 0;
}



unsigned int getNumberOfTowers()	{
	return ((getTowerGrp(NULL))->numOfTowers);
}

int checkCharType()	{
	int i = 1;
	while(i <= getTowerGrp(NULL)->numOfTowers)	{
		if(getTowerGrp(NULL)->listOfTowers[i]->towerType == CHAR_TYPE) {
			return 1;
		}
		i++;
	}
	return 0;
}
void freeAllTowers()	{

	int i = 1;
	while(i <= getTowerGrp(NULL)->numOfTowers)	{
		free(getTowerGrp(NULL)->listOfTowers[i]);
		i++;
	}
	if(getTowerGrp(NULL)->numOfTowers != 0)	{
		getTowerGrp(NULL)->numOfTowers -=i;
	}
}

void testGetTower()	{

    freeAllTowers();
	createTowerFromPositions(1);
	sput_fail_unless(getNumberOfTowers() == 1, "Valid: Number of towers held in group is one.");
	sput_fail_unless(getTowerID(1) != NULL,"Valid: Tower with ID 1 exists.");
	createTowerFromPositions(2);
	sput_fail_unless(getNumberOfTowers() == 2, "Valid: Number of towers held in group is two");
	sput_fail_unless(getTowerID(2) != NULL,"Valid: Tower with ID 2 exists.");
	freeAllTowers();
	sput_fail_unless(getNumberOfTowers() == 0,"valid: All towers have been removed.");
	sput_fail_unless(createTowerFromPositions(99) == 0, "Invalid: no tower position 99");
	sput_fail_unless(createTowerFromPositions(0) == 0, "Invalid: no tower position 0");
	sput_fail_unless(getNumberOfTowers() == 0,"valid: No Towers: previous attempts were invalid");
}

tower getTowerPointer(tower updatedT) {
    
    static tower t;
    
    if(updatedT != NULL) {
        t = updatedT;
    }
    
    return t;
}

/*
 *Returns tower based on ID
 */
tower getTowerID(int target)	{
	int i;
	for( i = 1; i <= (getTowerGrp(NULL))->numOfTowers; i++)	{
		if((getTowerGrp(NULL))->listOfTowers[i]->towerID == target)	{
				return getTowerGrp(NULL)->listOfTowers[i];
		}
	}

	return NULL;
}



void populateTower(tower newTow, int id) {
   	
    newTow->towerID = id;
    newTow->x = MAP_WIDTH/2;
    newTow->y = (MAP_HEIGHT/2)+10;
    newTow->damage = 10;
    newTow->range = 10;
    newTow->firing = 0;
    newTow->level = 1;
    newTow->speed = 50;


}

void getStats(int *towerType, int *range, int *damage, int *speed, int *AOEpower, int *AOErange, unsigned int towerID)
{
    TowerGroup towers = getTowerGrp(NULL);
     *range = towers->listOfTowers[towerID]->range;
     *damage = towers->listOfTowers[towerID]->damage;
     *speed = towers->listOfTowers[towerID]->speed;
     *AOEpower = towers->listOfTowers[towerID]->AOEpower;
     *AOErange = towers->listOfTowers[towerID]->AOErange;
     *towerType = towers->listOfTowers[towerID]->towerType;
}

int getTowerX(int towerID)
{
    TowerGroup TG = getTowerGrp(NULL);
    return TG->listOfTowers[towerID]->x;
}

int getTowerY(int towerID)
{
    TowerGroup TG = getTowerGrp(NULL);
    return TG->listOfTowers[towerID]->y;
}

int getTowerWidth(int towerID) {
    TowerGroup TG = getTowerGrp(NULL);
    return TG->listOfTowers[towerID]->width;
}


int setTowerY(int towerID, int newY)	{

	getTowerGrp(NULL)->listOfTowers[towerID]->y = newY;
	return newY;
	
}

int setTowerRange(int towerID, int newRange)	{

	getTowerGrp(NULL)->listOfTowers[towerID]->range = newRange;
	return newRange;

}

int setTowerDamage(int towerID, int newDamage)	{

	getTowerGrp(NULL)->listOfTowers[towerID]->damage = newDamage;
	return newDamage;

}

int setTowerX(int towerID,int newX)	{

	getTowerGrp(NULL)->listOfTowers[towerID]->x = newX;
	return newX;

}

int isFiring() {
    
    tower t = getTowerPointer(NULL);
    
    return t->firing;
}

int firingX() {
    tower t = getTowerPointer(NULL);

    return t->targetPosition[0];

}

int firingY() {
    tower t = getTowerPointer(NULL);
    
    return t->targetPosition[1];
}


void freeTower(tower t) {
  free(t);
}

void fire() {
  
	int enemyID, towerID;

	for(towerID = 1; towerID <= getNumberOfTowers(); towerID++)	{
	  tower currentTower = getTowerID(towerID);
	  currentTower->firing = 0;
	  
	  // check cooldown to see if tower is ready to fire
	  if(currentTower->firingCoolDown > 0) {
	    currentTower->firingCoolDown--;
	  } else {
	        // pick a target
		    for(enemyID = 1; enemyID <= getNumberOfEnemies(); enemyID++)	{
			    if (!isDead(enemyID) ) {
			      if(inRange(currentTower->x + (currentTower->width/2),
                             currentTower->y + (currentTower->height/2),
                             currentTower->range, enemyID) == 1) {
			          
                      // if first enemy encountered, fire at it
                      if(currentTower->firing == 0) {
                          currentTower->firing = 1;
                          currentTower->targetID = enemyID;
                      }
                      // else, compare with current target to choose closest to end of path
                      else {
                          if(distanceToEndOfPath(enemyID) <
                             distanceToEndOfPath(currentTower->targetID) ) {
                              currentTower->targetID = enemyID;
                          }
                      }
                  }
                }
            }

          // now target is assigned, shoot!
            if(currentTower->firing == 1) {
                currentTower->firingCoolDown = MAX_COOLDOWN - currentTower->speed;
                towerGetTargetPos(currentTower->targetPosition, currentTower->targetID);
                
                switch (currentTower->firingType) {
                  case laser :
                    fireLaser(currentTower->x+currentTower->gunX, currentTower->y+currentTower->gunY, currentTower->damage, currentTower->targetID, currentTower->towerType);
                    break;
                  case missile :
                    launchMissile(currentTower->x+currentTower->gunX, currentTower->y+currentTower->gunY, currentTower->damage, currentTower->targetID, currentTower->towerType);
                    break;
                  case bullet :
                    launchBullet(currentTower->x+currentTower->gunX, currentTower->y+currentTower->gunY, currentTower->damage, currentTower->targetID, currentTower->towerType);
                    break;
                }
            }
        }
    }
}

  

void printTower(tower t) {

    printf("tower x = %d, tower y = %d, tower firing = %d", t->x, t->y, t->firing);
    if(t->firing) {
        printf(" Tower firing coords x = %d, tower firing coords y = %d ",
               t->targetPosition[0], t->targetPosition[1]);
    }
    else {
        printf(" ");
    }
}

void present_tower(Display d)
{
    TowerGroup TG = getTowerGrp(NULL);
    if(TG->numOfTowers>0)
    {
        for(int towerID=1; towerID<=TG->numOfTowers; ++towerID)
        {
            tower currentTower = getTowerID(towerID);
            drawTower(d, currentTower->x, currentTower->y, currentTower->width,
                      currentTower->height,currentTower->range, 0);
        }
    }
    // bullets added here temporarily
    moveProjectiles();
    drawProjectiles();
}

int getNumOfTowers(void) {
    TowerGroup TG = getTowerGrp(NULL);
    
    return TG->numOfTowers;
}
