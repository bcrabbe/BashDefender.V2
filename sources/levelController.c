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
			default:
					current = current->next;
					break;
		}
	}
}

void levelQueueReader()	{

	Keyword current;
	KeywordQueue kQueue = getKWQueue(NULL);
	current = kQueue->start;
	while(current != NULL)	{
		switch(current->lCommand)	{
			case wave:
					if(kQueue->start == current)	{
						printf("wave\n");
						iprint(returnPropertyValue(current,dTime));
						waveCreatorCommand(current);
						current = removeLink(current);
					} else { 
						current = current->next;
					}
					break;
			case makeEnemy:
					if(createEnemyCommand(current))	{
						current = removeLink(current);
					} else {
						return;
					}
					break;
			case delay:
					setCreateEnemyGroupDelay(returnPropertyValue(current,dTime));
					current = removeLink(current);
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
        //printf("freeing start \n");
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
	printf("exit\n");
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

int createEnemyCommand(Keyword makeEnemy)	{

	if(checkClock(singleEnemyCreated,ENEMYSPAWNCOOLDOWN) && checkClock(groupDelay,getEnemyGroupDelay()))	{
		setCreateEnemyGroupDelay(0); //!setting delay back to zero
		createSpecificEnemy(returnPropertyValue(makeEnemy,enemyType),returnPropertyValue(makeEnemy,enemyLevel),returnPropertyValue(makeEnemy,entrance));
		//createSpecificEnemy(makeEnemy->propertiesList[0]->propertyValue,makeEnemy->propertiesList[1]->propertyValue,makeEnemy->propertiesList[2]->propertyValue);

		return 1;
	} 
	return 0;

}

void waveCreatorCommand(Keyword waveKeyWord)	{
	printf("checking wave delay\n");
	iprint(returnPropertyValue(waveKeyWord,dTime));
	int enemyNum;
   	for(enemyNum = 0; enemyNum < waveKeyWord->propertiesList[2]->propertyValue; enemyNum++)	{
		breakDownWaveCommand(waveKeyWord->propertiesList,waveKeyWord->nProperties);
	}
	printf("adding delay\n");
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
	printf("setting tower position\n");
	addTowerPosNode(setTower->propertiesList[0]->propertyValue,setTower->propertiesList[1]->propertyValue);

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
	printf("Set wave total\n");
	setTotalWaveNo(setWaveTotal->propertiesList[0]->propertyValue);
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
			printf("empty queue\n");
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
		printf("checking keyword\n");	
		Keyword newKey = createKeyword();
		if(!strcmp(token,"towerPos"))	{
			addKWtoQueue(newKey); 
			newKey->lCommand = makeTowerP;
			printf("assigned tower pos keyword\n");
		} else if(!strcmp(token,"totalWaves"))	{
			addKWtoQueue(newKey); 
			newKey->lCommand = totalWaves;
			printf("assigned total Waves keyword\n");
		} else if(!strcmp(token,"wave"))	{
			addKWtoQueue(newKey); 
			newKey->lCommand = wave;
			printf("assigned Waves keyword\n");
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
		sprint(token);
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
