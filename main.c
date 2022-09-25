#include "backend.h"
#include "frontend.h"

int availableKeyboard;
int close1;
struct room *rooms;
int flagDrawMap;
int gameStarted = 0;
ROOM **MAP;
int size = 0;
int difficulty;
int flagTrapActivated = 0;
int flagTreasurePicked = 0;
int playerTakeDamage = 0;
int playerAttackRat = 0;
int heroHealth = 0;
int heroAttack = 0;
int hasWon = 0;
MONSTER *monsterArray;
struct room *emptyRooms;
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

roomCoord *Coords;
HEROE Hero;
MONSTER *monsterArray;
ROOM **MAP;
UserHeroAction lastUserAction = IDLE;
struct player player1;

void *playerControlTime()
{

    availableKeyboard = 1;
    sleep(0.1);
    availableKeyboard = 0;

    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{

    // ==================== Escoger modo de juego ====================
    int difficulty;
    srand(time(0));

    while (size == 0)
    {

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

    emptyRooms = malloc(sizeof(struct room) * (size * size - size));

    pthread_t playerControlThread;

    // ================== Frontend ==================

    TTF_Init();

    font = TTF_OpenFont("Montserrat-Regular.ttf", 20);
    SDL_Color WhiteFont = {255, 255, 255};

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        printf("error initializing SDL: %s\n", SDL_GetError());
    }
    SDL_Window *win = SDL_CreateWindow("GAME",
                                       SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       (size * ROOM_SIZE) + 20 + ICON_SIZE, size * ROOM_SIZE, 0);
    SDL_SetWindowResizable(win, SDL_FALSE);

    Uint32 render_flags = SDL_RENDERER_ACCELERATED;

    rend = SDL_CreateRenderer(win, -1, render_flags);

    player1.sprite = "./Images/Hero/SpriteF.png";
    player1.positions_Num = size;
    drawMap(size);

    // ================== Sonido ==================
    Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);
    backgroundMusic = Mix_LoadMUS("./Music/background.mp3");
    Mix_VolumeMusic(30);
    Mix_PlayMusic(backgroundMusic, -1);

    openChestSound = Mix_LoadWAV("./Music/openChest.wav");
    playerTakeDmgSound = Mix_LoadWAV("./Music/playerTakeDmg.mp3");
    ratTakeDmgSound = Mix_LoadWAV("./Music/ratTakeDmg.wav");
    winSound = Mix_LoadWAV("./Music/win.wav");
    loseSound = Mix_LoadWAV("./Music/lose.wav");

    gameStarted = 1;

    // background
    SDL_Surface *background_Surface = IMG_Load("./Images/Room/background.png");
    SDL_Texture *background_Texture = SDL_CreateTextureFromSurface(rend, background_Surface);
    SDL_FreeSurface(background_Surface);
    SDL_Rect background_Rect;
    SDL_QueryTexture(background_Texture, NULL, NULL, &background_Rect.w, &background_Rect.h);
    background_Rect.w = (size * ROOM_SIZE) + 20 + ICON_SIZE;
    background_Rect.h = size * ROOM_SIZE;
    background_Rect.x = 0;
    background_Rect.y = 0;

    // Icono de espada
    SDL_Surface *attack_Icon_Surface = IMG_Load("./Images/Misc/sword.png");
    SDL_Texture *attack_Icon_Texture = SDL_CreateTextureFromSurface(rend, attack_Icon_Surface);
    SDL_FreeSurface(attack_Icon_Surface);
    SDL_Rect attack_Icon_Rect;
    SDL_QueryTexture(attack_Icon_Texture, NULL, NULL, &attack_Icon_Rect.w, &attack_Icon_Rect.h);
    attack_Icon_Rect.w = ICON_SIZE;
    attack_Icon_Rect.h = ICON_SIZE;
    attack_Icon_Rect.x = size * ROOM_SIZE + 10;
    attack_Icon_Rect.y = 0;
    SDL_RenderCopy(rend, attack_Icon_Texture, NULL, &attack_Icon_Rect);

    // Puntos de ataque
    sprintf(text, "%d", player1.attack_points);
    SDL_Surface *attack_Text_Surface = TTF_RenderText_Solid(font, text, WhiteFont);
    SDL_Texture *attack_Text_Texture = SDL_CreateTextureFromSurface(rend, attack_Text_Surface);
    SDL_FreeSurface(attack_Text_Surface);
    SDL_Rect attack_Text_Texture_rect;
    attack_Text_Texture_rect.x = size * ROOM_SIZE + 15;
    attack_Text_Texture_rect.y = attack_Icon_Rect.y + ICON_SIZE + 5;
    attack_Text_Texture_rect.w = ICON_SIZE - 10;
    attack_Text_Texture_rect.h = ICON_SIZE - 10;
    SDL_RenderCopy(rend, attack_Text_Texture, NULL, &attack_Text_Texture_rect);

    // Icono de vida
    SDL_Surface *health_Icon_Surface = IMG_Load("./Images/Misc/heart.png");
    SDL_Texture *health_Icon_Texture = SDL_CreateTextureFromSurface(rend, health_Icon_Surface);
    SDL_FreeSurface(health_Icon_Surface);
    SDL_Rect health_Icon_Rect;
    SDL_QueryTexture(health_Icon_Texture, NULL, NULL, &health_Icon_Rect.w, &health_Icon_Rect.h);
    health_Icon_Rect.w = ICON_SIZE;
    health_Icon_Rect.h = ICON_SIZE;
    health_Icon_Rect.x = size * ROOM_SIZE + 10;
    health_Icon_Rect.y = attack_Text_Texture_rect.y + ICON_SIZE + 25;
    SDL_RenderCopy(rend, attack_Icon_Texture, NULL, &health_Icon_Rect);

    // Puntos de vida
    sprintf(text, "%d", player1.health_points);
    SDL_Surface *health_Text_Surface = TTF_RenderText_Solid(font, text, WhiteFont);
    SDL_Texture *health_Text_Texture = SDL_CreateTextureFromSurface(rend, health_Text_Surface);
    SDL_FreeSurface(health_Text_Surface);
    SDL_Rect health_Text_Rect;
    health_Text_Rect.x = size * ROOM_SIZE + 15;
    health_Text_Rect.y = health_Icon_Rect.y + ICON_SIZE + 5;
    health_Text_Rect.w = ICON_SIZE - 10;
    health_Text_Rect.h = ICON_SIZE - 10;
    SDL_RenderCopy(rend, health_Text_Texture, NULL, &health_Text_Rect);

    // Rata hace daño
    SDL_Surface *ratAttack_Surface = IMG_Load("./Images/Misc/RatDamage.png");
    SDL_Texture *ratAttack_Texture = SDL_CreateTextureFromSurface(rend, ratAttack_Surface);
    SDL_FreeSurface(ratAttack_Surface);
    SDL_Rect ratAttack_Rect;
    SDL_QueryTexture(ratAttack_Texture, NULL, NULL, &ratAttack_Rect.w, &ratAttack_Rect.h);
    ratAttack_Rect.w = ICON_SIZE + 10;
    ratAttack_Rect.h = ICON_SIZE + 10;
    ratAttack_Rect.x = health_Icon_Rect.x - 5;
    ratAttack_Rect.y = health_Icon_Rect.y - 5;

    // Jugador hace daño
    SDL_Surface *playerAttack_Surface = IMG_Load("./Images/Misc/sword.png");
    SDL_Texture *playerAttack_Texture = SDL_CreateTextureFromSurface(rend, playerAttack_Surface);
    SDL_FreeSurface(playerAttack_Surface);
    SDL_Rect playerAttack_Rect;
    SDL_QueryTexture(playerAttack_Texture, NULL, NULL, &playerAttack_Rect.w, &playerAttack_Rect.h);
    playerAttack_Rect.w = ICON_SIZE - 10;
    playerAttack_Rect.h = ICON_SIZE - 10;
    SDL_RenderCopy(rend, playerAttack_Texture, NULL, &playerAttack_Rect);

    // controls animation loop

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
                    break;
                case SDL_SCANCODE_SPACE:
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

        SDL_RenderCopy(rend, background_Texture, NULL, &background_Rect);

        if (flagTrapActivated || flagTreasurePicked)
        {
            renderRoom(size);
            flagTrapActivated = 0;
            flagTreasurePicked = 0;
        }
        renderMap(size); // render map
        renderRat(size);
        drawplayer();                                                       // render player
        SDL_RenderCopy(rend, attack_Icon_Texture, NULL, &attack_Icon_Rect); // render attack icon

        // render attack points
        sprintf(text, "%d", heroAttack);
        attack_Text_Surface = TTF_RenderText_Solid(font, text, WhiteFont);
        attack_Text_Texture = SDL_CreateTextureFromSurface(rend, attack_Text_Surface);
        SDL_FreeSurface(attack_Text_Surface);
        SDL_RenderCopy(rend, attack_Text_Texture, NULL, &attack_Text_Texture_rect);

        SDL_RenderCopy(rend, health_Icon_Texture, NULL, &health_Icon_Rect); // render health icon

        // render health points
        sprintf(text, "%d", heroHealth);
        health_Text_Surface = TTF_RenderText_Solid(font, text, WhiteFont);
        health_Text_Texture = SDL_CreateTextureFromSurface(rend, health_Text_Surface);
        SDL_FreeSurface(health_Text_Surface);
        SDL_RenderCopy(rend, health_Text_Texture, NULL, &health_Text_Rect);

        if (playerTakeDamage)
        {
            SDL_RenderCopy(rend, ratAttack_Texture, NULL, &ratAttack_Rect);
        }

        if (playerAttackRat)
        {
            playerAttack_Rect.x = player1.hitbox.x;
            playerAttack_Rect.y = player1.hitbox.y;
            SDL_RenderCopy(rend, playerAttack_Texture, NULL, &playerAttack_Rect);
        }

        SDL_RenderPresent(rend);

        SDL_Delay(1000 / 30);
        if (heroHealth <= 0)
        {
            Mix_VolumeChunk(playerTakeDmgSound, 0);
            Mix_VolumeChunk(openChestSound, 0);
            Mix_FreeMusic(backgroundMusic);
            Mix_PlayChannel(-1, loseSound, 0);
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Game Over", "You lost the game", NULL);
            close1 = 1;
        }
        else if (hasWon)
        {
            Mix_FreeMusic(backgroundMusic);
            Mix_PlayChannel(-1, winSound, 0);
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

    free(rooms);
    free(emptyRooms);
    free(monsterArray);
    free(Coords);

    // destroy renderer
    SDL_DestroyRenderer(rend);

    // destroy window
    SDL_DestroyWindow(win);

    SDL_Quit();

    return 0;
}
