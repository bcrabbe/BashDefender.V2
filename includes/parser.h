//
//  parser.h
//  
//
//  Created by ben on 07/11/2014.
//
//

#ifndef _parser_h
#define _parser_h

#include "../includes/actionQueueDataStructure.h"

#define ENABLE_TESTING 0
typedef struct stringList {
    char ** stringArray;
    int numberOfStrings;
} stringList;

cmdType getCommandType(char * firstToken );
char * strdup(const char * s);
int parse(char *inputString);
void freeCommandArray(char **commandArray,int numberOfChunks);
char ** breakUpString(const char * inputString, int *numberOfChunksPtr, const char * delimiter);
void testCommandArray(char ** commandArray, int numberOfChunks);
void testGetAction(enum cmdType action);
void actionUsageError();
cmdOption getCommandOption(char * input);
int parseUpgrade(char ** commandArray, int numberOfChunks);
void testGetUpgradeStat(cmdOption statToUpgrade);
void optionUsageError();
int parseKill(char ** commandArray,int numberOfChunks);
unsigned int getTargetEnemy(const char * inputStringTargeting);
int parsePs(char * optionString);
int parseAptget(char * aptToGetString);
void freeParseLists();
void initialiseParseLists();
stringList * getCommandList(stringList * commandList);
stringList *  getOptionList(stringList * optionList);
stringList * intialiseCommandList();
stringList * intialiseOptionList();
void testStringLists();

unsigned int getTargetTower(const char * inputStringTargeting);
int parseCat(char * inputStringTargeting);
int parseMan(char * inputStringCommandMan);
int parseMktwr(char ** commandArray);
int parseAptget(char * aptToGetString);



#endif


