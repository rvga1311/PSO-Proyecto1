#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>

#define ROOM_SIZE 33
#define PLAYER_SIZE 30

SDL_Renderer *rend;
SDL_Surface *surface;

struct player
{
    char *sprite;
    SDL_Surface *surface;
    SDL_Texture *texture;
    SDL_Rect hitbox;
    int positions_Num;
    int positions[30][2];
};

struct player player1;

void drawplayer()
{
    player1.surface = IMG_Load(player1.sprite);
    player1.texture = SDL_CreateTextureFromSurface(rend, player1.surface);
    SDL_FreeSurface(player1.surface);
    SDL_QueryTexture(player1.texture, NULL, NULL, &player1.hitbox.w, &player1.hitbox.h);
    player1.hitbox.w = PLAYER_SIZE;
    player1.hitbox.h = PLAYER_SIZE;
    SDL_RenderCopy(rend, player1.texture, NULL, &player1.hitbox);
}

struct room
{
    char *sprite;
    SDL_Surface *surface;
    SDL_Texture *texture;
    SDL_Rect location;
    int i;
    int j;
};

void renderMap(int size, struct room rooms[size])
{
    for (int i = 0; i < size; i++)
    {
        SDL_RenderCopy(rend, rooms[i].texture, NULL, &rooms[i].location);
    }
}

int valid_move(int val[2], int size, int array[30][2])
{
    for (int i = 0; i <= size; i++)
    {
        if (array[i][0] == val[0] && array[i][1] == val[1])
        {
            return 1;
        }
    }
    return 0;
}

