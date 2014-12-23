//
//  Information_Window.c
//  Group_Project
//
//  Functions relating to game windows and displaying information on screen
//
//  Created by Michael on 10/11/2014.
//  Copyright (c) 2014 Michael. All rights reserved.
//

/*---------- Standard Headers -----------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*---------- Custom Headers	-----------*/
#include "../includes/Information_Window.h"
#include "../includes/tower.h"
#include "../includes/gameProperties.h"
#include "../includes/actionQueueDataStructure.h"
#include "../includes/Display.h"
#include "../includes/sput.h"

/*---------- Data Types -----------*/
typedef enum stringType {DEFAULT, TOWER_INFO, OTHER_INFO} StringType;

typedef struct towerMonitor {
    char *string;
    StringType st;
    int timeSet;
    int targetTower;
} TowerMonitor;

/*---------- Hash Defines -----------*/
#define MAX_OUTPUT_STRING 200
#define DEFAULT_TOWER_MONITOR_TIME 10000
#define DEFAULT_TERMINAL_WINDOW_TIME 4000

/*----------Function Prototypes (Internal)-----------*/
TowerMonitor *getTowerMonitor(void);
char *getDefaultTowerString();
char *getTowerString(unsigned int targetTower);





/**
 Update tower monitor according to information in tower monitor object
 */
void towerMonitor(void) {
    TowerMonitor *tm = getTowerMonitor();
    int time = SDL_GetTicks();
    
    //Set output string accordingly
    switch (tm->st) {
        case DEFAULT:
            tm->string = getDefaultTowerString();
            break;
        case TOWER_INFO:
            tm->string = getTowerString(tm->targetTower);
            break;
        case OTHER_INFO:
            break;
    }
    
    //If another string has been set and a period of time has elapsed, reset to default
    if(tm->st != DEFAULT && time - tm->timeSet > DEFAULT_TOWER_MONITOR_TIME) {
        tm->st = DEFAULT;
    }
    
    updateTowerMonitor(tm->string);
}

/**
 Sends any string to tower monitor and displays for set period of time
 */
void textToTowerMonitor(char *string) {
    TowerMonitor *tm = getTowerMonitor();
    
    strcpy(tm->string, string);
    tm->st = OTHER_INFO;
    tm->timeSet = SDL_GetTicks();
    
}

/**
 Alerts tower monitor that tower information has been requested, information for that tower will be displayed for set period of
 time
 */
void displayTowerInfo(unsigned int targetTower) {
    TowerMonitor *tm = getTowerMonitor();
    
    tm->targetTower = targetTower;
    tm->st = TOWER_INFO;
    tm->timeSet = SDL_GetTicks();
}


/**
 Initialize tower monitor object when first called, return pointer to object each subsequent call
 */
TowerMonitor *getTowerMonitor(void) {
    static TowerMonitor *tm;
    static bool initialized = false;
    
    if(!initialized) {
        tm = (TowerMonitor *) malloc(sizeof(TowerMonitor));
        
        tm->string = (char *) malloc(sizeof(char) * MAX_OUTPUT_STRING);
        tm->timeSet = 0;
        tm->st = DEFAULT;
        
        initialized = true;
    }
    
    return tm;
}

/**
 update terminal window with optional output string
 */
void terminalWindow(char *string) {
    int time = SDL_GetTicks();
    static int timeOfCall = 0;
    static char *outputString = NULL;
    
    if(string != NULL) {
        outputString = malloc(MAX_OUTPUT_STRING);
        sprintf(outputString, "\n\n\n               **********\n%s\n               **********\n\n\n", string);
        timeOfCall = time;
    }
    
    if(time - timeOfCall > DEFAULT_TERMINAL_WINDOW_TIME) {
        free(outputString);
        outputString = NULL;
    }
    
    updateTerminalWindow(outputString);
}

/**
 Creates output string for stats monitor and updates stats monitor
 */
