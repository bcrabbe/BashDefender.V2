/*---------- Standard Headers -----------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*---------- Custom Headers -----------*/

#include "../includes/debug.h"
#include "../includes/sput.h"
#include "../includes/levelController.h"

/*---------- Functions ----------*/
//int SCREEN_WIDTH_GLOBAL;
//int SCREEN_HEIGHT_GLOBAL;

struct keyword	{

	levelCommand lCommand;
	KeywordProp *propertiesList;	
	int nProperties;
	Keyword next;
	Keyword prev;

};

struct keywordProp	{
	property p;
	int propertyValue;
};

struct keywordQueue	{

	Keyword start;
	Keyword end;
	int nCommands;
};

void initialQueueReader()	{
	Keyword current;
	KeywordQueue kQueue = getKWQueue(NULL);
	current = kQueue->start;
	while(current != NULL)	{
		switch(current->lCommand)	{
			case makeTowerP:
					makeTowerCommand(current);				
					current = removeLink(current);
					break;
			case totalWaves:
					setWaveTotalCommand(current);
					current = removeLink(current);
					break;
			case path:
					pathCommand(current);
					current = removeLink(current);
					break;
			default:
					current = current->next;
					break;
		}
	}
}

/*
 *Sets number of paths for current level
 */
void pathCommand(Keyword pathCommand)	{

		iprint(returnPropertyValue(pathCommand,pathLevel));
		iprint(returnPropertyValue(pathCommand,numberOfPaths));
}

void levelQueueReader()	{

	Keyword current;
	KeywordQueue kQueue = getKWQueue(NULL);
	current = kQueue->start;
	while(current != NULL)	{
		switch(current->lCommand)	{
			case wave:
					//! only expands waves into create enemies commands if it is at the start of the queue
					if(kQueue->start == current && getWave(getGame(NULL)) == returnPropertyValue(current,waveID))	{
						increaseEnemyNumbersThisWave(returnPropertyValue(current,numberOfEnemies));
						waveCreatorCommand(current);
						current = removeLink(current);
					} else { 
						current = current->prev;
					}
					break;
			case makeEnemy:
					if(createEnemyCommand(current))	{
						current = removeLink(current);
						return;
					} else {
						return;
					}
					break;
			case delay:
					setCreateEnemyGroupDelay(returnPropertyValue(current,dTime));
					current = removeLink(current);
					return;
					break;
			default:
					break;
		}
	}
}

Keyword removeLink(Keyword current)	{
	Keyword temp;
	KeywordQueue kQueue = getKWQueue(NULL);
	if(current == kQueue->start)	{
		kQueue->start = kQueue->start->prev;
		current = kQueue->start;
		if(kQueue->start != NULL)	{	
			free(kQueue->start->next);
			kQueue->start->next = NULL;
		}
	} else if(current == kQueue->end) {
		kQueue->end = current->next;
		current->next->prev = NULL;
		free(current);
		current = NULL;		
	} else {
		temp = current->prev;
		current->next->prev = current->prev;
		current->prev->next = current->next;	
		free(current);
		current = temp;
	}
	kQueue->nCommands--;
	return current;
}

int returnPropertyValue(Keyword current, property reqProperty)	{

	int i;
	for(i = 0; i < current->nProperties; i++)	{
		if(current->propertiesList[i]->p == reqProperty)	{
			return current->propertiesList[i]->propertyValue;
		}
	}

	return 0;

}
/*
 *Returns specified property value from specified queue position
 */
int returnPropertyValueFromQueue(int place,property reqProperty)	{

	KeywordQueue kQueue = getKWQueue(NULL);
	int numberOfKeywords;
	Keyword currKeyword;
	for(numberOfKeywords = 1, currKeyword = kQueue->start; currKeyword != NULL && place != numberOfKeywords; currKeyword = currKeyword->prev, numberOfKeywords++)	{
			/* Do Nothing */	
	}

	if(currKeyword == NULL)	{
		return 0;
	} else	{
		return returnPropertyValue(currKeyword,reqProperty);
	}
}