void drawMap(int size, struct room rooms[size])
{

    // Make a identity matrix of size 10x10
    int prueba[10][10] = {{1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
                          {0, 1, 1, 0, 0, 0, 0, 0, 0, 0},
                          {0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
                          {0, 0, 1, 1, 0, 0, 0, 0, 0, 0},
                          {0, 0, 1, 1, 1, 0, 0, 0, 0, 0},
                          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};

    // // Make a identity matrix of size 30 x 30
    // int prueba[30][30];
    // for (int i = 0; i < 30; i++)
    // {
    //     for (int j = 0; j < 30; j++)
    //     {
    //         if (i == j)
    //         {
    //             prueba[i][j] = 1;
    //         }
    //         else
    //         {
    //             prueba[i][j] = 0;
    //         }
    //     }
    // }

    // prueba[0][0] = 1;
    // prueba[0][1] = 1;
    // prueba[0][2] = 1;
    // prueba[2][0] = 1;
    // prueba[2][1] = 1;
    // prueba[2][2] = 1;

    int currentX = 0;
    int currentY = 0;
    int counter = 0;

    for (int i = 0; i < size; i++)
    {
        for (int y = 0; y < size; y++)
        {
            if (prueba[i][y] == 1)
            {
                rooms[counter].surface = IMG_Load("./Images/Room.jpg");
                rooms[counter].texture = SDL_CreateTextureFromSurface(rend, rooms[counter].surface);
                SDL_FreeSurface(rooms[counter].surface);
                SDL_QueryTexture(rooms[counter].texture, NULL, NULL, &rooms[counter].location.w, &rooms[counter].location.h);

                rooms[counter].location.w = ROOM_SIZE;
                rooms[counter].location.h = ROOM_SIZE;
                rooms[counter].location.x = currentX;
                rooms[counter].location.y = currentY;
                rooms[counter].i = i;
                rooms[counter].j = y;
                player1.positions[counter][0] = currentX;
                player1.positions[counter][1] = currentY;
                counter++;
            }
            currentX += ROOM_SIZE;
        }
        currentX = 0;
        currentY += ROOM_SIZE;
    }
}

void destroyMap(int size, struct room rooms[size])
{
    for (int i = 0; i < size; i++)
    {
        SDL_DestroyTexture(rooms[i].texture);
    }
}

int main(int argc, char *argv[])
{

    int size = 10;
    struct room rooms[size];

    // returns zero on success else non-zero
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        printf("error initializing SDL: %s\n", SDL_GetError());
    }
    SDL_Window *win = SDL_CreateWindow("GAME", // creates a window
                                       SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       size * ROOM_SIZE, size * ROOM_SIZE, 0);
    SDL_SetWindowResizable(win, SDL_FALSE);

    // triggers the program that controls
    // your graphics hardware and sets flags
    Uint32 render_flags = SDL_RENDERER_ACCELERATED;

    // creates a renderer to render our images
    rend = SDL_CreateRenderer(win, -1, render_flags);

    player1.sprite = "./Images/SpriteF.png";
    player1.hitbox.x = 0;
    player1.hitbox.y = 0;
    player1.positions_Num = size;
    drawMap(size, rooms);

    // controls animation loop
    int close = 0;

    // speed of box
    int speed = 300;
    int possibleMovement[2];

    // animation loop
    while (!close)
    {
        SDL_Event event;

        // Events management
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {

            case SDL_QUIT:
                // handling of close button
                close = 1;
                break;

            case SDL_KEYDOWN:
                // keyboard API for key pressed
                switch (event.key.keysym.scancode)
                {
                case SDL_SCANCODE_W:
                case SDL_SCANCODE_UP:

                    possibleMovement[0] = player1.hitbox.x;
                    possibleMovement[1] = player1.hitbox.y - ROOM_SIZE;
                    if (valid_move(possibleMovement, player1.positions_Num, player1.positions))
                    {
                        player1.sprite = "./Images/SpriteB.png";
                        player1.hitbox.y -= ROOM_SIZE;
                    }

                    break;
                case SDL_SCANCODE_A:
                case SDL_SCANCODE_LEFT:
                    possibleMovement[0] = player1.hitbox.x - ROOM_SIZE;
                    possibleMovement[1] = player1.hitbox.y;
                    if (valid_move(possibleMovement, player1.positions_Num, player1.positions))
                    {
                        player1.sprite = "./Images/SpriteL.png";
                        player1.hitbox.x -= ROOM_SIZE;
                    }

                    break;
                case SDL_SCANCODE_S:
                case SDL_SCANCODE_DOWN:
                    possibleMovement[0] = player1.hitbox.x;
                    possibleMovement[1] = player1.hitbox.y + ROOM_SIZE;
                    if (valid_move(possibleMovement, player1.positions_Num, player1.positions))
                    {
                        player1.sprite = "./Images/SpriteF.png";
                        player1.hitbox.y += ROOM_SIZE;
                    }

                    break;
                case SDL_SCANCODE_D:
                case SDL_SCANCODE_RIGHT:

                    possibleMovement[0] = player1.hitbox.x + ROOM_SIZE;
                    possibleMovement[1] = player1.hitbox.y;
                    if (valid_move(possibleMovement, player1.positions_Num, player1.positions))
                    {
                        player1.sprite = "./Images/SpriteR.png";
                        player1.hitbox.x += ROOM_SIZE;
                    }
                    break;
                default:
                    break;
                }
            }
        }

        // right boundary
        if (player1.hitbox.x + player1.hitbox.w > 1000)
            player1.hitbox.x = 1000 - player1.hitbox.w;

        // left boundary
        if (player1.hitbox.x < 0)
            player1.hitbox.x = 0;

        // bottom boundary
        if (player1.hitbox.y + player1.hitbox.h > 1000)
            player1.hitbox.y = 1000 - player1.hitbox.h;

        // upper boundary
        if (player1.hitbox.y < 0)
            player1.hitbox.y = 0;

        // clears the screen
        SDL_RenderClear(rend);
        // SDL_RenderCopy(rend, tex, NULL, &dest);
        renderMap(size, rooms);
        drawplayer();

        // triggers the double buffers
        // for multiple rendering
        SDL_RenderPresent(rend);

        // calculates to 60 fps
        SDL_Delay(1000 / 30);
    }

    // destroy texture
    destroyMap(size, rooms);
    SDL_DestroyTexture(player1.texture);

    // destroy renderer
    SDL_DestroyRenderer(rend);

    // destroy window
    SDL_DestroyWindow(win);

    // close SDL
    SDL_Quit();

    return 0;
}
