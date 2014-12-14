#ifndef actionQueueDataStructure_h

/*---------- Custom Headers -----------*/
#include "../includes/tower.h"
#include "../includes/enemy.h"
#include "../includes/actionQueueDataStructure.h"

#define ENDOFSTRING '\0'
#define READCHAR	'/'
typedef enum levelCommand { makeTowerP= 0, 
							totalWaves = 1, 
							wave = 2, 
							delay = 3 
} levelCommand;

typedef enum property {	x = 0, 
						y = 1, 
						total = 2, 
						waveID = 3, 
						enemyType = 4,
					    numberOfEnemies = 5,	
						dTime = 5 
} property;

typedef struct keyword *Keyword;
typedef struct keywordProp *KeywordProp;
typedef struct keywordQueue *KeywordQueue;
typedef struct parseLineArray *ParseLineArray;

/*---------- Functions ----------*/
void createKeywordQueue();
KeywordQueue getKWQueue(KeywordQueue kwQueue);
void initLevel();
void endLevel();
void addKeyWordToken(char *token);
void readLevelSettingsFile(char *file);
int checkProperty(char *token);
int checkForSemiColon(char *token);
void addKWtoQueue(Keyword nwKW);
char* expandCBuffer(char *toExpand, int currSize);
char* getToken(char *line);
int validateLine(char *Line, int nWords);
void addProperty(property p);
void addValue(char *token);
void createLevel();
void printQueue();
void makeTowerCommand(Keyword setTower);
void setWaveTotalCommand(Keyword setWaveTotal);
void waveCreatorCommand(Keyword waveKeyWord);
Keyword removeLink(Keyword current);
void initialQueueReader();
void levelQueueReader();
#endif
