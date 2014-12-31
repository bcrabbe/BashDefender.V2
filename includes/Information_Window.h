//
//  Information_Window.h
//  Group_Project
//
//  Created by Michael on 10/11/2014.
//  Copyright (c) 2014 Michael. All rights reserved.
//

#ifndef _Information_Window_h
#define _Information_Window_h

/*---------- Standard Headers -----------*/
#include <stdbool.h>

/*---------- Incomplete Types -----------*/
typedef struct towerMonitor TowerMonitor;

/*---------- Data Types -----------*/
typedef struct towerMonitor TowerMonitor;
typedef struct terminalWindow TerminalWindow;

/*----------Function Prototypes (External)-----------*/
void updateAllInfoWindow(void);
char *textToTowerMonitor(char *string);
void displayTowerInfo(unsigned int targetTower);
char *errorToTerminalWindow(char *string);
char *commandToTerminalWindow(char *string);
TowerMonitor *getTowerMonitor(void);
TerminalWindow *getTerminalWindow(void);
/*----------Function Prototypes (Testing)------------*/
void testingInformationWindowModule(void);
void testParserToInfoWindow(void);

void tutorial_one();
void tutorial_two();
void tutorial_three();
void tutorial_four();
void tutorial_five();
void tutorial_five_error();
void tutorial_six();
void tutorial_seven();
void tutorial_eight();
void tutorial_nine();
void tutorialUpdateAllInfoWindow(void);
void tutorialTowerMonitor();
#endif
