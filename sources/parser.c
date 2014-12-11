//
//  paser.c
//  The parser needs to accept a string from the text input
//
//  Created by ben on 07/11/2014.
//
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../includes/tower.h"
#include "../includes/actionQueueDataStructure.h"
#include "./../includes/parser.h"
#include "../includes/Information_Window.h"

/*
 * Parse called with string of user input from terminal window.
 * reads the first token and calls the relevant command function 
 * returns zero if syntax error.
 */
int parse(char *inputString)
{
    size_t len = 1+strlen(inputString);//gets the size of inputString
    if( len < 3*sizeof(char)  )
    {
        actionUsageError();
        return 0;
    }

    int numberOfChunks;
    char **commandArray = breakUpString(inputString, &numberOfChunks, " ");
    //array of strings, each elem holds a token string from the input command
    int minNumberOfChunks = 2,//as of cat man and upgrade
        maxNumberOfChunks = 3;//being implemented
    if( numberOfChunks<minNumberOfChunks || maxNumberOfChunks>3)
    {
        freeCommandArray(commandArray, numberOfChunks);
        return 0;//no valid commands with less than 2 strings or more than 3
    }

    //enumerated type commandType can describe each of the possible commands(see actionQueue.h)

    commandType action = getAction(commandArray[0]);//the first string in the command should contain the action
    
    if(action==commandError)//if getAction returns commandError then the input is invalid
    {                //Error messaging handled in getAction function
        freeCommandArray(commandArray, numberOfChunks);
        return 0;
    }
    

    int specificReturns=0;//stores return values of the different functions that execute the commands
    /**** Now we deal with each possible command separately as they all have different syntax ****/
    switch (action)
    {
        case upgrade:
        {
            if(numberOfChunks<3)
            {
                actionUsageError();
                freeCommandArray(commandArray, numberOfChunks);
                return 0;
            }
            specificReturns = parseUpgrade(commandArray, numberOfChunks);
            freeCommandArray(commandArray, numberOfChunks);
            return specificReturns;//0 for error
        }
        case cat:
        {
            specificReturns = parseCat(commandArray[1]);
            freeCommandArray(commandArray, numberOfChunks);
            return specificReturns;//0 for error
        }
        case man:
        {
            specificReturns = parseMan(commandArray[1]);
            freeCommandArray(commandArray, numberOfChunks);
            return specificReturns;//0 for error
        }
        case mktwr:
        {
            if(numberOfChunks<3)
            {
                freeCommandArray(commandArray, numberOfChunks);
                return 0;
            }
            specificReturns = parseMktwr(commandArray);
            freeCommandArray(commandArray, numberOfChunks);
            return specificReturns;//0 for error
        }
        case execute:
        case set:
        default:
            fprintf(stderr,"\n***parsing not implemented yet returning***\n");
            freeCommandArray(commandArray, numberOfChunks);
            return 0;
    }
    
}
/*
 *  Called when we read mktwr cmd.
 *  gets tower position and pushes to queue
 *  returns 1 if cmd was probably successfully pushed to queue
 *  returns 0 if definately not succesful or if target or stat call failed
 */
int parseMktwr(char ** commandArray)
{
    int towerPosition = tolower(commandArray[2][0]) - 'a' +1;
    upgradeStat twrType = getUpgradeStats(commandArray[1]);
    if(towerPosition<'a' || twrType!=INT || twrType!=CHAR)//put in a greaterthan bound on the number of postions
    {
        //syntax error
        return 0;
    }
    
    if(pushToQueue(getQueue(NULL),mktwr,twrType,towerPosition)>=1)
    {
        return 1;
    }
    else return 0;
}
/*  calls man printing functions
 *  returns 1 if ok
    returns 0 if error and prints message
 */
int parseMan(char * inputStringCommandMan)
{
    commandType commandToMan = getAction(inputStringCommandMan);
    switch (commandToMan)
    {
        case upgrade:
        {
            manUpgrade();
            return 1;
        }
        case cat:
        {
            manCat();
            return 1;
        }
        case man:
        {
            manMan();
            return 1;//0 for error
        }
        case execute:
        {
            //manExecute();
            return 1;
        }
        case set:
        {
            //manSet();
            return 1;
        }
        case mktwr:
        {
            return 1;
        }
        default:
        {
            fprintf(stderr,"\n*** Man Command Error ***\n");
            fprintf(stderr,"second command not recognised \n");
            fprintf(stderr,"you entered: %s\n",inputStringCommandMan);
            actionUsageError();
            return 0;
        }
    }
}



