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

/*----------Function Prototypes (External)-----------*/
void updateAllInfoWindow(void);
void textToTowerMonitor(char *string);
void displayTowerInfo(unsigned int targetTower);
void errorToTerminalWindow(char *string);
void commandToTerminalWindow(char *string);
void manUpgrade(void);
void manCat(void);
void manMan(void);
void manPs(void);
void manKill(void);

void testingInformationWindowModule(void);

#endif