int createEnemyCommand(Keyword makeEnemy)	{
	//! only create enemy if all cooldowns are ready
	if(checkClock(singleEnemyCreated,ENEMYSPAWNCOOLDOWN) && checkClock(groupDelay,getEnemyGroupDelay()))	{
		setCreateEnemyGroupDelay(0); //!setting delay back to zero
		createSpecificEnemy(returnPropertyValue(makeEnemy,enemyType),returnPropertyValue(makeEnemy,enemyLevel),returnPropertyValue(makeEnemy,entrance));

		return 1;
	} 
	return 0;

}

void waveCreatorCommand(Keyword waveKeyWord)	{
	int enemyNum;
	int totalEnemies = returnPropertyValue(waveKeyWord,numberOfEnemies);
   	for(enemyNum = 0; enemyNum < totalEnemies; enemyNum++)	{
		breakDownWaveCommand(waveKeyWord->propertiesList,waveKeyWord->nProperties);
	}
	//! Adding delay for next group creation
	addGroupCreationDelay(waveKeyWord);
}

int addGroupCreationDelay(Keyword waveKW)	{
	int dValue;
	KeywordQueue kWQueue = getKWQueue(NULL);
    Keyword newKey = createKeyword();
    addKWtoQueue(newKey);
    newKey->lCommand = delay;
	if((dValue = returnPropertyValue(waveKW,dTime)))	{
		addProperty(dTime);	
		kWQueue->end->propertiesList[kWQueue->end->nProperties-1]->propertyValue = dValue;
		return 1;
	}

	return 0;
}


void makeTowerCommand(Keyword setTower)	{
	addTowerPosNode(returnPropertyValue(setTower,x),returnPropertyValue(setTower,y));
}

void breakDownWaveCommand(KeywordProp *propertiesList, int nProps)	{
		int n;
		Keyword newKey = createKeyword();
		addKWtoQueue(newKey);	
		newKey->lCommand = makeEnemy;
		KeywordQueue kWQueue = getKWQueue(NULL);
		for(n = 0; n < nProps; n++)	{
			if(propertiesList[n]->p != numberOfEnemies && propertiesList[n]->p != dTime)	{
				addProperty(propertiesList[n]->p);
				kWQueue->end->propertiesList[kWQueue->end->nProperties-1]->propertyValue = propertiesList[n]->propertyValue;
			}
		}

}

void setWaveTotalCommand(Keyword setWaveTotal)	{
	setTotalWaveNo(returnPropertyValue(setWaveTotal,total));
}

void printQueue()	{
	
	KeywordQueue kQueue = getKWQueue(NULL);
	Keyword curr = kQueue->start;
	int cmdN = kQueue->nCommands;
	int prop;
	printf("There are %d commands in queue\n",kQueue->nCommands);
	while(curr->prev != NULL)	{
		printf("commands %d\n",cmdN);
		printf("keyword is: %d\n",curr->lCommand);
		printf("Has %d properties\n",curr->nProperties);
		for(prop = 1; prop <= curr->nProperties; prop++)	{
			printf("property %d is %d\n",prop,curr->propertiesList[prop-1]->propertyValue);
		}	
		curr = curr->prev;
	}	
	printf("keyword is: %d\n",curr->lCommand);
	printf("Has %d properties\n",curr->nProperties);
	for(prop = 1; prop <= curr->nProperties; prop++)	{
		printf("property %d is %d\n",prop,curr->propertiesList[prop-1]->propertyValue);
	}	
}

Keyword createKeyword()	{

	Keyword newKw = (Keyword) malloc(sizeof(*newKw));
	newKw->propertiesList = NULL;
	newKw->prev = NULL;
	newKw->next = NULL;
	newKw->nProperties = 0;
	return newKw; 
}

void addKWtoQueue(Keyword newKW)	{

		KeywordQueue kQueue = getKWQueue(NULL);
		if((kQueue->start == NULL) && (kQueue->end == NULL))	{
			kQueue->start = kQueue->end = newKW;
		} else {
			newKW->next = kQueue->end;
			kQueue->end->prev = newKW;
			kQueue->end = kQueue->end->prev;
		}
		kQueue->nCommands++;

}

