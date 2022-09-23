#ifndef _FRONTEND_H_
#define _FRONTEND_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_ttf.h>
#include "backend.h"

#define ROOM_SIZE 33
#define PLAYER_SIZE 30
#define ICON_SIZE 50

SDL_Renderer *rend;
SDL_Surface *surface;
TTF_Font *font;
char text[5];

Mix_Music *backgroundMusic;
Mix_Chunk *openChestSound;
Mix_Chunk *playerTakeDmgSound;
Mix_Chunk *ratTakeDmgSound;
Mix_Chunk *winSound;
Mix_Chunk *loseSound;

int chestPlayerPosX;
int chestPlayerPosY;

int gameStarted;

struct player
{
    char *sprite;
    SDL_Surface *surface;
    SDL_Texture *texture;
    SDL_Rect hitbox;
    int positions_Num;
    int positions[30][2];
    int attack_points;
    int health_points;
};

struct player player1;

struct room
{
    char *sprite;
    SDL_Surface *surface;
    SDL_Texture *texture;
    SDL_Rect location;
    int i;
    int j;
};
struct room *rooms;
struct room *emptyRooms;
void drawplayer();
void renderMap(int size);
int valid_move(int val[2], int size, int array[30][2]);
void drawMap(int size);
void destroyMap(int size);
void renderRoom(int size);
void renderRat(int size);

#endif