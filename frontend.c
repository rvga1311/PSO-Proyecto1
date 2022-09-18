#include "frontend.h"
#include "backend.h"

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

void renderMap(int size)
{
    for (int i = 0; i < size; i++)
    {
        SDL_RenderCopy(rend, rooms[i].texture, NULL, &rooms[i].location);
    }
}

void renderRat(int size)
{
    for (int i = 0; i < size / 2; i++)
    {
        monsterArray[i].surface = IMG_Load("./Images/Misc/rat.png");
        monsterArray[i].texture = SDL_CreateTextureFromSurface(rend, monsterArray[i].surface);
        SDL_FreeSurface(monsterArray[i].surface);
        SDL_QueryTexture(monsterArray[i].texture, NULL, NULL, &monsterArray[i].hitbox.w, &monsterArray[i].hitbox.h);

        monsterArray[i].hitbox.w = ROOM_SIZE;
        monsterArray[i].hitbox.h = ROOM_SIZE;

        if (monsterArray[i].health > 0)
        {
            SDL_RenderCopy(rend, monsterArray[i].texture, NULL, &monsterArray[i].hitbox);
        }
    }
}

int valid_move(int val[2], int size, int array[30][2])
{
    for (int i = 0; i <= size; i++)
    {
        if (array[i][0] == val[0] && array[i][1] == val[1])
        {
            if (isOtherMonsterThere(val[1] / ROOM_SIZE, val[0] / ROOM_SIZE) == 0)
            {
                return 1;
            }
        }
    }
    return 0;
}

void renderRoom(int size, int x, int y)
{
    for (int i = 0; i < size; i++)
    {
        if (rooms[i].location.x == x && rooms[i].location.y == y)
        {
            printf("flags treasure %d, trap %d\n", flagTreasurePicked, flagTrapActivated);
            if (flagTreasurePicked)
            {
                rooms[i].surface = IMG_Load("./Images/Room/RoomTreasureChest.jpg");
            }
            else if (flagTrapActivated)
            {
                rooms[i].surface = IMG_Load("./Images/Room/RoomTrapChest.jpg");
            }

            rooms[i].texture = SDL_CreateTextureFromSurface(rend, rooms[i].surface);
            SDL_FreeSurface(rooms[i].surface);
            SDL_QueryTexture(rooms[i].texture, NULL, NULL, &rooms[i].location.w, &rooms[i].location.h);
            rooms[i].location.w = ROOM_SIZE;
            rooms[i].location.h = ROOM_SIZE;
        }
    }
}

void drawMap(int size)
{

    int currentX = 0;
    int currentY = 0;
    int counter = 0;

    for (int i = 0; i < size; i++)
    {
        for (int y = 0; y < size; y++)
        {
            if (MAP[i][y].isVoid == 0)
            {
                if (MAP[i][y].hasTrap == 1 || MAP[i][y].hasTreasure == 1)
                {
                    rooms[counter].surface = IMG_Load("./Images/Room/RoomCloseChest.jpg");
                    printf("Chest %d, %d\n", y, i);
                }
                else if (MAP[i][y].isBeginning == 1)
                {
                    if (!gameStarted)
                    {
                        player1.hitbox.x = currentX;
                        player1.hitbox.y = currentY;
                    }

                    rooms[counter].surface = IMG_Load("./Images/Room/InitialRoom.jpg");
                }
                else if (MAP[i][y].isEnd == 1)
                {
                    rooms[counter].surface = IMG_Load("./Images/Room/FinalRoom.jpg");
                }
                else if (MAP[i][y].hadTrap == 1)
                {
                    rooms[counter].surface = IMG_Load("./Images/Room/RoomTrapChest.jpg");
                }
                else if (MAP[i][y].hadTreasure == 1)
                {
                    rooms[counter].surface = IMG_Load("./Images/Room/RoomTreasureChest.jpg");
                }
                else
                {
                    rooms[counter].surface = IMG_Load("./Images/Room/Room.jpg");
                }
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

void destroyMap(int size)
{
    for (int i = 0; i < size; i++)
    {
        SDL_DestroyTexture(rooms[i].texture);
    }
}