void createKeywordQueue()	{

	KeywordQueue newQueue = (KeywordQueue) malloc(sizeof(*newQueue));
	newQueue->start = NULL;
	newQueue->end = NULL;
	newQueue->nCommands = 0;
	getKWQueue(newQueue);
}

KeywordQueue getKWQueue(KeywordQueue kwQueue)	{

	static KeywordQueue currKWQueue;

	if(kwQueue != NULL)	{
		currKWQueue = kwQueue;
	}

	return currKWQueue;

}

/*
 *Initializes all data structures required for level
 */
void initLevel()    {
	createKeywordQueue();
	createLevel();
    createLevelPaths();
    createTowerGroup();
    createActionQueue();
    createGame();
	createLevelClocks();
    createEnemyGroup();
	createTowerPos();
	initialQueueReader();
	createProjectileList();
	initialiseParseLists();
	init_abilities();
}

void createLevelClocks()	{

		addClock(singleEnemyCreated);
		addClock(lastCmdAction);
		addClock(groupDelay);
}

void createLevel()	{
	readLevelSettingsFile("../data/level1.txt");
}

void addKeyWordToken(char *token)	{
		Keyword newKey = createKeyword();
		if(!strcmp(token,"towerPos"))	{
			addKWtoQueue(newKey); 
			newKey->lCommand = makeTowerP;
		} else if(!strcmp(token,"totalWaves"))	{
			addKWtoQueue(newKey); 
			newKey->lCommand = totalWaves;
		} else if(!strcmp(token,"wave"))	{
			addKWtoQueue(newKey); 
			newKey->lCommand = wave;
		} else if(!strcmp(token,"path"))	{
			addKWtoQueue(newKey); 
			newKey->lCommand = path;
		} else {
			fprintf(stderr,"Keyword not recognised\n");
			free(newKey);
		}
}

void addProperty(property p)	{
	KeywordQueue kWQueue = getKWQueue(NULL);
	kWQueue->end->nProperties++;

	if(kWQueue->end->nProperties == 1)	{
		kWQueue->end->propertiesList = (KeywordProp*) malloc(sizeof(*kWQueue->end->propertiesList));
	} else	{
		kWQueue->end->propertiesList = (KeywordProp*) realloc(kWQueue->end->propertiesList, (kWQueue->end->nProperties)*sizeof(*(kWQueue->end->propertiesList))); 
	}


	kWQueue->end->propertiesList[kWQueue->end->nProperties-1] = (KeywordProp) malloc(sizeof(*(kWQueue->end->propertiesList[kWQueue->end->nProperties -1])));

	kWQueue->end->propertiesList[kWQueue->end->nProperties -1]->p = p;
}



int validateLine(char *Line, int nWords)	{

	char *token;
	int wordCount = 0;
	while(wordCount < nWords)	{
		token = getToken(Line);
		if(wordCount < 1)	{
			addKeyWordToken(token); //!This is a keyword
		} else	{
			if(!checkProperty(token)){	//!This is a property
				addValue(token);		//! Not a property add a value	
			}	
		}
		free(token);
		token = NULL;
		wordCount++;
	}

	return 0;
}

int checkProperty(char *token)	{
	if(!strcmp(token,"x"))	{
		addProperty(x);
	} else if(!strcmp(token,"y"))	{
		addProperty(y);	
	} else if(!strcmp(token,"total"))	{
		addProperty(total);	
	} else if(!strcmp(token,"waveNum"))	{
		addProperty(waveID);	
	} else if(!strcmp(token,"enemyType"))	{
		addProperty(enemyType);
	} else if(!strcmp(token,"numberOfEnemies"))	{
		addProperty(numberOfEnemies);
	} else if(!strcmp(token,"delay"))	{
		addProperty(dTime);	
	} else if(!strcmp(token,"entrance"))	{
		addProperty(entrance);
	} else if(!strcmp(token,"level"))	{
		addProperty(enemyLevel);		
	} else if(!strcmp(token,"pathLevel"))	{
		addProperty(pathLevel);		
	} else if(!strcmp(token,"numberOfPaths"))	{
		addProperty(numberOfPaths);		
	}else {
		return 0;
	}

	return 1;
}