/*
 *  Called when we read cat cmd.
 *  gets target and pushes to info window.
 *  returns 1 if cmd was probably successfully pushed.
 *  returns 0 if definately not succesful or if target call failed.
 */
int parseCat(char * inputStringTargeting)
{
    //looks for tower type target:
    if( inputStringTargeting[0]=='t' || inputStringTargeting[0]=='T' )
    {
        unsigned int targetTower = getTargetTower(inputStringTargeting);
        if(targetTower)
        {
            towerMonitor(targetTower, NULL);//function in Information_Window.c
            return 1;
        }
        else
            return 0;
    }
    //can we also cat other things eg enemies?
    //for now
    else
        return 0;
    
}

/*  
 *  Called when we read upgrade cmd.
 *  gets stat and target and pushes to queue
 *  returns 1 if cmd was probably successfully pushed to queue
 *  returns 0 if definately not succesful or if target or stat call failed
 */
int parseUpgrade(char ** commandArray, int numberOfChunks)
{
    upgradeStat statToUpgrade = getUpgradeStats(commandArray[1]);
    
    int target = getTargetTower(commandArray[2]);
    
    if(target!=0 && statToUpgrade!=statError )
    {
        commandType action = upgrade;
        if(pushToQueue(getQueue(NULL),action,statToUpgrade,target)>=1)
            //push to queue returns number of items on queue
            return 1;
    }
    return 0;
}

/* 
 *  Called on cat and upgrade commands with the target specifying token.
    looks at the 2nd char in the string to find an int 1-9 to be the target.
    Note, wont work for anything > 9, would just see 1.
    Will print its own error message.
    Returns TargetTowerID if sucessful
    Returns 0 if error
 */
unsigned int getTargetTower(const char * inputStringTargeting)
{
    unsigned int numberOfTowers = getNumberOfTowers();//getNumberOfTowers(); this is func in tower.c

    size_t len = strlen(inputStringTargeting);//gets the size of string
    if( len<(2*sizeof(char)) )
    {
        fprintf(stderr,"*** SYNTAX ERROR: You must target a tower with this command ***\n");
        fprintf(stderr,"to target a tower enter t followed by a number 1 - %d \n",numberOfTowers);
        return 0;
    }
    if (inputStringTargeting[0]!='t' && inputStringTargeting[0]!='T')
    {
        fprintf(stderr,"*** ERROR: You must target a tower with this command ***\n");
        fprintf(stderr,"to target a tower enter t followed by a number 1 - %d \n",numberOfTowers);
        return 0;
    }
    
    unsigned int targetTower = (unsigned int)(inputStringTargeting[1]-'0');
    
    if(targetTower > numberOfTowers)
    {
        fprintf(stderr,"*** ERROR: target tower does not exist ***\n");
        fprintf(stderr,"You have only %d towers you entered t%d\n",
                numberOfTowers,targetTower);
        return 0;
    }

    return targetTower;
}

/*  Called when we read an upgrade command, tests the next token against the 
 *  possible stats returns the corresponding upgradeStat Or
    returns statError  and calls the upgradeStatUsageError function
 */
