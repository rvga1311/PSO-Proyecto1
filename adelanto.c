// create hello world c program
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ncurses.h>

#define HERO_WON -999
// define enum of HeroAction type where can MOVE_LEFT, MOVE_RIGHT, MOVE_UP, MOVE_DOWN, ATTACK, IDLE
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

UserHeroAction lastUserAction = IDLE;
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
    pthread_t action;
} MONSTER;
pthread_mutex_t lockMAP;
pthread_mutex_t lockHero;

roomCoord *Coords;
HEROE Hero;
MONSTER *monsterArray;
ROOM **MAP;

int heroHealth;
int heroAttack;
int size;
int hasWon;

int isNotVoid(int x, int y);
int isOtherMonsterThere(int x, int y);
int checkIfOccupied(int x, int y);

void Input()
{
    char teclaPulsada = 0;
    for (int i = 0; i < 12; i++)
    {
        teclaPulsada = getch();
        if (teclaPulsada != ERR)
        {
            break;
        }
        usleep(10000);
    }

    switch (teclaPulsada)
    {
    case 'a':
        lastUserAction = MOVE_LEFT;
        break;
    case 'd':
        lastUserAction = MOVE_RIGHT;
        break;
    case 'w':
        lastUserAction = MOVE_UP;
        break;
    case 's':
        lastUserAction = MOVE_DOWN;
        break;
    case 'e':
        lastUserAction = PICK_TREASURE;
        break;
    case 'k':
        lastUserAction = ATTACK;
        break;
    default:
        // // printf("NO MOVE\n");
        lastUserAction = IDLE;
        break;
    }
}

void print_monsters()
{
    for (int i = 0; i < size / 2; i++)
    {
        printw("Monster (%d, %d): %d\n", monsterArray[i].positionX, monsterArray[i].positionY, monsterArray[i].health);
    }
}

void print_end()
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (MAP[i][j].isVoid != 1 && MAP[i][j].isEnd == 1)
            {

                printw("(%d, %d)\n: ", i, j);
                printw("\tisEnd: %d\n", MAP[i][j].isEnd);
                printw("\tisVoid: %d\n", MAP[i][j].isVoid);
                printw("\thasHero: %d\n", MAP[i][j].hasHero);
                printw("\thasMonster: %d\n", MAP[i][j].hasMonster);
                printw("\thasTrap: %d\n", MAP[i][j].hasTrap);
                printw("\thasTreasure: %d\n", MAP[i][j].hasTreasure);
                // if (MAP[i][j].isEnd && MAP[i][j].isVoid != 0) {
                //     printw("(%d, %d) is END\n", i, j);
                // }
            }
        }
    }
    refresh();
}
