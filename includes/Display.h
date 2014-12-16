//
//  Display.h
//  TEST
//
//  Created by Vlad Vyshnevskyy on 19/11/2014.
//  Copyright (c) 2014 VV-SD. All rights reserved.
//

int SCREEN_WIDTH_GLOBAL;
int SCREEN_HEIGHT_GLOBAL;

#ifndef TEST_Display_h
#define TEST_Display_h

#define SCREEN_WIDTH    800
#define SCREEN_HEIGHT   700

//#define FULLSCREEN_YN SDL_WINDOW_FULLSCREEN
#define FULLSCREEN_YN 0

//Tower monitor dimensions
#define TOWER_MONITOR_HEIGHT (SCREEN_HEIGHT_GLOBAL / 3.5)
#define TOWER_MONITOR_WIDTH (SCREEN_WIDTH_GLOBAL / 3.5)
#define TOWER_MONITOR_X (SCREEN_WIDTH_GLOBAL - TOWER_MONITOR_WIDTH)
#define TOWER_MONITOR_Y (SCREEN_HEIGHT_GLOBAL - TOWER_MONITOR_HEIGHT)
#define TOWER_TEXT_BORDER_X 25
#define TOWER_TEXT_BORDER_Y 25

//Stats monitor dimensions
#define STATS_BAR_X 0
#define STATS_BAR_Y 0
#define STATS_BAR_WIDTH (SCREEN_WIDTH)
#define STATS_BAR_HEIGHT 30
#define STATS_BAR_BORDER_X 300
#define STATS_BAR_BORDER_Y 10

//Action queue dimensions
#define ACTION_QUEUE_X 0
#define ACTION_QUEUE_Y (STATS_BAR_HEIGHT)
#define ACTION_QUEUE_WIDTH (SCREEN_WIDTH_GLOBAL / 5)
#define ACTION_QUEUE_HEIGHT (SCREEN_HEIGHT_GLOBAL / 5)
#define ACTION_QUEUE_BORDER_X 25
#define ACTION_QUEUE_BORDER_Y 20

//Terminal monitor dimensions
#define TERMINAL_WINDOW_HEIGHT (SCREEN_HEIGHT_GLOBAL / 3.5)
#define TERMINAL_WINDOW_WIDTH (SCREEN_WIDTH_GLOBAL / 3.5)
#define TERMINAL_WINDOW_X (0)
#define TERMINAL_WINDOW_Y (SCREEN_HEIGHT_GLOBAL - TERMINAL_WINDOW_HEIGHT)

//Map Dimensions

#define MAP_X 	0	
#define MAP_Y	0
#define MAP_WIDTH SCREEN_WIDTH_GLOBAL
#define MAP_HEIGHT	(SCREEN_HEIGHT_GLOBAL - TOWER_MONITOR_HEIGHT)


#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include <stdlib.h>
#include <string.h>

typedef struct display *Display;
int processEvents(Display d);


Display init_SDL();

Display getDisplayPointer(Display d);


void startFrame(Display d);
void endFrame(Display d);
void shutSDL(Display d);



//tower
void drawTower(Display d, int x, int y, int w, int h, int range, int type);
void drawLine(Display d, int X_from, int Y_from, int X_target, int Y_target);
void drawTowerPosition(int x, int y, int w, int h);

//enemy

void drawEnemy(Display d, int x, int y, int w, int h, int pic_width, int pic_height, double currentHealth, double maxHealth, int type, int frames, int anim_speed);

void presentAnimation();
void drawBackground();


//check
void check_load_images(SDL_Surface *surface, char *pic_name);

TTF_Font *getInfoWindowFont(TTF_Font *font);


#include <stdio.h>
#include "Information_Window.h"

void displayTowerMonitor();
void displayStatsBar();
void displayActionQueueMonitor();
void updateTowerMonitor(char *outputString);
void updateStatsBar(char *outputString);
void updateActionQueueMonitor(char *outputString);
char *strdup2(char * s);


int terminal_window(Display d, char *pass, char *clear);
void display_text(Display d, char *pass);
void menu_screen(Display d, int *started);

int getBackgroundWidth();
int getBackgroundHeight();

#endif

// for my purpose...
//void present_tower(Display d){
//    static int done = 0;
//    if (!done) {
//        init_tower(d, "tower.png");
//        done = 1;
//    }
//    drawTower(d, 80, 100);
//}