void addValue(char *token)	{
	KeywordQueue kWQueue = getKWQueue(NULL);
	int value = atoi(token);
	kWQueue->end->propertiesList[kWQueue->end->nProperties-1]->propertyValue = value;
}

char* getToken(char *line)	{
	int c = 0, letter = 0;
	char *word = NULL;
	if(line[c] == READCHAR)	{
		for(c = 0; line[c] == READCHAR || line[c] == ' '; c++){
		}
	}
	for(; line[c] != ' ' && line[c] != ENDOFSTRING; c++,letter++)	{
		word = expandCBuffer(word,letter);
		word[letter] = line[c];
		line[c] = READCHAR;		//!Marking characters as read
	}
	word[letter] = '\0';
	return word;
}

char* expandCBuffer(char *toExpand, int currSize)	{
	if(!currSize)	{
		return (toExpand = (char*) malloc(sizeof(char)));
	} else {
		return (toExpand = (char*) realloc(toExpand, (currSize+1)*(sizeof(char))));
	}
}

/*
 *Reads level settings file
 */
void readLevelSettingsFile(char *file)	{
	FILE *fp;
    char letter;
	int wordCount = 1;
   	char *currentLine; 
	int currSize = 0;
	if((fp = fopen(file,"r")) != NULL)	{
		while((letter = getc(fp)) != EOF)	{
			if(letter == ' ')	{
				wordCount++;
			}
				currentLine = expandCBuffer(currentLine, currSize);
			if(letter == '\n')	{
				currentLine[currSize] = ENDOFSTRING;
				validateLine(currentLine,wordCount);
				free(currentLine);
				currentLine = NULL;
				wordCount = 1;
				currSize = 0;
			} else {
				currentLine[currSize] = letter;
				currSize++;
			}
		}	
	}	
}


/*
 *Frees all Data structures
 */
void endLevel() {

    freeAllTowers();
    free(getTowerGrp(NULL));
    free(getGame(NULL));
    free(getQueue(NULL));
	freeAllTowerPositions();
	freeEnemyGroup();
	freeLevelPaths();
}

/*---------- Test Functions ----------*/
void setUpTesting()	{
    createKeywordQueue();
	readLevelSettingsFile(TESTLEVEL);	
    createLevelPaths();
    createTowerGroup();
    createActionQueue();
    createGame();
    createLevelClocks();
    createEnemyGroup();
    createTowerPos();
    createProjectileList();
    initialiseParseLists();
    init_abilities();
}

void testLevelController()	{

    sput_start_testing();
    sput_set_output_stream(NULL);

    sput_enter_suite("testReadLevelSettingsFile(): Testing reading and processing level keywords");
    sput_run_test(testReadLevelSettingsFile);
    sput_leave_suite();


    sput_finish_testing();
}

void testReadLevelSettingsFile()	{
	sput_fail_unless(countKeywords() == 8,"8 Keywords Should Exist in the level settings queue");
	initialQueueReader();	//! Removing set up commands
	sput_fail_unless(countKeywords() == 3,"3 Keywords Should Exist in the level settings queue");
	sput_fail_unless(returnPropertyValueFromQueue(1,waveID) == 1,"First keyword has waveID 1");
	sput_fail_unless(returnPropertyValueFromQueue(2,waveID) == 2,"Second keyword has waveID 2");
	sput_fail_unless(returnPropertyValueFromQueue(3,waveID) == 3,"Third keyword has waveID 3");
	levelQueueReader();
}


int countKeywords()	{
		KeywordQueue kQueue = getKWQueue(NULL);
		int numberOfKeywords;
		Keyword currKeyword;
		for(numberOfKeywords = 0, currKeyword = kQueue->start; currKeyword != NULL; currKeyword = currKeyword->prev, numberOfKeywords++)	{
			/* Do Nothing */	
		}
		return numberOfKeywords;
}




