//
//  Information_Window.c
//  Group_Project
//
//  Functions relating to tower monitor and stats monitor
//
//  Created by Michael on 10/11/2014.
//  Copyright (c) 2014 Michael. All rights reserved.
//

#include "../includes/Information_Window.h"
#include "../includes/tower.h"
#include "../includes/gameProperties.h"
#include "../includes/actionQueueDataStructure.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_OUTPUT_STRING 200
#define DEFAULT_SCREEN_TIME 10000
#define DEFAULT_SCREEN_TIME2 4000

#include <SDL2/SDL.h>

/**
 If called with target tower as first parameter and second parameter set to -1, gets output string
 for that tower and sends to tower monitor. If called with first parameter set to NULL and optional
 output string as second parameter, sends that string to tower monitor. After a certain period
 of time, default tower screen reappears.
 */
void towerMonitor(unsigned int targetTower, char *optionalOutputString) {
    int time = SDL_GetTicks();
    static int lastTower = 0, timeOfCall = 0, optionalStringSet = 0;
    char *outputString;
    static char *optionalString = NULL;
    
    if(targetTower && !optionalOutputString) {
        outputString = getTowerString(targetTower);
        timeOfCall = SDL_GetTicks();
        lastTower = targetTower;
        optionalStringSet = 0;
    }
    else if(optionalOutputString) {
        outputString = optionalString = optionalOutputString;
        timeOfCall = SDL_GetTicks();
        optionalStringSet = 1;
    }
    else {
        outputString = getDefaultTowerString();
    }
    
    if(timeOfCall != 0 && time - timeOfCall < DEFAULT_SCREEN_TIME) {
        if(optionalStringSet) {
            outputString = optionalString;
        }
        else {
            outputString = getTowerString(lastTower);
        }
    }
    
    if(time - timeOfCall > DEFAULT_SCREEN_TIME) {
        optionalStringSet = 0;
    }
    
    updateTowerMonitor(outputString);
}

/**
 update terminal window with optional output string
 */
/*void terminalWindow(char *string) {
    int time = SDL_GetTicks();
    static int timeOfCall = 0;
    static char *outputString = NULL;
    
    if(string != NULL) {
        outputString = malloc(MAX_OUTPUT_STRING);
        sprintf(outputString, "\n\n\n               **********\n%s\n               **********\n\n\n", string);
        timeOfCall = time;
    }
    
    if(time - timeOfCall > DEFAULT_SCREEN_TIME2) {
        outputString = NULL;
    }
    
    updateTerminalWindow(outputString);
}*/

/**
 Creates output string for stats monitor and updates stats monitor
 */
void statsBar() {
    
    GameProperties properties = getGame(NULL);
    
    int mem = getAvailableMemory(properties);
    int waveNumber = getWave(properties);
    int health = getHealth(properties);
    
    char *outputString = malloc(MAX_OUTPUT_STRING);
    
    sprintf(outputString, "Available Memory: %dbytes                                                 Wave Number: %d                                                                     Health: %d", mem, waveNumber, health);
    
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
 Creates default string for tower monitor and sends to tower monitor
 */
char *getDefaultTowerString() {
    
    char *outputString = malloc(MAX_OUTPUT_STRING);
    
    sprintf(outputString, "TOWER MONITOR\n\nActive Towers: %d", getNumberOfTowers());
    
    return outputString;
}

/**
 Creates output string for specific tower and sends to tower monitor
 */
char *getTowerString(unsigned int targetTower) {
    
    int range, damage, speed, AOEpower, AOErange;
    getStats(&range, &damage, &speed, &AOEpower, &AOErange, targetTower);
    
    char *outputString = malloc(MAX_OUTPUT_STRING);
    
    sprintf(outputString, "TOWER %d\n\nRange: %d\nDamage: %d\nAOE Speed: %d\nAOE Power: %d\nAOE Range: %d", targetTower, range, damage, speed, AOEpower, AOErange);
    
    return outputString;
}


/**
 Sends "upgrade" command help string to tower monitor
 */
void manUpgrade()
{
    towerMonitor(-1, "GENERAL COMMANDS MANUAL: \n\nupgrade\n\ntype ""upgrade"" followed by a stat\n ( p, r, s, AOEp, AOEr)\n ) followed by a target tower\neg t1, t2, t3...\nExamples:\nupgrade r t2\nupgrade p t3");
}

/**
 Sends "cat" command help string to tower monitor
 */
void manCat()
{
    towerMonitor(-1, "GENERAL COMMANDS MANUAL: \n\ncat \n\ntype ""cat"" followed by a target eg t1, t2, t3... to display the stats of that target\n");
}

/**
 Sends "man" command help string to tower monitor
 */
void manMan()
{
    towerMonitor(-1, "GENERAL COMMANDS MANUAL: \n\nman \n\ntype ""man"" followed by a command eg upgrade or cat to view the manual entry for that command\n");
}

void manPs()
{
    towerMonitor(-1, "GENERAL COMMANDS MANUAL: \n\nps\n\ntype ""ps"" followed by a command\n ( -x\n ) to discover information about one or more enemies\nExamples:\nps -x\n");
}

void manKill()
{
    towerMonitor(-1, "GENERAL COMMANDS MANUAL: \n\nps\n\ntype ""kill -9"" followed by a target enemyID (eg 6) or *all*\n to kill one or more enemies\nExamples:\nkill -9 7\n kill -9 all");
}
