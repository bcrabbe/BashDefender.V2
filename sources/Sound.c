//
//  Sound.c
//  Bash_Defender_V2
//
//  Created by Vlad Vyshnevskyy on 09/12/2014.
//  Copyright (c) 2014 Michael. All rights reserved.
//


#include "../includes/Display.h"
#include "../includes/parser.h"
#include "../includes/tower.h"
#include <stdbool.h>
#include <time.h>

#include <SDL2_mixer/SDL_mixer.h>
#include <SDL2/SDL.h>

struct sound{
    Mix_Music *background_music;
    Mix_Chunk *tower_sound[2];
    Mix_Chunk *enemy_sound;

};

typedef struct sound *Sound;

/* 
 *internal Sound functions 
 */
Sound getSoundPointer(Sound s);
void check_load_sound(void *music, char *sound_name);
void load_sound_effect (Mix_Chunk **effect, char *effect_name);

/*
 * initialize the sound
 */
void init_sound(){
    
    Sound s = (Sound)malloc(sizeof(struct sound));

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) < 0) {
        printf("Error openning audio mixer");
        SDL_Quit();
    }
    
    s->background_music = Mix_LoadMUS("sound.wav");

    check_load_sound((Mix_Music*)s->background_music, "sound.mp3");

    load_sound_effect(&s->enemy_sound, "death.wav");
    load_sound_effect(&s->tower_sound[0], "laser_sound1.wav");
    load_sound_effect(&s->tower_sound[1], "laser_sound2.wav");

    Mix_VolumeMusic(50);

    getSoundPointer(s);
}

/*
 *Load sound effect
 */
void load_sound_effect (Mix_Chunk **effect, char *effect_name) {
    *effect = Mix_LoadWAV(effect_name);
    check_load_sound((Mix_Chunk *)effect, effect_name);
    
}

/*
 *play tower sound by types (i.e laser or bullet...) 
 */
void towerSound(int type){
    Sound s = getSoundPointer(NULL);
    Mix_PlayChannel(-1, s->tower_sound[type], 0);
    Mix_VolumeChunk(s->tower_sound[type], 50);
}


/*
 *play enemy death sound 
 */
void enemyDeathSound(){
    Sound s = getSoundPointer(NULL);
    Mix_PlayChannel(-1, s->enemy_sound, 0);
    Mix_VolumeChunk(s->enemy_sound, 50);
}


/*
 *play background sound
 */
void playBackgroundSound(){
    SDL_Delay(20);
    Sound s = getSoundPointer(NULL);
    Mix_PlayMusic(s->background_music, -1);
}

/*
*check that sound was loaded correctly
*/
void check_load_sound(void *music, char *sound_name){
    if(music == NULL){
        printf("Cannot find %s\n", sound_name);
        SDL_Quit();
        exit(1);
    }
}

/*
 *singleton..
*/
Sound getSoundPointer(Sound s){
    static Sound sound;
    if(s != NULL)	{
        sound = s;
    }
    return sound;
}

/*
 *destroy everything
 */
void shutSound(){
    Sound s = getSoundPointer(NULL);
    Mix_FreeMusic(s->background_music);
    Mix_FreeChunk(s->enemy_sound);
    Mix_FreeChunk(s->tower_sound[0]);
    Mix_FreeChunk(s->tower_sound[1]);
    Mix_CloseAudio();
    Mix_Quit();
}
