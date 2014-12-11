//
//  Display.c
//
//  All functions directly related to displaying the game
//
//

#include "../includes/Display.h"
#include "../includes/parser.h"
#include "../includes/tower.h"
#include <stdbool.h>

#define HEALTH 80

struct display {
    //Window objects
    SDL_Window  *window;
    SDL_Renderer *renderer;
    Uint32 sky, red;
    
    //background
    SDL_Surface *backgroundSurface;
    SDL_Texture *backgroundTexture;

    
    //Tower monitor objects
    SDL_Surface *towerMonitorSurface;
    SDL_Texture *towerMonitorTexture;
    SDL_Surface *towerMonitorTextSurface;
    SDL_Texture *towerMonitorTextTexture;
    SDL_Rect  towerMonitorRect;
    SDL_Rect  towerMonitorTextureRect;
    TTF_Font *towerMonitorFont;
    SDL_Color towerMonitorFontColour;
    
    //Stats bar objects
    SDL_Rect statsBarRect;
    SDL_Color statsBarColour;
    SDL_Surface *statsBarTextSurface;
    SDL_Texture *statsBarTextTexture;
    SDL_Rect statsBarTextureRect;
    TTF_Font *statsBarFont;
    SDL_Color statsBarFontColour;

    //Tower objects
    SDL_Surface *towerSurface[2];
    SDL_Texture *towerTexture[2];
    SDL_Surface *towerPositionSurface;
    SDL_Texture *towerPoistionTexture;
    
    //Enemy objects
    SDL_Surface *enemySurface[2];
    SDL_Texture *enemyTexture[2];
    
    //animation rect
    SDL_Rect    srcRect;
    SDL_Rect    rect;

    SDL_Event event;
    
    //animation
    SDL_Surface *circ1_Surface[2];
    SDL_Texture *circ1_Texture[2];
    SDL_Surface *circ2_Surface[2];
    SDL_Texture *circ2_Texture[2];
};

/*Functions prototypes for functions only used internally*/

void initTTF(void);
SDL_Surface *getInfoWindowTextSurface(char *outputString);
void crash(char *message);
void getWindowSize(int *w, int *h);
void drawRange(Display d, double cx, double cy, double r);
void init_pic(SDL_Renderer **rend, SDL_Surface **surface, SDL_Texture **texture, char *pic_name);
void check_load_images(SDL_Surface *surface, char *pic_name);
void presentCircuit(Display d,SDL_Texture *text[2], int x,int y,int w, int h, int frames, int pic_width, int pic_height, int anim_speed);
int getBackgroundWidth();
int getBackgroundHeight();

