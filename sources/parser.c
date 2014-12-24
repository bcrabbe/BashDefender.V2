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
#include <stdbool.h>
#include <math.h>

#include "../includes/tower.h"
#include "../includes/actionQueueDataStructure.h"
#include "./../includes/parser.h"
#include "../includes/Information_Window.h"
#include "../includes/abilities.h"
#include "../includes/enemy.h"
#include "../includes/sput.h"

unsigned int getTargetTower(const char * inputStringTargeting, bool needsIdentifier);
int parseMktwr(char ** commandArray, int numberOfTokens);




unsigned long int stringToInt(const char * string);

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
        optionUsageError();
        return 0;
    }


    int numberOfTokens;
    char **commandArray = breakUpString(inputString, &numberOfTokens, " ,");
    testCommandArray(commandArray, numberOfTokens);
    //array of strings, each elem holds a token string from the input command
    int minNumberOfChunks = 2;//as of cat man and upgrade
    if( numberOfTokens < minNumberOfChunks )
    {
        optionUsageError();
        freeCommandArray(commandArray, numberOfTokens);
        return 0;//no valid commands with less than 2 strings or more than 3
    }
  
    int specificReturns = parseCommands(commandArray,numberOfTokens);
    
    freeCommandArray(commandArray, numberOfTokens);
    return specificReturns;//0 for error
}
/*
 *
 */
int parseCommands(char ** commandArray, int numberOfTokens)
{
    //enumerated type cmdType can describe each of the possible commands(see actionQueue.h)
    cmdType command = getCommandType(commandArray[0]);//the first string in the command should contain the action
    
    if(command==cmd_commandError)//if getAction returns commandError then the input is invalid
    {                //Error messaging handled in getCommandType function
        return 0;
    }
    int specificReturns=0;//stores return values of the different functions that execute the commands
    /**** Now we deal with each possible command separately as they all have different syntax ****/
    switch (command)
    {
        case cmd_upgrade:
        {
            if(numberOfTokens<3) {
                optionUsageError();
                specificReturns = 0;
            }
            else {
                specificReturns = parseUpgrade(commandArray, numberOfTokens);
            }
            break;
            
        }
        case cmd_cat:
        {
            if(numberOfTokens!=2) {
                optionUsageError();
                specificReturns = 0;
            }
            else {
                specificReturns = parseCat(commandArray[1]);
            }
            break;
            
        }
        case cmd_man:
        {
            if(numberOfTokens!=2) {
                optionUsageError();
                specificReturns = 0;
            }
            else {
                specificReturns = parseMan(commandArray[1]);
            }
            break;
            
        }
        case cmd_mktwr:
        {
            if(numberOfTokens<3)
            {
                optionUsageError();
                specificReturns = 0;
            }
            else {
                specificReturns = parseMktwr(commandArray,numberOfTokens);
            }
            break;
            
        }
        case cmd_aptget:
        {
            if(numberOfTokens!=2) {
                optionUsageError();
                specificReturns = 0;
            }
            else {
                specificReturns = parseAptget(commandArray[1]);
            }
            break;
            
        }
        case cmd_ps:
        {
            if(numberOfTokens!=2) {
                optionUsageError();
                specificReturns = 0;
            }
            else {
                specificReturns = parsePs(commandArray[1]);
            }
            break;
        }
        case cmd_kill:
        {
            parseKill(commandArray, numberOfTokens);
            break;
        }
        case cmd_commandError:
        {
            printf("command was not read\n");
            break;
        }
        case cmd_execute:
        case cmd_set:
        default:
            fprintf(stderr,"\n***parsing not implemented yet returning***\n");
    }
    return specificReturns;

}

/*
 *
 */
