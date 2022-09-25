#ifndef _FRONTEND_H_
#define _FRONTEND_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_ttf.h>

#define ROOM_SIZE 33
#define PLAYER_SIZE 30
#define ICON_SIZE 50

extern SDL_Renderer *rend;
extern SDL_Surface *surface;
extern TTF_Font *font;
extern char text[5];

extern Mix_Music *backgroundMusic;
extern Mix_Chunk *openChestSound;
extern Mix_Chunk *playerTakeDmgSound;
extern Mix_Chunk *ratTakeDmgSound;
extern Mix_Chunk *winSound;
extern Mix_Chunk *loseSound;

extern int chestPlayerPosX;
extern int chestPlayerPosY;

extern int gameStarted;

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

extern struct player player1;

struct room
{
    char *sprite;
    SDL_Surface *surface;
    SDL_Texture *texture;
    SDL_Rect location;
    int i;
    int j;
};
extern struct room *rooms;
extern struct room *emptyRooms;
void drawplayer();
void renderMap(int size);
int valid_move(int val[2], int size, int array[30][2]);
void drawMap(int size);
void destroyMap(int size);
void renderRoom(int size);
void renderRat(int size);

#endif