Display init_SDL(){
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0) crash("SDL_Init()");
    if(TTF_Init() != 0) crash("TTF_Init()");
    if(IMG_Init(0) != 0) crash("IMG_Init()");
    
    Display d = (Display)malloc(sizeof(struct display));
    d->window = SDL_CreateWindow("TOWER DEFENSE", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, FULLSCREEN_YN);
    getDisplayPointer(d);
    getWindowSize(&SCREEN_WIDTH_GLOBAL,&SCREEN_HEIGHT_GLOBAL);
    d->renderer = SDL_CreateRenderer(d->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    
    d->towerMonitorSurface = IMG_Load("info_monitor.png");
    d->towerMonitorTexture = SDL_CreateTextureFromSurface(d->renderer, d->towerMonitorSurface);
    d->towerMonitorRect = (SDL_Rect){TOWER_MONITOR_X, TOWER_MONITOR_Y, TOWER_MONITOR_WIDTH, TOWER_MONITOR_HEIGHT};
    d->towerMonitorTextureRect = (SDL_Rect) {TOWER_MONITOR_X + TOWER_TEXT_BORDER_X, TOWER_MONITOR_Y + TOWER_TEXT_BORDER_Y, 0, 0};
    d->towerMonitorFont = TTF_OpenFont("OpenSans-Regular.ttf", 10);
    if(d->towerMonitorFont == NULL) crash("TTF_(OpenFont)");
    d->towerMonitorFontColour.r = 0x7c, d->towerMonitorFontColour.g = 0xfc, d->towerMonitorFontColour.b = 0x00;
    
    d->statsBarRect = (SDL_Rect) {STATS_BAR_X, STATS_BAR_Y, STATS_BAR_WIDTH, STATS_BAR_HEIGHT};
    d->statsBarColour = (SDL_Color) {63, 63, 63, 0};
    d->statsBarTextureRect = (SDL_Rect) {STATS_BAR_X + STATS_BAR_BORDER_X, STATS_BAR_Y + STATS_BAR_BORDER_Y, STATS_BAR_WIDTH, STATS_BAR_HEIGHT};
    d->statsBarFont = TTF_OpenFont("OpenSans-Regular.ttf", 10);
    if(d->statsBarFont == NULL) crash("TTF_(OpenFont)");
    d->statsBarFontColour.r = 0xFF, d->statsBarFontColour.g = 0xFF, d->statsBarFontColour.b = 0xFF;
    
    

    putenv("SDL_VIDEODRIVER=dga");
    
    /*inititalize pictures (load picture to the texture)*/
    init_pic(&d->renderer, &d->backgroundSurface, &d->backgroundTexture, "map1.png");
    init_pic(&d->renderer, &d->enemySurface[0], &d->enemyTexture[0], "sdl2-spritesheet-actual.png");
    init_pic(&d->renderer, &d->enemySurface[1], &d->enemyTexture[1], "int_enemy_basic.png");
    init_pic(&d->renderer, &d->towerSurface[0], &d->towerTexture[0], "tower.png");
    init_pic(&d->renderer, &d->towerSurface[1], &d->towerTexture[1], "tower1.png");
    init_pic(&d->renderer, &d->towerPositionSurface, &d->towerPoistionTexture, "TowerLocationsA.png");
    init_pic(&d->renderer, &d->circ1_Surface[0], &d->circ1_Texture[0], "circ1_dark.png");
    init_pic(&d->renderer, &d->circ1_Surface[1], &d->circ1_Texture[1], "circ1_light.png");
    init_pic(&d->renderer, &d->circ2_Surface[0], &d->circ2_Texture[0], "circ3_dark.png");
    init_pic(&d->renderer, &d->circ2_Surface[1], &d->circ2_Texture[1], "circ3_light.png");
    getDisplayPointer(d);//store display ptr
    
    TTF_Font *font;
    font = TTF_OpenFont("OpenSans-Regular.ttf", 10);
    if (font == NULL) {
        fprintf(stderr, "TTF_OpenFont() Failed: ");
    }
    
    /*improves quality of font*/
    TTF_SetFontHinting(font, TTF_HINTING_LIGHT);
    
    return d;
}

int getBackgroundWidth() {
  Display d =getDisplayPointer(NULL);
  return d->backgroundSurface->w;
}

int getBackgroundHeight() {
  Display d =getDisplayPointer(NULL);
  return d->backgroundSurface->h;
}


/*call fucntion in the while loop to present all the animations*/
void presentAnimation(){
    Display d = getDisplayPointer(NULL);
    presentCircuit(d, d->circ1_Texture,100, 100, 30, 100, 5,385, 324, 300);
    presentCircuit(d, d->circ2_Texture,300, 300, 30, 60, 7,386, 195, 350);
}

void drawBackground(){
    Display d = getDisplayPointer(NULL);
    SDL_RenderCopy(d->renderer, d->backgroundTexture, NULL, NULL);
}

/*Draw tower position*/
void drawTowerPosition(int x, int y, int w, int h){
    Display d =getDisplayPointer(NULL);
    d->rect = (SDL_Rect) {x, y, w, h};
    SDL_RenderCopy(d->renderer, d->towerPoistionTexture, NULL, &d->rect);
}

/*present any animation with one staitc image at the back and one above it*/
void presentCircuit(Display d,SDL_Texture *text[2], int x,int y,int w, int h, int frames, int pic_width, int pic_height, int anim_speed){
    
    d->rect= (SDL_Rect) {x, y , w * frames, h};
    SDL_RenderCopy(d->renderer, text[0], NULL, &d->rect);
    
    Uint32 ticks = SDL_GetTicks();
    Uint32 sprite = (ticks / anim_speed) % frames;
    //srcrect runs through the actual pixels of the pixture
    d->srcRect = (SDL_Rect){ sprite * (pic_width/frames), 0, (pic_width/frames), pic_height};
    
    //dstrect size could be decided by the user
    d->rect = (SDL_Rect) {sprite * w + x, y, w, h};
    SDL_RenderCopy(d->renderer, text[1], &d->srcRect, &d->rect);
}


