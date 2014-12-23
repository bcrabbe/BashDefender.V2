//
//  Information_Window.h
//  Group_Project
//
//  Created by Michael on 10/11/2014.
//  Copyright (c) 2014 Michael. All rights reserved.
//

#ifndef _Information_Window_h
#define _Information_Window_h

#include "../includes/Display.h"

void towerMonitor(unsigned int targetTower, char *outputString);
char *getDefaultTowerString();
char *getTowerString(unsigned int targetTower);
void terminalWindow(char *string);
void statsBar();
void actionQueueMonitor();
void manUpgrade();
void manCat();
void manMan();
void manPs();
void manKill();

#endif
