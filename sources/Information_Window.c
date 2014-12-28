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
typedef enum towerMonitorString {TOWER_DEFAULT, TOWER_INFO, OTHER_INFO} TowerMonitorString;
typedef enum terminalWindowString {TERMINAL_DEFAULT, ERROR_MESSAGE} TerminalWindowString;

struct towerMonitor {
    char *string;
    TowerMonitorString stringType;
    int timeSet;
    int targetTower;
};

typedef struct commandNode {
    char *commandString;
    struct commandNode *next;
} CommandNode;

typedef struct terminalWindow {
    CommandNode *start;
    TerminalWindowString stringType;
    int commands;
    char *outputString;
    char *errorString;
    int timeSet;
} TerminalWindow;

/*---------- Hash Defines -----------*/
#define MAX_OUTPUT_STRING 200
#define TOTAL_COMMANDS_DISPLAYED 5
#define DEFAULT_TOWER_MONITOR_TIME 10000
#define TERMINAL_ERROR_TIME 5000

/*----------Function Prototypes (Internal)-----------*/
void towerMonitor(void);
void terminalWindow(void);
void statsBar(void);
void towerInformation(void);
void actionQueueMonitor(void);
TowerMonitor *getTowerMonitor(void);
void getDefaultTowerString(char **inputString);
void getTowerString(unsigned int targetTower, char **inputString);
TerminalWindow *getTerminalWindow(void);
CommandNode *createCommandNode(void);
void destroyCommandNode(CommandNode **start);




/**
 Updates all information windows and bars
 */
void updateAllInfoWindow(void) {
    statsBar();
    towerMonitor();
    actionQueueMonitor();
    towerInformation();
    terminalWindow();
}

/**
 Update tower monitor according to information in tower monitor object
 */
void towerMonitor(void) {
    TowerMonitor *tm = getTowerMonitor();
    int time = SDL_GetTicks();
    
    //Set output string accordingly
    switch (tm->stringType) {
        case TOWER_DEFAULT:
            getDefaultTowerString(&tm->string);
            break;
        case TOWER_INFO:
            getTowerString(tm->targetTower, &tm->string);
            break;
        case OTHER_INFO:
            break;
    }
    
    //If another string has been set and a period of time has elapsed, reset to default
    if(tm->stringType != TOWER_DEFAULT && time - tm->timeSet > DEFAULT_TOWER_MONITOR_TIME) {
        tm->stringType = TOWER_DEFAULT;
    }
    
    updateTowerMonitor(tm->string);
}

/**
 Sends any string to tower monitor and displays for set period of time
 */
void textToTowerMonitor(char *string) {
    TowerMonitor *tm = getTowerMonitor();
    
    strcpy(tm->string, string);
    tm->stringType = OTHER_INFO;
    tm->timeSet = SDL_GetTicks();
    
}

/**
 Alerts tower monitor that tower information has been requested, information for that tower will be displayed for set period of
 time
 */
void displayTowerInfo(unsigned int targetTower) {
    TowerMonitor *tm = getTowerMonitor();
    
    tm->targetTower = targetTower;
    tm->stringType = TOWER_INFO;
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
        tm->stringType = TOWER_DEFAULT;
        
        initialized = true;
    }
    
    return tm;
}

/**
 update terminal window according to information in terminal window object
 */
void terminalWindow() {
    TerminalWindow *tw = getTerminalWindow();
    int time = SDL_GetTicks();
    
    strcpy(tw->outputString, "");
    
    switch(tw->stringType) {
        case TERMINAL_DEFAULT:
            for(CommandNode *start = tw->start; start != NULL; start = start->next) {
                strcat(tw->outputString, "$ ");
                strcat(tw->outputString, start->commandString);
                strcat(tw->outputString, "\n");
            }
            break;
        case ERROR_MESSAGE:
            strcpy(tw->outputString, tw->errorString);
            
            //Reset to default terminal string after period of time has elapsed
            if(time - tw->timeSet > TERMINAL_ERROR_TIME) {
                tw->stringType = TERMINAL_DEFAULT;
            }
            break;
    }

    if(strlen(tw->outputString) > 0) {
        updateTerminalWindow(tw->outputString);
    }
}