void init_pic(SDL_Renderer **rend, SDL_Surface **surface, SDL_Texture **texture, char *pic_name){
    *surface = IMG_Load(pic_name);
    check_load_images(*surface, pic_name);
    *texture = SDL_CreateTextureFromSurface(*rend, *surface);
}

Display getDisplayPointer(Display d)
{
    
	static Display disp;
    
	if(d != NULL)	{
		disp = d;
	}
    
	return disp;
}

/**
 Prints last SDL error message to stderr, along withb message included in first parameter.
 */
void crash(char *message) {
    fprintf(stderr, "%s: %s\n", message, SDL_GetError());
    SDL_Quit();
}


/*Tower and enemy graphics functions*/

/* fill variables with the width and height values of the screen*/
void getWindowSize(int *w, int *h){
    Display d = getDisplayPointer(NULL);
    SDL_GetWindowSize(d->window, w, h);
}

/*draw the range (circle)*/
void drawRange(Display d, double cx, double cy, double r){
    SDL_SetRenderDrawColor(d->renderer, 255, 0, 0, 255);
    double dx, dy;
    dx = floor(sqrt((1.0 * r ) ));
    SDL_RenderDrawLine(d->renderer, cx-dx, cy+r, cx+dx, cy+r);
    SDL_RenderDrawLine(d->renderer, cx-dx, cy-r, cx+dx, cy-r);
    for (dy = 1; dy <= r; dy += 1.0) {
        dx = floor(sqrt((2.0 * r * dy) - (dy * dy)));
        SDL_RenderDrawPoint(d->renderer, cx+dx, cy+r-dy);
        SDL_RenderDrawPoint(d->renderer, cx+dx, cy-r+dy);
        SDL_RenderDrawPoint(d->renderer, cx-dx, cy+r-dy);
        SDL_RenderDrawPoint(d->renderer, cx-dx, cy-r+dy);
    }
}


/*draw damage line from X & Y to target X Y> */
void drawLine(Display d, int X_from, int Y_from, int X_target, int Y_target)
{
    SDL_SetRenderDrawColor(d->renderer, 252, 1, 1, 255);
    SDL_RenderDrawLine(d->renderer, X_from, Y_from, X_target, Y_target);
}

//* draw an enemy at x and y coor with the health bar above it*/
void drawEnemy(Display d, int x, int y, int w, int h, int pic_width, int pic_height, double currentHealth, double maxHealth, int type, int frames, int anim_speed){
    Uint32 ticks = SDL_GetTicks();
    Uint32 sprite = (ticks / anim_speed) % frames;
    d->srcRect = (SDL_Rect){ sprite * (pic_width/frames), 0, (pic_width/frames), pic_height};
    d->rect = (SDL_Rect) {x, y, w, h};
    /*create animation by putting part of a spritesheet(image) into destination rect*/
    SDL_RenderCopy(d->renderer, d->enemyTexture[type], &d->srcRect, &d->rect);
    
    /*presenting and manipulating color and width of the health bar*/
    double color = (255*((double)currentHealth/maxHealth));
    SDL_SetRenderDrawColor(d->renderer, 0, color, 0, 255);
    double health = ((double)(currentHealth * HEALTH)/maxHealth);
    d->rect = (SDL_Rect) {x, y -20, health, 10};
    SDL_RenderFillRect(d->renderer, &d->rect);
}


/* draws the tower at x and y coor */
void drawTower(Display d, int x, int y, int w, int h, int range, int type){
    d->rect= (SDL_Rect) {x, y ,w, h};
    SDL_RenderCopy(d->renderer, d->towerTexture[type], NULL, &d->rect);
    SDL_SetRenderDrawColor(d->renderer, 0, 0, 0, 255);
    drawRange(d, x + (double)w/2, y + (double)h/2, range);
}



/*clear the screen before making any drawings */
void startFrame(Display d) {
    //Display d = getDisplayPointer(NULL);
    SDL_SetRenderDrawColor(d->renderer, 168, 230, 255, 255);
    SDL_RenderClear(d->renderer);
}

/*peresent everything renderer has to draw*/
void endFrame(Display d) {
    SDL_RenderPresent(d->renderer);
    SDL_Delay(20);
}

