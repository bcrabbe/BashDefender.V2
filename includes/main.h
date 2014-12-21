typedef enum tutPhase {

	phaseOne = 1,
	phaseTwo = 2,
	phaseThree = 3,
	phaseFour = 4,
	phaseFive =5,
	phaseSix = 6,
	phaseSeven = 7,
	phaseEight = 8,
	phaseNine = 9,
	phaseTen = 10,
	phaseEleven = 11
} tutPhase;


void quitGame();
void startLevel(Display d, int *restart);
void testing();
void tutorialLevel(Display d,int *restart);
/*----------System Test Functions-----------*/

void parseToQueueTesting();
void parseToTowerTesting();
void towerToEnemyTesting();
void enemyToGamePropertiesTesting();
//void testparseterm();

/*----------Testing Suites-----------*/
void testEnemyDeath();
void testParseToTower();
void testValidParses();
void testEnemyInRange();
void testTerminalWindowInput();
void testAbilities();
//void testmenuscreen();
void testtermwin();
void testpsx();
//void testmscreen();
void testkillall();
char *test_psx_string(char *psxlist);
char *test_string_1(char *pass2);
char *test_string_2(char *clear);
