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
char *towerMonitor(void);
char *terminalWindow(void);
void statsBar(void);
void towerInformation(void);
void actionQueueMonitor(void);
TowerMonitor *getTowerMonitor(void);
char *getDefaultTowerString(TowerMonitor *tm);
char *getTowerString(unsigned int targetTower, TowerMonitor *tm);
TerminalWindow *getTerminalWindow(void);
CommandNode *createCommandNode(void);
void destroyCommandNode(CommandNode **start);

/*----------Function Prototypes (Testing)-----------*/
void testTowerMonitor(void);
void testTerminalWindow(void);


/**
 update tower monitor according to information in tower monitor object
 @param void
 @returns pointer to display string currently held in tower monitor
 */

void updateAllInfoWindow(void) {
    statsBar();
    towerMonitor();
    actionQueueMonitor();
    towerInformation();
    terminalWindow();
}

void tutorialUpdateAllInfoWindow(void) {
    statsBar();
	tutorialTowerMonitor();
    actionQueueMonitor();
    towerInformation();
    terminalWindow();
}

void tutorialTowerMonitor()	{
	TowerMonitor *tm = getTowerMonitor();
	updateTowerMonitor(tm->string);
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
            getDefaultTowerString(tm);
            break;
        case TOWER_INFO:
            getTowerString(tm->targetTower, tm);
            break;
        case OTHER_INFO:
            break;

    }
    
    //If another string has been set and a period of time has elapsed, reset to default
    if(tm->stringType != TOWER_DEFAULT && time - tm->timeSet > DEFAULT_TOWER_MONITOR_TIME) {
        tm->stringType = TOWER_DEFAULT;
    }
    
    updateTowerMonitor(tm->string);
    
    return tm->string;
}


/**
send string to tower monitor and display for set period of time
@param string to be displayed
@returns pointer to string to be displayed in tower monitor object
*/
char *textToTowerMonitor(char *string) {
    TowerMonitor *tm = getTowerMonitor();
    
    strcpy(tm->string, string);
    tm->stringType = OTHER_INFO;
    tm->timeSet = SDL_GetTicks();
    
    return tm->string;
}

/**
 alerts tower monitor that tower information has been requested, information for that
 tower will be displayed for set period of time
 @param the tower for which information is to be displayed
 @returns void
 */
void displayTowerInfo(unsigned int targetTower) {
    TowerMonitor *tm = getTowerMonitor();
    
    tm->targetTower = targetTower;
    tm->stringType = TOWER_INFO;
    tm->timeSet = SDL_GetTicks();
}


/**
 initialize tower monitor object when first called, return pointer to object each subsequent call
 @param void
 @returns the Tower Monitor object
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
 @param void
 @returns the display string currently stored in terminal window object
 */
char *terminalWindow(void) {
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

    
    //Send string to display if not empty
    if(strlen(tw->outputString) > 0) {
        updateTerminalWindow(tw->outputString);
    }
    
    return tw->outputString;
}

/**
 initialize terminal window object when first called, return pointer to object each subsequent call
 @param void
 @returns pointer to the terminal window object
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
 send error to terminal window object
 @param string to be sent to terminal window after formatting
 @returns void
 */
char *errorToTerminalWindow(char *string) {
    TerminalWindow *tw = getTerminalWindow();
    
    static char errorString[MAX_OUTPUT_STRING];
    sprintf(errorString, "******************************\n%s\n******************************", string);
    
    strcpy(tw->errorString, errorString);
    tw->stringType = ERROR_MESSAGE;
    tw->timeSet = SDL_GetTicks();
    
    return errorString;
}

/**
 send command to terminal window object
 @param the command to be added to terminal window
 @returns a pointer to the display string with last N commands stored in terminal window
 */
char *commandToTerminalWindow(char *string) {
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
    
    return tw->outputString;
}

/**
 Creates command node that will store command string
 @param void
 @returns pointer to the command node
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
 Deallocate memory for previously created command node and command string and relink list
 @param pointer to the command node pointer
 @returns void
 */
void destroyCommandNode(CommandNode **start) {
    CommandNode *temp = *start;
    *start = (*start)->next;
    free(temp->commandString);
    free(temp);

}

/**
 Creates output string for stats bar and updates it
 @param void
 @returns void
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
 @param void
 @returns void
 */
void actionQueueMonitor() {
    
    char *outputString = getActionQueueString();
    
    updateActionQueueMonitor(outputString);
}

/**
 Creates tower string for every drawn tower and displays it
 @param void
 @returns void
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
 Creates default tower monitor string and stores in tower monitor object
 @param Tower monitor object
 @returns Pointer to the created display string
 */
char *getDefaultTowerString(TowerMonitor *tm) {
    
    static char defaultTowerString[MAX_OUTPUT_STRING];
    
    sprintf(defaultTowerString, "TOWER MONITOR\n\nActive Towers: %d", getNumberOfTowers());
    strcpy(tm->string, defaultTowerString);
    
    return defaultTowerString;
}