upgradeStat getUpgradeStats(const char * inputStringUpgradeStats)
{
    /*first lets make an array of strings to hold all the possible action commands*/
    const char **validUpgradeStats;
    int numberOfStats=7;//have 5 action commands at this time: upgrade, execute, set, man, cat
    validUpgradeStats=(const char **)malloc(numberOfStats*sizeof(char*));//array of $[numberOfActions] strings
    validUpgradeStats[0]="p";
    validUpgradeStats[1]="r";
    validUpgradeStats[2]="s";
    validUpgradeStats[3]="AOEr";
    validUpgradeStats[4]="AOEp";
    validUpgradeStats[5]="INT";
    validUpgradeStats[6]="CHAR";


    //now test the input string against all valid stats
    upgradeStat statToUpgrade=statError;
    for(int i=0; i<numberOfStats; ++i)
    {
        if(strcmp(inputStringUpgradeStats,validUpgradeStats[i])==0)//if the string is identical to one of the commands
        {                                        //then action is set to that command
            switch (i)
            {
                case 0:
                    statToUpgrade = power;
                    return statToUpgrade;
                case 1:
                    statToUpgrade = range;
                    return statToUpgrade;
                case 2:
                    statToUpgrade = speed;
                    return statToUpgrade;
                case 3:
                    statToUpgrade = AOErange;
                    return statToUpgrade;
                case 4:
                    statToUpgrade = AOEpower;
                    return statToUpgrade;
                case 5:
                    statToUpgrade = INT;
                    return statToUpgrade;
                case 6:
                    statToUpgrade = CHAR;
                    return statToUpgrade;
                    
            }
        }
    }
    
    if(statToUpgrade==statError)//if it is still set to ERROR then the user made a mistake
    {
        upgrageStatUsageError(inputStringUpgradeStats, statToUpgrade, validUpgradeStats, numberOfStats);
    }
    free(validUpgradeStats);//free the mallocd array
    return statToUpgrade;
}




/* 
 *  If there was a syntax error in the users command call this function which
    will print usage advice to the terminal window
 */
void upgrageStatUsageError(const char * inputStringUpgradeStats, upgradeStat statToUpgrade, const char ** validUpgradeStats, int numberOfStats)
{
    if(statToUpgrade==statError)//if it is still set to ERROR then the user made a mistake
    {
        fprintf(stderr,"*** stat not recognised ***\n");
        fprintf(stderr,"You entered: %s\n",inputStringUpgradeStats);
        fprintf(stderr,"These are the possible stats: \n");
        for(int i=0; i<numberOfStats; ++i)
        {
            fprintf(stderr,"%s\n",validUpgradeStats[i]);
        }
        fprintf(stderr,"\nType man [COMMAND] for usage\n");//we advise them on usage
    }//error messages will need to be passed back to the terminal to be printed. hopefully can do this by setting up a custom stream. For now will print to stderr.
}






/* 
 *  Takes the first string of the input command and tests it against the correct
    syntax to find which command they want to execute then returns that command 
    as a enum commandType variable. Returns commandType correspodning to the 
    validated command or a commandError commandType
 */
commandType getAction( const char * inputAction )
{
    /*first lets make an array of strings to hold all the possible action commands*/
    const char **validActions;
    int numberOfActions=6;//have 5 action commands at this time: upgrade, execute, set, man, cat
    validActions=(const char **)malloc(numberOfActions*sizeof(char*));//array of $[numberOfActions] strings
    validActions[0]="upgrade";
    validActions[1]="execute";
    validActions[2]="set";
    validActions[3]="man";
    validActions[4]="cat";
    validActions[5]="mktwr";
    //now test the input string against all valid actions
    commandType action = commandError;
    for(int i=0; i<numberOfActions; ++i)
    {
        if(strcmp(inputAction,validActions[i])==0)//if the string is identical to one of the commands
        {                                        //then action is set to that command
            switch (i)
            {
                case 0:
                    action = upgrade;
                    return action;
                case 1:
                    action = execute;
                    return action;
                case 2:
                    action = set;
                    return action;
                case 3:
                    action = man;
                    return action;
                case 4:
                    action = cat;
                    return action;
                case 5:
                    action = mktwr;
                    return action;
                
            }
        }
    }
    
    if(action==commandError)//if it is still set to ERROR then the user made a mistake
    {
        actionUsageError();
    }
    free(validActions);//free the mallocd array
    return action;
}



/*
 *   If there was a syntax error in the users command call this function which
     will print usage advice to the terminal window.
 */
void actionUsageError()
{
    const char **validActions;
    int numberOfActions=6;//have 5 action commands at this time: upgrade, execute,
                          //set, man, cat
    validActions=(const char **)malloc(numberOfActions*sizeof(char*));
    //array of
        //$[numberOfActions] strings
    validActions[0]="upgrade";
    validActions[1]="execute";
    validActions[2]="set";
    validActions[3]="man";
    validActions[4]="cat";
    validActions[5]="mktwr";

    
    fprintf(stderr,"*** Action not recognised ***\n");
    fprintf(stderr,"Possible commands: \n");
    for(int i=0; i<numberOfActions; ++i)
    {
        fprintf(stderr,"%s\n",validActions[i]);
    }
    fprintf(stderr,"\nType man [COMMAND] for usage\n");//we advise them on usage
    //error messages will need to be passed back to the terminal to be printed.
    //hopefully can do this by setting up a custom stream. For now will print to stderr.
    free(validActions);//free the mallocd array

}



