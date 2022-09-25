#ifndef _BACKEND_H_
#define _BACKEND_H_

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string.h>
#include <unistd.h>

#define HERO_WON -999

typedef enum HeroAction
{
    MOVE_LEFT,
    MOVE_RIGHT,
    MOVE_UP,
    MOVE_DOWN,
    ATTACK,
    PICK_TREASURE,
    IDLE,
    QUIT
} UserHeroAction;

typedef struct ROOMS
{
    int isVoid;
    int isBeginning;
    int isEnd;
    int hasTreasure;
    int hadTreasure;
    int hasTrap;
    int hadTrap;
    int hasHero;
    int hasMonster;
} ROOM;

typedef struct roomCoords
{
    int axisX;
    int axisY;
    pthread_mutex_t lock;
} roomCoord;

typedef struct HEROES
{
    int positionX;
    int positionY;
    pthread_t heroAction;
} HEROE;

typedef struct MONSTERS
{
    int health;
    int positionX;
    int positionY;
    int isResting;
    SDL_Surface *surface;
    SDL_Texture *texture;
    SDL_Rect hitbox;
    pthread_t action;
} MONSTER;

extern roomCoord *Coords;
extern HEROE Hero;
extern MONSTER *monsterArray;
extern ROOM **MAP;
extern UserHeroAction lastUserAction;

extern int heroHealth;
extern int heroAttack;
extern int size;
extern int hasWon;
extern int flagTreasurePicked;
extern int flagTrapActivated;
extern int playerTakeDamage;
extern int playerAttackRat;

int isNotVoid(int x, int y);
int isOtherMonsterThere(int x, int y);
int checkIfOccupied(int x, int y);
void *monstersActions(void *imMonster);
int isNotVoid(int x, int y);
void printMonsterArray();
int whichMonster();
int isOtherMonsterThere(int x, int y);
ROOM **fillMonsterArray(ROOM **matrix);
int checkIfSurrounded(int x, int y);
int checkIfSurroundedByEntrance(int x, int y);
int checkIfOccupied(int x, int y);
void pushIntCoords(int x, int y);
;
void instanceCoords();
int randomNumber(int number);
ROOM **createMatrix();
void *heroActions();
void printMap();
void *playerTakeDmgAnimation(void *arg);
void *playerAttackAnimation(void *arg);

#endif