/**
 Creates display string for specific tower and stores in tower monitor object
 @param Tower for which to create display string, tower monitor object
 @returns Pointer to the created display string
 */
char *getTowerString(unsigned int targetTower, TowerMonitor *tm) {
    
    static char towerString[MAX_OUTPUT_STRING];
    
    int range, damage, speed, AOEpower, AOErange;
    getStats(&range, &damage, &speed, &AOEpower, &AOErange, targetTower);


    sprintf(towerString, "TOWER %d\n\nRange: %d\nDamage: %d\nSpeed: %d\nAOE Power: %d\nAOE Range: %d", targetTower, range, damage, speed, AOEpower, AOErange);
    strcpy(tm->string, towerString);
    
    return towerString;
}

/**
 Updates everything in information window
 @param void
 @returns void
 */
void updateAllInfoWindow(void) {
    statsBar();
    towerMonitor();
    actionQueueMonitor();
    towerInformation();
    terminalWindow();
}

/*Test functions*/

/**
 test important function in information window
 @param void
 @returns void
 */
void testingInformationWindowModule()	{
    
    sput_start_testing();
    sput_set_output_stream(NULL);
    
    sput_enter_suite("testTowerMonitor");
    sput_run_test(testTowerMonitor);
    sput_leave_suite();
    
    sput_enter_suite("testTerminalWindow");
    sput_run_test(testTerminalWindow);
    sput_leave_suite();
    
    sput_finish_testing();
}

/**
 test if strings in tower monitor are being stored correctly
 @param void
 @returns void
 */
void testTowerMonitor(void) {
    TowerMonitor *tm= getTowerMonitor(); //initialize tower monitor
    
    createTowerGroup(); //Create tower group to test retrieving default tower string
    sput_fail_if(strcmp(getDefaultTowerString(tm), tm->string) != 0, "Testing default string");
    
    textToTowerMonitor("This is a test string");
    sput_fail_if(strcmp(tm->string, "This is a test string") != 0, "Testing random string");
    
    userCreateTower(200, 200); //Create random tower to test retrieving specific tower string.
    sput_fail_if(strcmp(getTowerString(1, tm), tm->string) != 0, "Testing specific tower string");
}


void tutorial_one()	{

	textToTowerMonitor("Hi!  Welcome to your first day as a Bash Defender.  \n Lets get started!\n");

}

void tutorial_two()	{

	textToTowerMonitor("lets try making a tower!\n please type:\n mktwr int a\n to make an integer tower in position a.  You can try another position if you like!\n\n This command looks like a lot like linux command 'mkdir' for making directories - but thats not too important right now.\n");

}

void tutorial_three()	{

		textToTowerMonitor("Well done!  You have made a tower of type integer.\n You should use this tower type against integer enemies - those are the ones with numbers.\n\n  In Computer Science, an integer is data type for storing whole numbers.\n");

}
void tutorial_four()	{
		textToTowerMonitor("I know what you are thinking, why would you need to build towers when its so calm and peaceful round here?\n Well, its not always like this.  In your job as a linux administrator, you will be under constant attack from virus's.\n.  In fact, it looks like there is one on its way right now!\n");

}

void tutorial_five()	{

	textToTowerMonitor("Oh no!  It looks like that enemy got away.  We need to prepare before the next enemy comes.\n  Upgrade your tower's damage by typing\n upgrade p t1");

}

void tutorial_six()	{

	textToTowerMonitor("Well Done!  Now your tower will do more damage against those pesky virus'\n.  Check it out, here comes another one!\n");

}

void tutorial_five_error()	{

	textToTowerMonitor("Thats not the correct stat to upgrade.\n  Please upgrade power by typing \n upgrade p t1\n");
}

void tutorial_seven()	{

	textToTowerMonitor("Well That was weird.\n Why didn't your upgraded tower kill that enemy?  Its because your tower was the wrong type!  That enemy was a char enemy - You can tell because it was red.  \n\nYou should also note that your health is going down with each enemy to break through your defenses (top right corner!).  Let this get to zero and your lose!");

}

void tutorial_eight()	{

		textToTowerMonitor("Right!  No more losing.  Lets create a char tower with mktwr char c\n  Char is data type in computer science for storing characters");

}

void tutorial_nine()	{

		textToTowerMonitor("Bam!  that char virus didn't stand a chance.  Lets look at some other upgrade commands before we finish up.  \n");

}
/*Test functions*/

/**
 test if strings in terminal window are being stored correctly
 @param void
 @returns void
 */
void testTerminalWindow(void) {
    TerminalWindow *tw = getTerminalWindow();
    
    sput_fail_if(strcmp(errorToTerminalWindow("This is a test string"), tw->errorString) != 0, "Testing error string");
    
    commandToTerminalWindow("A random command");
    sput_fail_if(strcmp(tw->start->commandString, "A random command") != 0, "Testing sending a command");
    commandToTerminalWindow("Another random command");
    sput_fail_if(strcmp(tw->start->next->commandString, "Another random command") != 0, "Testing sending another command");
}