/* check whether image was loaded successfully */
void check_load_images(SDL_Surface *surface, char *pic_name){
    if(surface == NULL){
        printf("Cannot find %s\n", pic_name);
        SDL_Quit();
        exit(1);
    }
}

/*destroy everything */
void shutSDL(Display d){
    SDL_DestroyTexture(d->statsBarTextTexture);
    SDL_DestroyTexture(d->towerMonitorTexture);
    SDL_DestroyTexture(d->towerMonitorTextTexture);
 //   SDL_DestroyTexture(d->towerTexture[0]);
 //   SDL_DestroyTexture(d->enemyTexture);

 //   SDL_FreeSurface(d->enemySurface);
//    SDL_FreeSurface(d->towerSurface[0]);
    //SDL_FreeSurface(d->background);
    SDL_DestroyRenderer(d->renderer);
    SDL_DestroyWindow(d->window);
    SDL_Quit();
    IMG_Quit();
    TTF_Quit();
}

/*End of tower and enemy graphics functions */



/*Information window functions*/

/**
 Display empty tower monitor in bottom right corner of screen
 */
void displayTowerMonitor() {
    
    Display d = getDisplayPointer(NULL);
    
    SDL_RenderCopy(d->renderer, d->towerMonitorTexture, NULL, &(d->towerMonitorRect));
}

/**
 Display empty stats bar at top of screen
 */
void displayStatsBar() {
    
    Display d = getDisplayPointer(NULL);
    
    SDL_SetRenderDrawColor(d->renderer, d->statsBarColour.r, d->statsBarColour.g, d->statsBarColour.b, 0);
    SDL_RenderFillRect(d->renderer, &(d->statsBarRect));
    SDL_SetRenderDrawColor(d->renderer, 255, 255, 255, 0);
    SDL_RenderDrawRect(d->renderer, &(d->statsBarRect));
}

/**
Display output string in tower monitor
*/
void updateTowerMonitor(char *outputString) {
    Display d = getDisplayPointer(NULL);
    
    displayTowerMonitor();
    
    d->towerMonitorTextSurface = TTF_RenderText_Blended_Wrapped(d->towerMonitorFont, outputString, d->towerMonitorFontColour, TOWER_MONITOR_WIDTH - TOWER_TEXT_BORDER_X);
    if(d->towerMonitorTextSurface == NULL) crash("getInfoWindowTextSurface()");
    d->towerMonitorTextTexture = SDL_CreateTextureFromSurface(d->renderer, d->towerMonitorTextSurface);
    
    //Query text dimensions so text doesn't strech to whole screen
    int textW = 0;
    int textH = 0;
    SDL_QueryTexture(d->towerMonitorTextTexture, NULL, NULL, &textW, &textH);
    d->towerMonitorTextureRect.w = textW;
    d->towerMonitorTextureRect.h = textH;
    
    SDL_RenderCopy(d->renderer, d->towerMonitorTextTexture, NULL, &(d->towerMonitorTextureRect));
    
    SDL_FreeSurface(d->towerMonitorTextSurface);
    SDL_DestroyTexture(d->towerMonitorTextTexture);
    
}

/**
 Display output string in stats monitor
 */
void updateStatsBar(char *outputString) {
    Display d = getDisplayPointer(NULL);
    
    displayStatsBar();
    
    d->statsBarTextSurface = TTF_RenderText_Blended_Wrapped(d->statsBarFont, outputString, d->statsBarFontColour, STATS_BAR_WIDTH);
    if(d->statsBarTextSurface == NULL) crash("getInfoWindowTextSurface()");
    d->statsBarTextTexture = SDL_CreateTextureFromSurface(d->renderer, d->statsBarTextSurface);
    
    //Query text dimensions so text doesn't strech to whole screen
    int textW = 0;
    int textH = 0;

    SDL_QueryTexture(d->statsBarTextTexture, NULL, NULL, &textW, &textH);
    d->statsBarTextureRect.w = textW;
    d->statsBarTextureRect.h = textH;
    d->statsBarTextureRect.x = (SCREEN_WIDTH / 2) - (textW / 2);
    
    SDL_RenderCopy(d->renderer, d->statsBarTextTexture, NULL, &(d->statsBarTextureRect));
    
    SDL_FreeSurface(d->statsBarTextSurface);
    SDL_DestroyTexture(d->statsBarTextTexture);
    free(outputString);
    
}