int parseKill(char ** commandArray,int numberOfTokens)
{
    cmdOption option = getCommandOption(commandArray[1]);
    
    if(option!=kill_minus9 && option!=kill_all)
    {
        optionUsageError();
        return 0;
    }
    if(option==kill_minus9) {
        if(numberOfTokens!=3) {
            optionUsageError();
            return 0;
        }
        else {
            int targetEnemyID = getTargetEnemy(commandArray[2]);//pass 3rd token
            //kill_ability(targetEnemyID);
            return 1;
        }
    }
    else if(option==kill_all) {
        //kill_all_ability();
        return 2;
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
unsigned int getTargetEnemy(const char * inputStringTargeting)
{
    unsigned int numberOfEnemies = getNumberOfEnemies();// this is func in enemy.c
    
    size_t len = strlen(inputStringTargeting);//gets the size of string
    if( len<(2*sizeof(char)) )
    {
        fprintf(stderr,"*** SYNTAX ERROR: You must target a tower with this command ***\n");
        fprintf(stderr,"to target a tower enter t followed by a number 1 - %d \n",numberOfEnemies);
        return 0;
    }
    if (inputStringTargeting[0]!='e' && inputStringTargeting[0]!='E')
    {
        fprintf(stderr,"*** ERROR: You must target a enemy with this command ***\n");
        fprintf(stderr,"to target a enemy enter e followed by a number 1 - %d \n",numberOfEnemies);
        return 0;
    }
    
    unsigned int targetEnemyID = (unsigned int)(inputStringTargeting[1]-'0');
    
    if(targetEnemyID > numberOfEnemies || targetEnemyID<1)
    {
        fprintf(stderr,"*** ERROR: target enemy does not exist ***\n");
        fprintf(stderr,"there are only %d enemies you entered e%d\n",
                numberOfEnemies,targetEnemyID);
        return 0;
    }
    return targetEnemyID;
}
/*
 *
 */
int parsePs(char * optionString)
{
    cmdOption option = getCommandOption(optionString);
    if(option != ps_x) {
        optionUsageError();
        return 0;
    }
    else {
        psx_ability();
        return 1;
    }
}

/*
 *
 */
int parseAptget(char * aptToGetString)
{
    cmdOption aptToGet = getCommandOption(aptToGetString);
    if(aptToGet!=aptget_kill)
    {
        fprintf(stderr,"\n***app not recognised***\n");
        fprintf(stderr,"type man aptget to see availible apps\n");
        return 0;
    }
    if(pushToQueue(getQueue(NULL),cmd_aptget,aptToGet,0)>=1)
    {
        printf("pushing tower to queue\n");
        return 1;
    }
    else return 0;
}

/*
 *  Called when we read mktwr cmd.
 *  gets tower position and pushes to queue
 *  returns 1 if cmd was probably successfully pushed to queue
 *  returns 0 if definately not succesful or if target or stat call failed
 */
int parseMktwr(char ** commandArray, int numberOfTokens)
{
    cmdOption twrType = getCommandOption(commandArray[1]);
    if( !(twrType==mktwr_int || twrType==mktwr_char) )
    {
        optionUsageError();
        
        terminalWindow("mktwr expected a type (int, or char)");
        return 0;
    }
    
    unsigned int numberOfTowers = getNumberOfTowers();//getNumberOfTowers(); this is func in tower.c
    int token = 2;
    while(token < numberOfTokens) {
        int towerPosition = (int)tolower(commandArray[token][0]) - 'a' + 1;
		iprint(isTowerPositionAvailable(towerPosition));
        if( isTowerPositionAvailable(towerPosition) ){
            if(pushToQueue(getQueue(NULL),cmd_mktwr,twrType,towerPosition)>=1)
            {
                printf("pushing tower %d to queue\n",towerPosition);
            }
        }
        else {
            if( towerPosition < 1 || towerPosition > maxTowerPosition() ) {
                char str[50];
                sprintf(str,"mktwr expected a target positon A - %c",maxTowerPositionChar());
                terminalWindow(str);
            }
        }
        ++token;
    }
    return 1;
}

/*  calls man printing functions
 *  returns 1 if ok
    returns 0 if error and prints message
 */
int parseMan(char * inputStringCommandMan)
{
    cmdType commandToMan = getCommandType(inputStringCommandMan);
    switch (commandToMan)
    {
        case cmd_upgrade:
        {
            manUpgrade();
            return 1;
        }
        case cmd_cat:
        {
            manCat();
            return 1;
        }
        case cmd_man:
        {
            manMan();
            return 1;//0 for error
        }
        case cmd_ps:
        {
            manPs();
            return 1;//0 for error
        }
        case cmd_kill:
        {
            manKill();
            return 1;//0 for error
        }
        case cmd_execute:
        {
            //manExecute();
            return 1;
        }
        case cmd_set:
        {
            //manSet();
            return 1;
        }
        case cmd_mktwr:
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
        unsigned int targetTower = getTargetTower(inputStringTargeting, true);
        if(targetTower)
        {
            displayTowerInfo(targetTower);//function in Information_Window.c
            return 1;
        }
        else
            return 0;
    }
    //can we also cat other things eg enemies?
    //for now
    else {
        return 0;
    }
    
}

void cleanUpParseUpgrade(cmdOption * statsToUpgradeArray,int * targetArray)
{
    if(statsToUpgradeArray) {
        free(statsToUpgradeArray);
    }
    if(targetArray) {
        free(targetArray);
    }
}
/*
 *  Called when we read upgrade cmd.
 *  gets stat and target and pushes to queue
 *  returns 1 if cmd was probably successfully pushed to queue
 *  returns 0 if definately not succesful or if target or stat call failed
 */
int parseUpgrade(char ** commandArray, int numberOfChunks)
{
//    printf("start parseUpgrade\n");
    cmdOption * statsToUpgradeArray = NULL;
    int numberOfStatsBeingUpgraded = 0;
    
    cmdOption statToUpgrade = getCommandOption(commandArray[1]);
    while(statToUpgrade>0 && statToUpgrade<=6) {
        ++numberOfStatsBeingUpgraded;
        cmdOption * tmp = realloc(statsToUpgradeArray, numberOfStatsBeingUpgraded*sizeof(cmdOption));
        if(tmp==NULL) {
            fprintf(stderr,"realloc error parser.c parseUpgrade() 1 \n");
            exit(1);
        }
        statsToUpgradeArray=tmp;
        statsToUpgradeArray[numberOfStatsBeingUpgraded-1] = statToUpgrade;
        statToUpgrade = getCommandOption(commandArray[1+numberOfStatsBeingUpgraded]);
    }
    if(!numberOfStatsBeingUpgraded) {
        //optionUsageError();
        cleanUpParseUpgrade(statsToUpgradeArray,NULL);
        return 0;
    }
    
//    printf("stat array: ");
//    for(int statIter=0; statIter<numberOfStatsBeingUpgraded; ++statIter) {
//        printf("%d ", statsToUpgradeArray[statIter]);
//    }
//    printf("\nstart targets \n");

    //now get targets
    int * targetArray = NULL;
    int numberOfTargets = 0;
    int firstTargetToken = 1+numberOfStatsBeingUpgraded;
    int target = getTargetTower(commandArray[firstTargetToken], true);
    while( firstTargetToken+numberOfTargets < numberOfChunks) {
        ++numberOfTargets;
        int * tmp = realloc(targetArray, numberOfTargets*sizeof(int));
        if(tmp==NULL) {
            fprintf(stderr,"realloc error parser.c parseUpgrade() 2\n");
            exit(1);
        }
        targetArray=tmp;
        targetArray[numberOfTargets-1] = target;
        if(firstTargetToken+numberOfTargets >= numberOfChunks ) {
            break;
        }
        
        target = getTargetTower(commandArray[firstTargetToken+numberOfTargets], false);
        if(target==0) {
            optionUsageError();
            cleanUpParseUpgrade(statsToUpgradeArray, targetArray);
            return 0;
        }
    }
    if(!numberOfTargets) {
        optionUsageError();
        cleanUpParseUpgrade(statsToUpgradeArray,targetArray);
        return 0;
    }
//    printf("tar array: ");
//
//    for(int tarIter=0; tarIter<numberOfTargets; ++tarIter) {
//        printf("%d ", targetArray[tarIter]);
//    }
    for(int statIter=0; statIter<numberOfStatsBeingUpgraded; ++statIter) {
        for(int tarIter=0; tarIter<numberOfTargets; ++tarIter) {
            if(pushToQueue(getQueue(NULL),cmd_upgrade, statsToUpgradeArray[statIter],
                           targetArray[tarIter])>=1) {
                 printf("\n>>> pushed stat = %d tar = %d <<< \n",statsToUpgradeArray[statIter],targetArray[tarIter]);
            }
        }
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
unsigned int getTargetTower(const char * inputStringTargeting, bool needsIdentifier)
{
    unsigned int numberOfTowers = getNumberOfTowers();//getNumberOfTowers(); this is func in tower.c
    
    size_t len = strlen(inputStringTargeting);//gets the size of string
    if( len<1  || ( needsIdentifier &&  len<2 ) )
    {
        char str[100];
        sprintf(str,"ERROR: You must target a towers with this command\nTo target a tower enter t followed by a number or list of numbers 1 - %d",numberOfTowers);
        terminalWindow(str);
        fprintf(stderr,"*** SYNTAX ERROR: You must target a tower with this command ***\n");
        fprintf(stderr,"to target a tower enter t followed by a number 1 - %d \n",numberOfTowers);
        return 0;
    }
    if ( needsIdentifier && !(inputStringTargeting[0]=='t' || inputStringTargeting[0]=='T') )
    {
        terminalWindow("ERROR: You must target a towers with this command");
        char str[100];
        sprintf(str,"ERROR: You must target a towers with this command\nTo target a tower enter t followed by a number or list of numbers 1 - %d",numberOfTowers);
        terminalWindow(str);
        fprintf(stderr,"*** ERROR: You must target a towers with this command ***\n");
        fprintf(stderr,"to target a tower enter t followed by a number or list of numbers 1 - %d \n",numberOfTowers);
        return 0;
    }
    unsigned int targetTower = 0;
    if( inputStringTargeting[0]=='t' || inputStringTargeting[0]=='T' ) {
        targetTower = stringToInt(inputStringTargeting+1    );

        //  targetTower = (unsigned int)(inputStringTargeting[1]-'0');
        //        printf("getTargetTower read %c giving %d\n",inputStringTargeting[1],targetTower);
    }
    else {
        targetTower = stringToInt(inputStringTargeting);
        //targetTower = (unsigned int)(inputStringTargeting[0]-'0');
        //  printf("getTargetTower read %c giving %d \n",inputStringTargeting[0],targetTower);
    }
    if(targetTower > numberOfTowers || targetTower < 1 )
    {
        char str[100];
        sprintf(str,"ERROR: target tower does not existYou have only %d towers you entered t%d",numberOfTowers,
                targetTower);
        terminalWindow(str);
        
        fprintf(stderr,"*** ERROR: target tower does not exist ***\n");
        fprintf(stderr,"You have only %d towers you entered t%d\n",
                numberOfTowers,targetTower);
        return 0;
    }
    return targetTower;
}


unsigned long int stringToInt(const char * string) {
    unsigned long int converted=0;
    size_t length = strlen(string);
    for(int i=0; i<length; ++i) {
        converted += (unsigned int)(string[i]-'0') * pow( 10, (length-i-1)) ;
    }
    return converted;
}
/*  Called when after we read a command, tests the next token against the
 *  possible options returns the corresponding cmdOption Or
    returns optionError  and calls the optUsageError function
 */
cmdOption getCommandOption(char * secondToken)
{
    for(int i = 0; secondToken[i]; i++) {
        secondToken[i] = tolower(secondToken[i]);
    }
    if(secondToken[0]=='-')//eat leading minus
    {
        secondToken=secondToken+1;
    }
    /*first lets get the array of strings that hold all the possible action commands*/
    stringList * optionList = getOptionList(NULL);
    int numberOfOptions=optionList->numberOfStrings;

    //now test the input string against all valid stats
    cmdOption option = optionError;
    for(int i=1; i<=numberOfOptions; ++i)
    {
        if(strcmp(secondToken,optionList->stringArray[i])==0)//if the string is identical to one of the commands
        {                                        //then action is set to that command
            switch (i)
            {
                case 1:
                    option = upgrade_power;
                    break;
                case 2:
                    option = upgrade_range;
                    break;
                case 3:
                    option = upgrade_speed;
                    break;
                case 4:
                    option = upgrade_AOErange;
                    break;
                case 5:
                    option = upgrade_AOEpower;
                    break;
                case 6:
                    option = upgrade_level;
                    break;
                case 7:
                    option = mktwr_int;
                    break;
                case 8:
                    option = mktwr_char;
                    break;
                case 9:
                    option = ps_x;
                    break;
                case 10:
                    option = kill_minus9;
                    break;
                case 11:
                    option = kill_all;
                    break;
                case 12:
                    option = aptget_kill;
                    break;
            }
            break;
        }
    }
    
    /*if(option==optionError)//if it is still set to ERROR then the user made a mistake
    {
        optionUsageError();
    }*/
    return option;
}




/* 
 *  If there was a syntax error in the users command call this function which
    will print usage advice to the terminal window
 */
void optionUsageError()
{
    terminalWindow("ERROR: Could not execute command.");
    terminalWindow("Type man [COMMAND] for help");
    fprintf(stderr,"*** Syntax error: Could not execute command.***\n");
    fprintf(stderr,"\nType man [COMMAND] for usage\n");//we advise them on usage
    //error messages will need to be passed back to the terminal to be printed. hopefully can do this by setting up a custom stream. For now will print to stderr.
}






/* 
 *  Takes the first string of the input command and tests it against the correct
    syntax to find which command they want to execute then returns that command 
    as a enum cmdType variable. Returns cmdType correspodning to the
    validated command or a commandError cmdType
 */
cmdType getCommandType(char * firstToken )
{
    for(int i = 0; firstToken[i]; i++) {
        firstToken[i] = tolower(firstToken[i]);
    }
    stringList * commandList = getCommandList(NULL);
    //now test the input string against all valid actions
    cmdType command = cmd_commandError;
    for(int i=1; i<=commandList->numberOfStrings; ++i)
    {
        if(strcmp(firstToken,commandList->stringArray[i])==0)//if the string is identical to one of the commands
        {                                        //then action is set to that command
            switch (i)
            {
                case 1:
                    command = cmd_upgrade;
                    break;
                case 2:
                    command = cmd_execute;
                    break;
                case 3:
                    command = cmd_set;
                    break;
                case 4:
                    command = cmd_man;
                    break;
                case 5:
                    command = cmd_cat;
                    break;
                case 6:
                    command = cmd_mktwr;
                    break;
                case 7:
                    command = cmd_ps;
                    break;
                case 8:
                    command = cmd_aptget;
                    break;
                
                case 9:
                    command = cmd_kill;
                    break;
            }
            break;
        }
    }
    
    if(command==cmd_commandError)//if it is still set to ERROR then the user made a mistake
    {
        actionUsageError(firstToken);
    }
    return command;
}

/*
 *   If there was a syntax error in the users command call this function which
     will print usage advice to the terminal window.
 */
void actionUsageError(const char * firstToken)
{
    stringList * commandList = getCommandList(NULL);
    fprintf(stderr,"*** ""%s"" command not recognised ***\n",firstToken);
    fprintf(stderr,"installed commands: \n");
    char str[200];
    sprintf(str," ""%s"" command not recognised",firstToken);
    
    int numberOfCommands=commandList->numberOfStrings;

    for(int i=1; i<=numberOfCommands; ++i)
    {
        fprintf(stderr,"%s\n",commandList->stringArray[i]);
    }
    fprintf(stderr,"\nType man [COMMAND] for usage\n");//we advise them on usage
    //error messages will need to be passed back to the terminal to be printed.
    //hopefully can do this by setting up a custom stream. For now will print to stderr.

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
    //free(commandArray);
}
void testStringLists()
{
    stringList * cmdList = getCommandList(NULL);
    testCommandArray(cmdList->stringArray,cmdList->numberOfStrings);
    stringList * optList = getOptionList(NULL);
    testCommandArray(optList->stringArray,optList->numberOfStrings);
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

void freeParseLists()
{
    stringList * commandList = getCommandList(NULL);
    for(int i=1; i<=commandList->numberOfStrings; ++i) {
        free(commandList->stringArray[i]);
    }
    free(commandList->stringArray+1);
    free(commandList);
    
    stringList * optsList = getOptionList(NULL);
    for(int i=1; i<=optsList->numberOfStrings; ++i) {
        free(optsList->stringArray[i]);
    }
    free(optsList->stringArray+1);
    free(optsList);
}
void initialiseParseLists()
{
    stringList * commandList = intialiseCommandList();
    getCommandList(commandList);
    stringList * optionList = intialiseOptionList();
    getOptionList(optionList);
}
stringList * getCommandList(stringList * commandList)
{
    static stringList * storedCommandList = NULL;
    if(commandList != NULL && storedCommandList == NULL ) {
        storedCommandList = commandList;
    }
    return storedCommandList;
}
stringList *  getOptionList(stringList * optionList)
{
    static stringList * storedOptionList = NULL;
    if(optionList != NULL && storedOptionList == NULL ) {
        storedOptionList = optionList;
    }
    return storedOptionList;
}


stringList * intialiseCommandList()
{
    /* make an array of strings to hold all the possible action commands*/
    static char **validActions;
    int numberOfActions=9;//have 5 action commands at this time: upgrade, execute, set, man, cat
    validActions=malloc((numberOfActions)*sizeof(char*));//array of $[numberOfActions] strings
    validActions-=1;
    validActions[1]=strdup("upgrade");
    validActions[2]=strdup("execute");
    validActions[3]=strdup("set");
    validActions[4]=strdup("man");
    validActions[5]=strdup("cat");
    validActions[6]=strdup("mktwr");
    validActions[7]=strdup("ps");
    validActions[8]=strdup("apt-get");
    validActions[9]=strdup("kill");


    stringList * commandList = malloc(sizeof(stringList));
    commandList->stringArray=validActions;
    commandList->numberOfStrings=numberOfActions;
    
    return commandList;
}

stringList * intialiseOptionList()
{
    /*first lets make an array of strings to hold all the possible action commands*/
    char **validOptions;
    int numberOfOptions=12;//have 5 action commands at this time: upgrade, execute, set, man, cat
    validOptions=malloc((numberOfOptions)*sizeof(char*));    //upgrade opts
    validOptions-=1;
    validOptions[1]=strdup("p");
    validOptions[2]=strdup("r");
    validOptions[3]=strdup("s");
    validOptions[4]=strdup("aoer");
    validOptions[5]=strdup("aoep");
    validOptions[6]=strdup("lvl");
    //mktwr opts:
    validOptions[7]=strdup("int");
    validOptions[8]=strdup("char");
    //ps opts:
    validOptions[9]=strdup("x");
    //kill opts:
    validOptions[10]=strdup("9");
    validOptions[11]=strdup("all");
    //aptget opts:
    validOptions[12]=strdup("kill");
    
    stringList * optionsList = malloc(sizeof(stringList));
    optionsList->stringArray=validOptions;
    optionsList->numberOfStrings=numberOfOptions;
    
    return optionsList;
}