void statsBar() {
    
    GameProperties properties = getGame(NULL);
    
    int mem = getAvailableMemory(properties);
    int waveNumber = getWave(properties);
    int totalWaves = getTotalWaves(properties);
    int health = getHealth(properties);
    
    char *outputString = malloc(MAX_OUTPUT_STRING);
    
    sprintf(outputString, "Available Memory: %dbytes                                                 Wave: %d / %d                                                                     Health: %d", mem, waveNumber, totalWaves, health);
    
    updateStatsBar(outputString);
}

/**
Creates output string for action queue monitor and updates it
*/
void actionQueueMonitor() {
    
    char *outputString = getActionQueueString();
    
    updateActionQueueMonitor(outputString);
}

/**
 Creates tower string for every drawn tower and displays it
 */
void towerInformation() {
    
    int numOfTowers = getNumOfTowers();
    
    if(numOfTowers > 0) {
        for(int towerID = 1; towerID <= numOfTowers; ++towerID) {
            char towerString[10];
            sprintf(towerString, "Tower %d", towerID);
            
            int towerX = getTowerX(towerID);
            int towerY = getTowerY(towerID);
            
            updateTowerInformation(towerX, towerY, towerString, towerID);
        }
    }
    
}

/**
 Creates default string for tower monitor
 */
char *getDefaultTowerString() {
    
    char *outputString = malloc(MAX_OUTPUT_STRING);
    
    sprintf(outputString, "TOWER MONITOR\n\nActive Towers: %d", getNumberOfTowers());
    
    return outputString;
}

/**
 Creates output string for specific tower
 */
char *getTowerString(unsigned int targetTower) {
    
    int range, damage, speed, AOEpower, AOErange;
    getStats(&range, &damage, &speed, &AOEpower, &AOErange, targetTower);
    
    char *outputString = malloc(MAX_OUTPUT_STRING);
    
    sprintf(outputString, "TOWER %d\n\nRange: %d\nDamage: %d\nSpeed: %d\nAOE Power: %d\nAOE Range: %d", targetTower, range, damage, speed, AOEpower, AOErange);
    
    return outputString;
}


/**
 Sends "upgrade" command help string to tower monitor
 */
void manUpgrade()
{
    textToTowerMonitor("GENERAL COMMANDS MANUAL: \n\nupgrade\n\nType ""upgrade"" followed by a stat\n( p, r, s, AOEp, AOEr)\nfollowed by a target tower\ne.g. t1, t2, t3...\nExamples:\nupgrade r t2\nupgrade p t3");
}

/**
 Sends "cat" command help string to tower monitor
 */
void manCat()
{
    textToTowerMonitor("GENERAL COMMANDS MANUAL: \n\ncat \n\ntype ""cat"" followed by a target, e.g. t1, t2, t3..., to display the stats of that target\n");
}

/**
 Sends "man" command help string to tower monitor
 */
void manMan()
{
    textToTowerMonitor("GENERAL COMMANDS MANUAL: \n\nman \n\ntype ""man"" followed by a command, e.g. upgrade or cat, to view the manual\nentry for that command\n");
}

/**
 Sends "manPs" command help string to tower monitor
 */
void manPs()
{
    textToTowerMonitor("GENERAL COMMANDS MANUAL: \n\nps\n\ntype ""ps"" followed by a command\n ( -x\n ) to discover information about one or more enemies\nExamples:\nps -x\n");
}

/**
 Sends "manKill" command help string to tower monitor
 */
void manKill()
{
    textToTowerMonitor("GENERAL COMMANDS MANUAL: \n\nps\n\ntype ""kill -9"" followed by a target enemyID (eg 6) or *all*\n to kill one or more enemies\nExamples:\nkill -9 7\n kill -9 all");
}


/*Test functions*/

/**
 Tests all functions in information window module
 */
void testingInformationWindowModule()	{
    sput_start_testing();
    sput_set_output_stream(NULL);
    
    //sput_enter_suite("");
    //sput_run_test();
    sput_leave_suite();
    
    sput_finish_testing();
    
}

