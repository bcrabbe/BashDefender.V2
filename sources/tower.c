#include <stdio.h>
#include <stdlib.h>

#include "../includes/tower.h"
#include "../includes/sput.h"
#include "../includes/debug.h"

struct tower {
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
    int width;
    int gunX;
    int gunY;
};

struct towerPos	{

	int numberOfPositions;
	TowerPosNode *towerPositions;

};

struct towerPosNode	{
	tPosIcon tIcon;
	int x;
	int y;

};

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
		newTower->x = x;
		newTower->y = y;
		newTower->tIcon = tPos->numberOfPositions;
		tPos->towerPositions[tPos->numberOfPositions] = newTower;
}

void drawAllTowerPositions()	{

	TowerPos tPos = getTowerPos(NULL);
	int t;
	for(t = 1; t <= tPos->numberOfPositions;t++)	{
		drawTowerPosition(tPos->towerPositions[t]->x,tPos->towerPositions[t]->y,50,50,tPos->towerPositions[t]->tIcon);
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

void testingTowerPositions()	{

	sput_start_testing();
	sput_set_output_stream(NULL);

	sput_enter_suite("testTowerCreation():  Checking they exist in group once created");
	sput_run_test(testTowerCreation);
	sput_leave_suite();

	sput_finish_testing();	

}

void testTowerCreation()	{

	addTowerPosNode(100,200);
	sput_fail_unless(getSpecifiedTowerPosX(1) == 100, "Tower position 1 x coord is 100");
	sput_fail_unless(getSpecifiedTowerPosY(1) == 200, "Tower position 1 y coord is 200");
	addTowerPosNode(300,500);
	sput_fail_unless(getSpecifiedTowerPosX(2) == 300, "Tower position 2 x coord is 300");
	sput_fail_unless(getSpecifiedTowerPosY(2) == 500, "Tower position 2 y coord is 500");
}

int getSpecifiedTowerPosX(int postion)	{

	TowerPos tPos = getTowerPos(NULL);
	return tPos->towerPositions[postion]->x;
}

int getSpecifiedTowerPosY(int postion)	{

	TowerPos tPos = getTowerPos(NULL);
	return tPos->towerPositions[postion]->y;
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

	sput_enter_suite("testGetTower(): Tower creation and being placed in tower array");
	sput_run_test(testGetTower);
	sput_leave_suite();
	
	sput_enter_suite("testUpgradeTowerStat(): Upgrading tower stats");
	sput_run_test(testUpgradeTowerStat);
	sput_leave_suite();

	sput_finish_testing();

}


struct towerGroup	{

	tower *listOfTowers;
	unsigned int numOfTowers;
	
};

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
void createTowerFromPositions(int position)	{
	TowerPos tPos = getTowerPos(NULL);
	printf("test\n");
	iprint(tPos->towerPositions[position]->x);
	userCreateTower(tPos->towerPositions[position]->x,tPos->towerPositions[position]->y);

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

    newTow->damage = 3;
    newTow->range = 200;
    newTow->firing = 0;
	  newTow->level = 1;
    newTow->speed = 10;
    newTow->AOEpower = 10;
    newTow->AOErange = 10;
    newTow->height = 80;
    newTow->width = 80;
    newTow->gunX = 40;
    newTow->gunY = 20;
    
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
		return upgradeT->damage++;
	}
	return 0;
}
int upgradeRange(int target)
{
	
	tower upgradeT;
	if((upgradeT = getTowerID(target))!= NULL)	{
		return upgradeT->range++;
	}
	return 0;
}
int upgradeSpeed(int target)
{
	
	tower upgradeT;
	if((upgradeT = getTowerID(target))!= NULL)	{
		return upgradeT->speed++;
	}
	return 0;
}
int upgradeAOEpower(int target)
{
	
	tower upgradeT;
	if((upgradeT = getTowerID(target))!= NULL)	{
		return upgradeT->AOEpower++;
	}
	return 0;
}
int upgradeAOErange(int target)
{
	
	tower upgradeT;
	if((upgradeT = getTowerID(target))!= NULL)	{
		return upgradeT->AOErange++;
	}
	return 0;
}

//upgradeStat upgradeTowerStat(upgradeStat stat, int target)	{
//
//	switch(stat)	{
//		case power:
//        {
//			if(upgradeDmg(target))	{
//				return power;
//			}
//        }
//		case range:
//        {
//			if(upgradeRange(target))	{
//				return range;
//			}
//        }
//		case speed:
//        {
//			if(upgradeSpeed(target))	{
//				return speed;
//			}
//        }
//		case AOErange:
//        {
//			if(upgradeAOErange(target))	{
//				return AOErange;
//			}
//        }
//		case AOEpower:
//        {
//			if(upgradeAOEpower(target))	{
//				return AOEpower;
//			}
//        }
//		default:
//			fprintf(stderr,"upgradeTowerStat tower.c: unrecognised stat\n");
//            return statError;
//
//	}
//}


void testUpgradeTowerStat()	{

	createLevelPaths();
	createTowerGroup();
	tower t1 = createTower();
	tower t2 = createTower();
	//sput_fail_unless(upgradeTowerStat(power,1) == power,"Valid Power Upgrade: tower one for upgrade");
	//sput_fail_unless(upgradeTowerStat(power,1) == power,"Valid Power Upgrade: Tower two for upgrade");
	//sput_fail_unless(upgradeTowerStat(0,1) == 0, " Invalid: Passing invalid upgrade");
	//sput_fail_unless(upgradeTowerStat(power,3) == 0, " Invalid: Passing invalid target");
	free(t1);
	free(t2);
}

unsigned int getNumberOfTowers()	{
	return ((getTowerGrp(NULL))->numOfTowers);
}


void freeAllTowers()	{

	int i = 1;
	while(i < getTowerGrp(NULL)->numOfTowers)	{
		free(getTowerGrp(NULL)->listOfTowers[i]);
		i++;
	}
}

void testGetTower()	{

	createLevelPaths();
	createTowerGroup();
	createTower();
	sput_fail_unless(getNumberOfTowers() == 1, "Valid: Number of towers held in group is one.");
	sput_fail_unless(getTowerID(1) != NULL,"Valid: Tower with ID 1 exists.");
	createTower();
	sput_fail_unless(getNumberOfTowers() == 2, "Valid: Number of towers held in group is two");
	sput_fail_unless(getTowerID(2) != NULL,"Valid: Tower with ID 2 exists.");
	freeAllTowers();
	free(getTowerGrp(NULL));
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

void getStats(int *range, int *damage, int *speed, int *AOEpower, int *AOErange, unsigned int towerID)
{
    TowerGroup towers = getTowerGrp(NULL);
     *range = towers->listOfTowers[towerID]->range;
     *damage = towers->listOfTowers[towerID]->damage;
     *speed = towers->listOfTowers[towerID]->speed;
     *AOEpower = towers->listOfTowers[towerID]->AOEpower;
     *AOErange = towers->listOfTowers[towerID]->AOErange;
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
            towerGetTargetPos(currentTower->targetPosition, currentTower->targetID);
            damageEnemy(currentTower->damage, currentTower->targetID);
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
            if(TG->listOfTowers[towerID]->firing == 1)	{
                drawLine(d, currentTower->x+currentTower->gunX,
                         currentTower->y+currentTower->gunY, currentTower->targetPosition[0],
                         currentTower->targetPosition[1]);
            }
            // 80s for tow width and height - these are constant for now.
        }
    }
}
