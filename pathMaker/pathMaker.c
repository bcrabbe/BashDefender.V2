
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include <stdio.h>
#include <stdlib.h>

#define CLEARINPUT while(getchar() != '\n') {}

#define CAPTURE_DELAY 50 //delay between mouse position capture events
#define ARG_NUM 2 //number of arguments that should be passed to file

// gcc `sdl2-config --cflags` -pedantic -Wall -std=c99 -O4 -o pathMaker pathMaker.c `sdl2-config --libs`

struct display {
  SDL_Window  *window;
  SDL_Renderer *renderer;
  
  SDL_Texture *backgroundTexture;
  SDL_Surface *backgroundSurface;
  
  int capture;
  int finished;
} ;

typedef struct display *Display;


void checkForEvents(Display d);
void crash(char *message);

int main(int argc, char *argv[]) {

  int levelNum, pathNum;
  
  printf("Enter level number: ");
  while(scanf("%d", &levelNum) != 1) {
    printf("try again: ");
    CLEARINPUT
  }
  
    CLEARINPUT
  printf("Enter path number: ");
  while(scanf("%d", &pathNum) != 1) {
    printf("try again: ");
    CLEARINPUT
  }
  

  char levelNumStr[10];
  sprintf(levelNumStr, "%d", levelNum);
  
  char pathNumStr[10];
  sprintf(pathNumStr, "%d", pathNum);
  
  char *filePath = calloc((strlen("../data/levels/lvl_") + strlen(levelNumStr) + strlen("/paths/path_") +strlen(pathNumStr) + strlen(".txt") ), sizeof(char) );
  if(filePath == NULL) {
    fprintf(stderr,"Unable to calloc space for path filePath\n");
    exit(1);
  }
  
  strcat(filePath, "../data/levels/lvl_");
  strcat(filePath, levelNumStr);
  strcat(filePath, "/paths/path_");
  strcat(filePath, pathNumStr);
  strcat(filePath, ".txt");
  
  int x, y;
  
  
    CLEARINPUT
  char cont;
  FILE *fp = fopen(filePath, "r");
  if(fp != NULL) {
    printf("Aready file located at '%s', overwrite? [y/n] ", filePath);
    while(scanf("%c", &cont) != 1 || (cont != 'y' && cont != 'n')) {
      printf("try again\n");
      CLEARINPUT
    }
  }
  if(cont == 'n') {
    fprintf(stderr,"OK, program exited\n");
    fclose(fp);
    exit(1);
  }
  fclose(fp);
  
  
  if(SDL_Init(SDL_INIT_EVERYTHING) != 0) crash("SDL_Init()");
  if(TTF_Init() != 0) crash("TTF_Init()");
  if(IMG_Init(0) != 0) crash("IMG_Init()");
  Display d = (Display)malloc(sizeof(struct display));
  
  d->finished = 0;
  d->capture = 0;
  d->backgroundSurface = IMG_Load("map.png");
  
  d->window = SDL_CreateWindow("Path Drawing tool", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, d->backgroundSurface->w, d->backgroundSurface->h, 0);
  d->renderer = SDL_CreateRenderer(d->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  
  d->backgroundTexture = SDL_CreateTextureFromSurface(d->renderer, d->backgroundSurface);
  
  SDL_RenderCopy(d->renderer, d->backgroundTexture, NULL, NULL);
  SDL_RenderPresent(d->renderer);
  
  remove("tmp.txt");
  FILE *t = fopen("tmp.txt", "w");
  fclose(t);
  
  FILE *tmpFP = fopen("tmp.txt","w+");
  if(tmpFP == NULL) {
    fprintf(stderr,"must have file named 'tmp.txt' in current folder\n");
    exit(1);
  }
  int numberOfCoords = 0;
  while (!d->finished) {
    if(d->capture) {
      checkForEvents(d);
      SDL_Delay(CAPTURE_DELAY);
      SDL_GetMouseState(&x,&y);
      printf("Step Number: %d,   x: %d, y: %d\n", numberOfCoords, x, y);
      fprintf(tmpFP, "%d,%d\n", x, y);
      numberOfCoords++;
    }
    checkForEvents(d);
  }
  
  remove(filePath);
  fp = fopen(filePath,"w");
  
  char c;
  
  fprintf(fp,"%d\n", numberOfCoords);
  
  rewind(tmpFP);
  while((c = fgetc(tmpFP)) != EOF) {
    printf("%c", c);
    fprintf(fp,"%c", c);
  }
  
  
  fclose(tmpFP);
  fclose(fp);
  
  remove("tmp.txt");
  
  
  
  return 0;

}

/**
 Prints last SDL error message to stderr, along withb message included in first parameter.
 */
void crash(char *message) {
    fprintf(stderr, "%s: %s\n", message, SDL_GetError());
    SDL_Quit();
}





// Gobble all events & ignore most
void checkForEvents(Display d) {

  SDL_Event event;
  while(SDL_PollEvent(&event))  {      
    switch (event.type){
      case SDL_QUIT:
        d->finished = 1;
        break;
      case SDL_MOUSEBUTTONDOWN :
        if (event.button.button == SDL_BUTTON_LEFT) {
          d->capture = 1;
        }
        break;
      case SDL_MOUSEBUTTONUP :
        if (event.button.button == SDL_BUTTON_LEFT) {
          d->capture = 0;
        }
        break;
    }
  }
}