/*End of information window functions*/


//Terminal functions
int processEvents(Display d){

    int done = 0;
    
    while(SDL_PollEvent(&d->event))
    {
        switch(d->event.type)
        {
            case SDL_WINDOWEVENT_CLOSE:
            {
                if(d->window)
                {
                    SDL_DestroyWindow(d->window);
                    d->window = NULL;
                    done = 1;
                }
            }
                break;
            case
            SDL_KEYDOWN:
            {
                switch(d->event.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        done = 1;
                        break;
                }
            }
                break;
            case SDL_QUIT:
                //quit out of the game
                done = 1;
                break;
        }
    }
    return done;
}


int terminal_window(Display d, char *pass, char *clear, char * inputCommand)
{
    //inputCommand=NULL;
    int done = 0;
    char *pass2;
    
    display_text(d, pass);
    
    int check = 0;
    SDL_Event *event = &d->event;
    check = (SDL_PollEvent(event));
    if(check != 0)
    {
        switch (event->type)
        {
            case SDL_TEXTINPUT:
            {
                strcat(pass, event->text.text);
                display_text(d, pass);
                break;
            }
            case SDL_KEYDOWN:
            {
                if(event->key.keysym.sym == SDLK_RETURN)
                {
                    if(strcmp(pass, clear) != 0)
                    {
                        display_text(d, pass);
                    }
                    // inputCommand = strdup2(pass);
                    
                    pass2 = pass + 2;
                    parse(pass2);
                    
                    strcpy(pass, clear);
                }
				if(event->key.keysym.sym == SDLK_BACKSPACE)
				{
					if(pass[strlen(pass) - 1] != '>')
					{
						pass[strlen(pass) - 1] = '\0';
					}
					display_text(d, pass);
				}
                switch(d->event.key.keysym.sym)
                {
                	case SDLK_ESCAPE:
                	done = 1;
                	break;
                
                }
                break;
            }
                //            case SDL_TEXTEDITING:
                //            {
                //                strcpy(event->edit.text, pass);
                //                event->edit.start = pass;
                //                event->edit.length = 32;
                //            }
                //default: printf("Event code = %d\n", event->type);
        }
    }
    return done;
}

void display_text(Display d, char *pass)
{
    int texW = 200, texH = 50;
    SDL_Rect dstrect = { 5, 5, texW, texH };
	SDL_Rect imagerect = {TERMINAL_WINDOW_X, TERMINAL_WINDOW_Y, TERMINAL_WINDOW_WIDTH, TERMINAL_WINDOW_HEIGHT};
    SDL_Surface *text, *imagesurface;
    SDL_Texture *newtexture, *imagetexture;
    SDL_Color text_color = {0, 255, 0};
    dstrect.x = TERMINAL_WINDOW_X + (TERMINAL_WINDOW_WIDTH / 10);
    dstrect.y = TERMINAL_WINDOW_Y + (TERMINAL_WINDOW_HEIGHT - (TERMINAL_WINDOW_HEIGHT / 7));
    
	imagesurface = IMG_Load("terminalwindow.png");
	imagetexture = SDL_CreateTextureFromSurface(d->renderer, imagesurface);
    text = TTF_RenderText_Solid(d->towerMonitorFont, pass, text_color);
    newtexture = SDL_CreateTextureFromSurface(d->renderer, text);
    if(newtexture == NULL)
    {
        printf("Panic\n");	
    }
    dstrect.w = text->w;
    dstrect.h = text->h;
    SDL_FreeSurface(text);
    SDL_QueryTexture(newtexture, NULL, NULL, &texW, &texH);
	SDL_RenderCopy(d->renderer, imagetexture, NULL, &imagerect);
    SDL_RenderCopy(d->renderer, newtexture, NULL, &dstrect);
	SDL_DestroyTexture(newtexture);
	SDL_DestroyTexture(imagetexture);
}

/* duplicates a string */
char *strdup2(char * s)
{
    size_t len = 1+strlen(s);//gets the size of s
    char *p = malloc(len);//allocates a block big enough to hold s
    
    return p ? memcpy(p, s, len) : NULL;//if p is non 0 ie malloc worked, then copy everything in s into p and return p. if p is NULL malloc didnt work so return NULL
}
//End of terminal functions