/*
 *  Takes the input string and breaks into separate words (where there is a 
    space and new string starts) each of these words is stored in the 
    commandArray which is an array of strings
 */
char **breakUpString(const char * inputString, int *numberOfChunksPtr, const char * delimiter)
{
    char    *stringChunk,                       //holds the chunks on the input string as we break it up
            *inputStringDuplicate = strdup(inputString),//duplicate input string for editting
            **commandArray = NULL;              //this will be an array to hold each of the chunk strings
    int     numberOfChunks=0;
    
    //using http://www.tutorialspoint.com/c_standard_library/c_function_strtok.htm
    stringChunk = strtok(inputStringDuplicate, delimiter); // gets the first chunk (up to the first space)
    
    // walk through rest of string
    while( stringChunk != NULL )
    {
        ++numberOfChunks;
        commandArray=(char **)realloc(commandArray,numberOfChunks*sizeof(char*));//array of strings
        commandArray[numberOfChunks-1]=(char *)malloc((size_t)(strlen(stringChunk)*sizeof(char)+1));
        strcpy(commandArray[numberOfChunks-1],stringChunk);
        
        stringChunk = strtok(NULL, delimiter);
    }
    free(inputStringDuplicate);//frees the malloc made in strdup()
                               //$(numberOfChunks) strings now stored in the commandArray
    *numberOfChunksPtr=numberOfChunks;
    return commandArray;
}

/*
 *  Duplicates a string
 */
char *strdup(const char * s)
{
    size_t len = 1+strlen(s);//gets the size of s
    char *p = malloc(len);//allocates a block big enough to hold s
    
    return p ? memcpy(p, s, len) : NULL;//if p is non 0 ie malloc worked, then copy everything in s into p and return p. if p is NULL malloc didnt work so return NULL
}

/*
 *  frees the memory allocated in breakup string funct
 */
void freeCommandArray(char **commandArray,int numberOfChunks)
{
    for(int i=0; i<numberOfChunks; ++i)
    {
        free(commandArray[i]);
    }
    free(commandArray);
}

/*
 *  Test function. Prints contents of commandArray
 */
void testCommandArray(char ** commandArray, int numberOfChunks)
{
    for(int i=0; i<numberOfChunks; ++i)
    {
        printf("%s",commandArray[i]);
        printf("|\n");
    }
}
/*
 *  Test function. Prints the action  we read.
 */
void testGetAction(commandType action)
{
    /*first lets make an array of strings to hold all the possible action commands*/
    const char **validActions;
    int numberOfActions=6;//have 5 action commands at this time: upgrade, execute, set, man, cat
    validActions=(const char **)malloc(numberOfActions*sizeof(char*));//array of $[numberOfActions] strings
    validActions[0]="upgrade";
    validActions[1]="execute";
    validActions[2]="set";
    validActions[3]="man";
    validActions[4]="cat";
    validActions[5]="mktwr";
    printf("****testGetAction****\n");
    printf("read action: %s\n", validActions[action]);
    free(validActions);//free the mallocd array

}
/*
 *  Test function. Prints upgrade stat we have read.
 */
void testGetUpgradeStat(upgradeStat statToUpgrade)
{
    /*first lets make an array of strings to hold all the possible action commands*/
    const char **validUpgradeStats;
    int numberOfStats=5;//have 5 action commands at this time: upgrade, execute, set, man, cat
    validUpgradeStats=(const char **)malloc(numberOfStats*sizeof(char*));//array of $[numberOfActions] strings
    validUpgradeStats[0]="p";
    validUpgradeStats[1]="r";
    validUpgradeStats[2]="s";
    validUpgradeStats[3]="AOEr";
    validUpgradeStats[4]="AOEp";
    printf("****testGetUpgradeStat****\n");
    printf("read stat: %s\n", validUpgradeStats[statToUpgrade]);
    free(validUpgradeStats);//free the mallocd array
    
}
