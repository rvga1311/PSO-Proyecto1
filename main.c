#include "frontend.h"

int availableKeyboard;
int close1;
pthread_mutex_t lockKeyboard;
struct room *rooms;
int flagDrawMap;
int gameStarted = 0;

void *playerControlTime()
{
    pthread_mutex_lock(&lockKeyboard);
    availableKeyboard = 1;
    sleep(0.1);
    availableKeyboard = 0;
    pthread_mutex_unlock(&lockKeyboard);

    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{

    // ==================== Escoger modo de juego ====================
    int difficulty;
    size = 0;
    flagTrapActivated = 0;
    flagTreasurePicked = 0;
    playerTakeDamage = 0;
    srand(time(0));

    while (size == 0)
    {
        // print please enter a difficulty level
        printf("Please enter a difficulty level (1, 2, or 3): \n");

        int ret = scanf("%d", &difficulty);
        if (ret != 1)
        {
            printf("scanf() failed.\n");
        }
        int systemRet = system("clear");
        if (systemRet == -1)
        {
            printf("system() failed.\n");
        }

        switch (difficulty)
        {
        case 1:
            size = 10;
            printf("You have selected easy mode\n");
            MAP = createMatrix();

            break;
        case 2:
            size = 20;
            printf("You have selected medium mode\n");
            MAP = createMatrix();
            break;
        case 3:
            size = 30;
            printf("You have selected hard mode\n");
            MAP = createMatrix();
            break;

        default:;
            int ret = system("clear");
            if (ret == -1)
            {
                printf("system() failed.\n");
            }
            printf("Please enter a valid difficulty level (1, 2, or 3): \n");
            break;
        }
    }

    // ==================== Inicializar Hilo Jugador ====================
    close1 = 0;

    pthread_create(&Hero.heroAction, NULL, &heroActions, NULL);
    heroHealth = 5;
    heroAttack = 1;
    hasWon = 0;
    fillMonsterArray(MAP);
    for (int i = 0; i < size / 2; i++)
    {
        monsterArray[i].action = pthread_create(&monsterArray[i].action, NULL, &monstersActions, &monsterArray[i]);
    }
    // Malloc rooms struct
    rooms = malloc(sizeof(struct room) * size);

    pthread_t playerControlThread;

    // ================== Frontend ==================

    TTF_Init();

    font = TTF_OpenFont("Montserrat-Regular.ttf", 20);
    SDL_Color WhiteFont = {255, 255, 255};

    // returns zero on success else non-zero
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        printf("error initializing SDL: %s\n", SDL_GetError());
    }
    SDL_Window *win = SDL_CreateWindow("GAME", // creates a window
                                       SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       (size * ROOM_SIZE) + 10 + ICON_SIZE, size * ROOM_SIZE, 0);
    SDL_SetWindowResizable(win, SDL_FALSE);

    // triggers the program that controls
    // your graphics hardware and sets flags
    Uint32 render_flags = SDL_RENDERER_ACCELERATED;

    // creates a renderer to render our images
    rend = SDL_CreateRenderer(win, -1, render_flags);

    player1.sprite = "./Images/Hero/SpriteF.png";
    player1.positions_Num = size;
    player1.health_points = heroHealth;
    player1.attack_points = heroAttack;
    drawMap(size);

    gameStarted = 1;

    // Icono de espada
    SDL_Surface *attack_Icon_Surface = IMG_Load("./Images/Misc/sword.png");
    SDL_Texture *attack_Icon_Texture = SDL_CreateTextureFromSurface(rend, attack_Icon_Surface);
    SDL_FreeSurface(attack_Icon_Surface);
    SDL_Rect attack_Icon_Rect;
    SDL_QueryTexture(attack_Icon_Texture, NULL, NULL, &attack_Icon_Rect.w, &attack_Icon_Rect.h);
    attack_Icon_Rect.w = ICON_SIZE;
    attack_Icon_Rect.h = ICON_SIZE;
    attack_Icon_Rect.x = size * ROOM_SIZE;
    attack_Icon_Rect.y = 0;
    SDL_RenderCopy(rend, attack_Icon_Texture, NULL, &attack_Icon_Rect);

    // Puntos de ataque
    sprintf(text, "%d", player1.attack_points);
    SDL_Surface *attack_Text_Surface = TTF_RenderText_Solid(font, text, WhiteFont);
    SDL_Texture *attack_Text_Texture = SDL_CreateTextureFromSurface(rend, attack_Text_Surface);
    SDL_FreeSurface(attack_Text_Surface);
    SDL_Rect attack_Text_Texture_rect;                               // create a rect
    attack_Text_Texture_rect.x = size * ROOM_SIZE + 5;               // controls the rect's x coordinate
    attack_Text_Texture_rect.y = attack_Icon_Rect.y + ICON_SIZE + 5; // controls the rect's y coordinte
    attack_Text_Texture_rect.w = ICON_SIZE - 10;                     // controls the width of the rect
    attack_Text_Texture_rect.h = ICON_SIZE - 10;                     // controls the height of the rect
    SDL_RenderCopy(rend, attack_Text_Texture, NULL, &attack_Text_Texture_rect);

    // Icono de vida
    SDL_Surface *health_Icon_Surface = IMG_Load("./Images/Misc/heart.png");
    SDL_Texture *health_Icon_Texture = SDL_CreateTextureFromSurface(rend, health_Icon_Surface);
    SDL_FreeSurface(health_Icon_Surface);
    SDL_Rect health_Icon_Rect;
    SDL_QueryTexture(health_Icon_Texture, NULL, NULL, &health_Icon_Rect.w, &health_Icon_Rect.h);
    health_Icon_Rect.w = ICON_SIZE;
    health_Icon_Rect.h = ICON_SIZE;
    health_Icon_Rect.x = size * ROOM_SIZE;
    health_Icon_Rect.y = attack_Text_Texture_rect.y + ICON_SIZE + 25;
    SDL_RenderCopy(rend, attack_Icon_Texture, NULL, &health_Icon_Rect);

    // Puntos de vida
    sprintf(text, "%d", player1.health_points);
    SDL_Surface *health_Text_Surface = TTF_RenderText_Solid(font, text, WhiteFont);
    SDL_Texture *health_Text_Texture = SDL_CreateTextureFromSurface(rend, health_Text_Surface);
    SDL_FreeSurface(health_Text_Surface);
    SDL_Rect health_Text_Rect;                               // create a rect
    health_Text_Rect.x = size * ROOM_SIZE + 5;               // controls the rect's x coordinate
    health_Text_Rect.y = health_Icon_Rect.y + ICON_SIZE + 5; // controls the rect's y coordinte
    health_Text_Rect.w = ICON_SIZE - 10;                     // controls the width of the rect
    health_Text_Rect.h = ICON_SIZE - 10;                     // controls the height of the rect
    SDL_RenderCopy(rend, health_Text_Texture, NULL, &health_Text_Rect);

    // Icono de espada
    SDL_Surface *damagePlayer_Surface = IMG_Load("./Images/Misc/sword.png");
    SDL_Texture *damagePlayer_Texture = SDL_CreateTextureFromSurface(rend, damagePlayer_Surface);
    SDL_FreeSurface(damagePlayer_Surface);
    SDL_Rect damagePlayer_Rect;
    SDL_QueryTexture(damagePlayer_Texture, NULL, NULL, &damagePlayer_Rect.w, &damagePlayer_Rect.h);
    damagePlayer_Rect.w = ICON_SIZE - 10;
    damagePlayer_Rect.h = ICON_SIZE - 10;

    // controls animation loop

    // speed of box
    // int speed = 300;

    // animation loop
    while (!close1)
    {

        SDL_Event event;
        int possibleMovement[2];
        int waitPlayer = 0;

        // Events management
        while (SDL_PollEvent(&event))

        {
            switch (event.type)
            {

            case SDL_QUIT:
                // handling of close1 button
                close1 = 1;
                break;

            case SDL_KEYDOWN:
                switch (event.key.keysym.scancode)
                {
                case SDL_SCANCODE_W:
                case SDL_SCANCODE_UP:

                    possibleMovement[0] = player1.hitbox.x;
                    possibleMovement[1] = player1.hitbox.y - ROOM_SIZE;
                    if (!availableKeyboard && valid_move(possibleMovement, player1.positions_Num, player1.positions))
                    {
                        player1.sprite = "./Images/Hero/SpriteB.png";
                        player1.hitbox.y -= ROOM_SIZE;
                        lastUserAction = MOVE_UP;
                        waitPlayer = 1;
                    }

                    break;
                case SDL_SCANCODE_A:
                case SDL_SCANCODE_LEFT:
                    possibleMovement[0] = player1.hitbox.x - ROOM_SIZE;
                    possibleMovement[1] = player1.hitbox.y;
                    if (!availableKeyboard && valid_move(possibleMovement, player1.positions_Num, player1.positions))
                    {
                        player1.sprite = "./Images/Hero/SpriteL.png";
                        player1.hitbox.x -= ROOM_SIZE;
                        lastUserAction = MOVE_LEFT;
                        waitPlayer = 1;
                    }

                    break;
                case SDL_SCANCODE_S:
                case SDL_SCANCODE_DOWN:
                    possibleMovement[0] = player1.hitbox.x;
                    possibleMovement[1] = player1.hitbox.y + ROOM_SIZE;
                    if (!availableKeyboard && valid_move(possibleMovement, player1.positions_Num, player1.positions))
                    {
                        player1.sprite = "./Images/Hero/SpriteF.png";
                        player1.hitbox.y += ROOM_SIZE;
                        lastUserAction = MOVE_DOWN;
                        waitPlayer = 1;
                    }

                    break;
                case SDL_SCANCODE_D:
                case SDL_SCANCODE_RIGHT:

                    possibleMovement[0] = player1.hitbox.x + ROOM_SIZE;
                    possibleMovement[1] = player1.hitbox.y;
                    if (!availableKeyboard && valid_move(possibleMovement, player1.positions_Num, player1.positions))
                    {
                        player1.sprite = "./Images/Hero/SpriteR.png";
                        player1.hitbox.x += ROOM_SIZE;
                        lastUserAction = MOVE_RIGHT;
                        waitPlayer = 1;
                    }

                    break;
                case SDL_SCANCODE_E:
                    lastUserAction = PICK_TREASURE;
                    // printf("Player %d,%d\n", player1.hitbox.x / 33, player1.hitbox.y / 33);
                    break;
                case SDL_SCANCODE_K:
                    lastUserAction = ATTACK;
                    break;
                default:
                    lastUserAction = IDLE;
                    break;
                }
            }
        }
        if (waitPlayer)
        {
            pthread_create(&playerControlThread, NULL, &playerControlTime, NULL);
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

        Hero.positionX = player1.hitbox.y / ROOM_SIZE;
        Hero.positionY = player1.hitbox.x / ROOM_SIZE;

        // clears the screen
        SDL_RenderClear(rend);
        // SDL_RenderCopy(rend, tex, NULL, &dest);
        if (flagTrapActivated || flagTreasurePicked)
        {
            int lockIdx = getRoomLockIdx(player1.hitbox.y / ROOM_SIZE, player1.hitbox.x / ROOM_SIZE);
            pthread_mutex_lock(&Coords[lockIdx].lock);
            // pthread_mutex_lock(&lockMap);
            renderRoom(size, player1.hitbox.x, player1.hitbox.y);
            flagTrapActivated = 0;
            flagTreasurePicked = 0;
            pthread_mutex_unlock(&Coords[lockIdx].lock);
            // pthread_mutex_unlock(&lockMap);
        }
        renderMap(size); // render map
        renderRat(size);
        drawplayer();                                                       // render player
        SDL_RenderCopy(rend, attack_Icon_Texture, NULL, &attack_Icon_Rect); // render attack icon

        if (playerTakeDamage)
        {
            damagePlayer_Rect.x = player1.hitbox.x;
            damagePlayer_Rect.y = player1.hitbox.y;
            SDL_RenderCopy(rend, damagePlayer_Texture, NULL, &damagePlayer_Rect);
        }

        // render attack points
        sprintf(text, "%d", player1.attack_points);
        attack_Text_Surface = TTF_RenderText_Solid(font, text, WhiteFont);
        attack_Text_Texture = SDL_CreateTextureFromSurface(rend, attack_Text_Surface);
        SDL_FreeSurface(attack_Text_Surface);
        SDL_RenderCopy(rend, attack_Text_Texture, NULL, &attack_Text_Texture_rect);

        SDL_RenderCopy(rend, health_Icon_Texture, NULL, &health_Icon_Rect); // render health icon

        // render health points
        sprintf(text, "%d", player1.health_points);
        health_Text_Surface = TTF_RenderText_Solid(font, text, WhiteFont);
        health_Text_Texture = SDL_CreateTextureFromSurface(rend, health_Text_Surface);
        SDL_FreeSurface(health_Text_Surface);
        SDL_RenderCopy(rend, health_Text_Texture, NULL, &health_Text_Rect);

        // triggers the double buffers
        // for multiple rendering
        SDL_RenderPresent(rend);

        // calculates to 60 fps
        SDL_Delay(1000 / 30);
        if (heroHealth <= 0)
        {
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Game Over", "You lost the game", NULL);
            close1 = 1;
        }
        else if (hasWon)
        {
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Game Over", "You won the game", NULL);
            close1 = 1;
        }
    }

    // destroy texture
    destroyMap(size);
    SDL_DestroyTexture(player1.texture);
    SDL_DestroyTexture(&attack_Icon_Rect);
    SDL_DestroyTexture(&attack_Text_Texture_rect);
    SDL_DestroyTexture(&health_Icon_Rect);
    SDL_DestroyTexture(&health_Text_Rect);

    pthread_mutex_destroy(&lockKeyboard);

    // destroy renderer
    SDL_DestroyRenderer(rend);

    // destroy window
    SDL_DestroyWindow(win);

    // close1 SDL
    SDL_Quit();

    return 0;
}