/**
Initialize terminal window object when first called, return pointer to object each subsequent call
*/
TerminalWindow *getTerminalWindow(void) {
    static TerminalWindow *tw;
    static bool initialized = false;
    
    if(!initialized) {
        tw = (TerminalWindow *) malloc(sizeof(TerminalWindow));
        tw->start = NULL;
        tw->commands = 0;
        tw->outputString = (char *) malloc(sizeof(char) * MAX_OUTPUT_STRING);
        tw->errorString = (char *) malloc(sizeof(char) * MAX_OUTPUT_STRING);
        tw->stringType = TERMINAL_DEFAULT;
        tw->timeSet = 0;
        
        initialized = true;
    }
    
    return tw;
}

/**
 Send error to terminal window object
 */
void errorToTerminalWindow(char *string) {
    TerminalWindow *tw = getTerminalWindow();
    
    sprintf(tw->errorString, "******************************\n%s\n******************************", string);
    tw->stringType = ERROR_MESSAGE;
    tw->timeSet = SDL_GetTicks();
}

/**
 Send command to terminal window object
 */
void commandToTerminalWindow(char *string) {
    TerminalWindow *tw = getTerminalWindow();
    
    //Create command node and add command string to it
    CommandNode *newNode = createCommandNode();
    strcpy(newNode->commandString, string);
    newNode->next = NULL;
    
    //Find last node in list and add new command node
    if(tw->start == NULL) {
        tw->start = newNode;
    }
    else {
        CommandNode *temp = tw->start;
        
        while(temp->next != NULL) {
            temp = temp->next;
        }
        
        temp->next = newNode;
    }
    
    tw->commands++;
    
    //Destroy first command and relink list if too many commands
    if(tw->commands > TOTAL_COMMANDS_DISPLAYED) {
        destroyCommandNode(&tw->start);
    }
}

/**
 Creates command node for inserting into list
*/
CommandNode *createCommandNode(void) {
    
    CommandNode *commandNode = (CommandNode*) malloc(sizeof(CommandNode));
    if(commandNode == NULL) {
        fprintf(stderr, "malloc failed in createCommandNode()");
    }
    commandNode->commandString = (char *) malloc(sizeof(char) * MAX_OUTPUT_STRING);
    if(commandNode->commandString == NULL) {
        fprintf(stderr, "malloc failed in createCommandNode()");
    }
    
    return commandNode;
    
}

/**
 Deallocate memory for previously created command node and command string
 */
void destroyCommandNode(CommandNode **start) {
    CommandNode *temp = *start;
    *start = (*start)->next;
    free(temp->commandString);
    free(temp);
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
void getDefaultTowerString(char **inputString) {
    
    sprintf(*inputString, "TOWER MONITOR\n\nActive Towers: %d", getNumberOfTowers());
    
}

/**
 Creates output string for specific tower
 */
void getTowerString(unsigned int targetTower, char **inputString) {
    
    int range, damage, speed, AOEpower, AOErange;
    getStats(&range, &damage, &speed, &AOEpower, &AOErange, targetTower);
    

    sprintf(*inputString, "TOWER %d\n\nRange: %d\nDamage: %d\nSpeed: %d\nAOE Power: %d\nAOE Range: %d", targetTower, range, damage, speed, AOEpower, AOErange);
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
 Tests functions in information window module
 */
void testingInformationWindowModule()	{
    sput_start_testing();
    sput_set_output_stream(NULL);
    
    //sput_enter_suite("");
    //sput_run_test();
    sput_leave_suite();
    
    sput_finish_testing();
    
}

