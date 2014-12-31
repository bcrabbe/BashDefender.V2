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

typedef struct  environmentVariable {
    char * name;
    char * name2;
    int value;
    int (*getValueFunc)();
} envVar;

typedef struct environmentVariableList {
    envVar ** array;
    int numberOfElements;
} envVarList;

typedef struct targetArray {
    int * array;
    int numberOfElements;
} targetArrayStruct;

typedef struct statArray {
    cmdOption * array;
    int numberOfElements;
} statArrayStruct;

typedef struct upgradeArrays {
    statArrayStruct * statArray;
    targetArrayStruct * tarArray;
} upgradeArraysStruct;

int parse(char *inputString);

char * strdup(const char * s);
void freeCommandArray(char **commandArray,int numberOfChunks);
char ** breakUpString(const char * inputString, int *numberOfChunksPtr, const char * delimiter);
void testCommandArray(char ** commandArray, int numberOfChunks);
void testGetAction(enum cmdType action);
void testGetUpgradeStat(cmdOption statToUpgrade);

void optionUsageError();
void actionUsageError();

unsigned int getTargetEnemy(const char * inputStringTargeting);
cmdOption getCommandOption(char * input);
cmdType getCommandType(char * firstToken );


void destroyEnvVarList();
void freeParseLists();
void initialiseParseLists();
stringList * getCommandList(stringList * commandList);
stringList * getOptionList(stringList * optionList);
envVarList * getEnvsList(envVarList * envsList);
stringList * intialiseCommandList();
stringList * intialiseOptionList();
envVarList * intialiseEnvVarsList();

void testStringLists();

int parseCat(char * inputStringTargeting);
int parseMan(char * inputStringCommandMan);
int parseAptget(char * aptToGetString);
int parseCommands(char ** commandArray, int numberOfTokens);

int parsePs(char * optionString);
int parseAptget(char * aptToGetString);
int parseKill(char ** commandArray,int numberOfChunks);
int parseUpgrade(char ** commandArray, int numberOfChunks);



#endif


