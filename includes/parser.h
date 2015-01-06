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


typedef struct stringList {
    char ** stringArray;
    int numberOfStrings;
} stringList;

typedef struct  environmentVariable {
    char * name;
    char * name2;
    int value;
    int (*updateValueFunc)(cmdType command);
    int (*getValueFunc)();
} envVar;

typedef struct environmentVariableList {
    envVar ** array;
    int numberOfElements;
} envVarList;


int parse(char *inputString);

//initialiser:
void initialiseParser();
//destructors:
void freeParseLists();
void destroyEnvVarList();

//unit test suite:
void testParser();

#endif


