#include "backend.h"
#include "frontend.h"

UserHeroAction lastUserAction = IDLE;

void *monstersActions(void *imMonster)
{
    MONSTER *actualMonster = (MONSTER *)imMonster;
    while (1)
    {

        int sleepTime = rand() % 5;
        int newX;
        int newY;
        // DECIDE PARA DONDE MOVERSE
        int newDirection;
        int keepGoing = 0;
        if (actualMonster->health < 1)
        {

            pthread_mutex_lock(&lockMAP);
            MAP[actualMonster->positionX][actualMonster->positionY].hasMonster = 0;
            pthread_mutex_unlock(&lockMAP);
            actualMonster->positionX = -1;
            actualMonster->positionY = -1;
            pthread_exit(0);
        }
        if (actualMonster->positionX == Hero.positionX && actualMonster->positionY == Hero.positionY)
        {

            // lock Health
            pthread_mutex_lock(&lockHero);
            // ATACAR
            heroHealth -= 1;

            // printf("El heroe ha sido atacado por un monstruo y ahora tiene %d de vida\n", heroHealth);
            // unlock
            pthread_mutex_unlock(&lockHero);
        }
        else
        {
            while (keepGoing == 0)
            {
                newDirection = rand() % 4;
                switch (newDirection)
                {
                    // MOVERSE ABAJO
                case 0:
                    if (isNotVoid(actualMonster->positionX + 1, actualMonster->positionY) != 0 && isNotVoid(actualMonster->positionX + 1, actualMonster->positionY) != size - 1 && isNotVoid(actualMonster->positionX + 1, actualMonster->positionY) != -1 && isOtherMonsterThere(actualMonster->positionX + 1, actualMonster->positionY) == 0)
                    {
                        // SE MUEVE
                        newX = actualMonster->positionX + 1;
                        newY = actualMonster->positionY;
                        keepGoing = 1;
                    }
                    break;
                    // MOVERSE ARRIBA
                case 1:
                    if (isNotVoid(actualMonster->positionX - 1, actualMonster->positionY) != 0 && isNotVoid(actualMonster->positionX - 1, actualMonster->positionY) != size - 1 && isNotVoid(actualMonster->positionX - 1, actualMonster->positionY) != -1 && isOtherMonsterThere(actualMonster->positionX - 1, actualMonster->positionY) == 0)
                    {
                        // SE MUEVE
                        newX = actualMonster->positionX - 1;
                        newY = actualMonster->positionY;
                        keepGoing = 1;
                    }
                    break;
                    // MOVERSE A LA DERECHA
                case 2:
                    if (isNotVoid(actualMonster->positionX, actualMonster->positionY + 1) != 0 && isNotVoid(actualMonster->positionX, actualMonster->positionY + 1) != size - 1 && isNotVoid(actualMonster->positionX, actualMonster->positionY + 1) != -1 && isOtherMonsterThere(actualMonster->positionX, actualMonster->positionY + 1) == 0)
                    {
                        // SE MUEVE
                        newX = actualMonster->positionX;
                        newY = actualMonster->positionY + 1;
                        keepGoing = 1;
                    }
                    break;
                    // MOVERSE A LA IZQUIERDA

                case 3:
                    if (isNotVoid(actualMonster->positionX, actualMonster->positionY - 1) != 0 && isNotVoid(actualMonster->positionX, actualMonster->positionY - 1) != size - 1 && isNotVoid(actualMonster->positionX, actualMonster->positionY - 1) != -1 && isOtherMonsterThere(actualMonster->positionX, actualMonster->positionY - 1) == 0)
                    {
                        // SE MUEVE
                        newX = actualMonster->positionX;
                        newY = actualMonster->positionY - 1;
                        keepGoing = 1;
                    }
                    break;

                default:
                    break;
                }
            }

            // se bloquea el MAP
            pthread_mutex_lock(&lockMAP);
            MAP[actualMonster->positionX][actualMonster->positionY].hasMonster = 0;
            MAP[newX][newY].hasMonster = 1;
            pthread_mutex_unlock(&lockMAP);

            actualMonster->positionX = newX;
            actualMonster->positionY = newY;
        }

        // SE PONE A MIMIR
        actualMonster->isResting = 1;
        usleep((sleepTime + 1) * 1000000);
        actualMonster->isResting = 0;
        // YA SE LEVANTÓ
    }
}

int isNotVoid(int x, int y)
{
    for (int i = 0; i < size; i++)
    {
        if (Coords[i].axisX == x && Coords[i].axisY == y)
        {
            return i;
        }
    }
    return -1;
}

void printMonsterArray()
{
    int i;
    for (i = 0; i < size / 2; i++)
    {
    }
}

int whichMonster()
{
    int i;
    for (i = 0; i < size / 2; i++)
    {
        if (monsterArray[i].positionX == Hero.positionX && monsterArray[i].positionY == Hero.positionY)
        {
            return i;
        }
    }
    return -1;
}

int isOtherMonsterThere(int x, int y)
{
    int i;
    for (i = 0; i < size / 2; i++)
    {
        if (monsterArray[i].positionX == x && monsterArray[i].positionY == y)
        {
            return 1;
        }
    }
    return 0;
}

ROOM **fillMonsterArray(ROOM **matrix)
{
    // printw("CREANDO MONSTRUOS\n");
    refresh();
    monsterArray = (MONSTER *)malloc((size / 2) * sizeof(MONSTER));
    // printw("FINAL MONSTRUOS \n");
    refresh();
    int monstersCreated = 0;
    int randomPosition;
    while (monstersCreated < (size / 2))
    {
        randomPosition = rand() % size;
        if (randomPosition == 0 || randomPosition == (size - 1))
        {
            randomPosition = 1;
        }
        if (isOtherMonsterThere(Coords[randomPosition].axisX, Coords[randomPosition].axisY) == 0)
        {
            monsterArray[monstersCreated].health = 3;
            monsterArray[monstersCreated].positionX = Coords[randomPosition].axisX;
            monsterArray[monstersCreated].positionY = Coords[randomPosition].axisY;
            monsterArray[monstersCreated].surface = IMG_Load("./Images/Misc/rat.png");
            monsterArray[monstersCreated].texture = SDL_CreateTextureFromSurface(rend, monsterArray[monstersCreated].surface);
            SDL_FreeSurface(monsterArray[monstersCreated].surface);
            SDL_QueryTexture(monsterArray[monstersCreated].texture, NULL, NULL, &monsterArray[monstersCreated].hitbox.w, &monsterArray[monstersCreated].hitbox.h);

            monsterArray[monstersCreated].hitbox.w = ROOM_SIZE;
            monsterArray[monstersCreated].hitbox.h = ROOM_SIZE;
            monsterArray[monstersCreated].hitbox.x = Coords[randomPosition].axisY * ROOM_SIZE;
            monsterArray[monstersCreated].hitbox.y = Coords[randomPosition].axisX * ROOM_SIZE;
            monstersCreated++;
            matrix[Coords[randomPosition].axisX][Coords[randomPosition].axisY].hasMonster = 1;
        }
    }
    return matrix;
}

int checkIfSurrounded(int x, int y)
{
    int blockers = 0;
    if (x - 1 < 0)
    {
        blockers++;
    }
    if (x + 1 > size - 1)
    {
        blockers++;
    }
    if (y - 1 < 0)
    {
        blockers++;
    }
    if (y + 1 > size - 1)
    {
        blockers++;
    }
    if (checkIfOccupied(x - 1, y))
    {
        blockers++;
    }
    if (checkIfOccupied(x + 1, y))
    {
        blockers++;
    }
    if (checkIfOccupied(x, y - 1))
    {
        blockers++;
    }
    if (checkIfOccupied(x, y + 1))
    {
        blockers++;
    }

    return blockers;
}

int checkIfOccupied(int x, int y)
{
    int i;
    for (i = 0; i < size; i++)
    {
        if (Coords[i].axisX == x && Coords[i].axisY == y)
        {
            return 1;
        }
    }
    return 0;
}

void pushIntCoords(int x, int y)
{
    for (int i = 0; i < size; i++)
    {
        if (Coords[i].axisX == -1 && Coords[i].axisY == -1)
        {
            Coords[i].axisX = x;
            Coords[i].axisY = y;
            break;
        }
    }
}

void instanceCoords()
{
    for (int i = 0; i < size; i++)
    {
        Coords[i].axisX = -1;
        Coords[i].axisY = -1;
    }
}

int randomNumber(int number)
{
    // choose a random value between 0 and size

    int random = rand() % number;
    return random;
}

ROOM **createMatrix()
{
    // instance struct ROOM

    ROOM baseRoom;
    baseRoom.isVoid = 1;
    // create a vector for roomCoord
    Coords = malloc(size * sizeof(roomCoord));
    instanceCoords();
    // create a matrix size of difficulty in dynamic memory
    ROOM **matrix = (ROOM **)malloc(size * sizeof(ROOM *));
    for (int i = 0; i < size; i++)
        matrix[i] = (ROOM *)malloc(size * sizeof(ROOM));

    // fill the matrix with voidRooms
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            matrix[i][j] = baseRoom;

    // first room
    int x = randomNumber(size);
    int y = randomNumber(size);
    int random1;
    int random2;
    int RandomRoom;
    Hero.positionX = x;
    Hero.positionY = y;
    matrix[x][y].isVoid = 0;
    matrix[x][y].isBeginning = 1;
    matrix[x][y].hasHero = 1;
    pushIntCoords(x, y);

    int instanciatedRooms = 1;
    while (instanciatedRooms < size - 1)
    {
        // usleep(10000);
        //  // printw("RANDOM NUM - \n");
        //  refresh();
        RandomRoom = randomNumber(instanciatedRooms);
        // printf("RANDOM NUM - %d\n", RandomRoom);
        if (RandomRoom == size - 1)
            RandomRoom--;

        x = Coords[RandomRoom].axisX;
        y = Coords[RandomRoom].axisY;
        // create a path from the first room to the last room
        random1 = randomNumber(4);
        random2 = randomNumber(10);
        if (random1 == 0)
        {
            // printw("RANDOM 0 with x: %d and y: %d- \n", x, y);
            refresh();

            // move up

            if (x > 0)
            {

                x--;
                if (x > -1 && (checkIfOccupied(x, y) == 0) && (checkIfSurrounded(x, y) < 4) && (checkIfSurrounded(x + 1, y) < 3) && (checkIfSurrounded(x - 1, y) < 3) && (checkIfSurrounded(x, y + 1) < 3) && (checkIfSurrounded(x, y - 1) < 3))
                {
                    if (random2 == 0 || random2 == 1 || random2 == 2 || random2 == 3)
                    {
                        matrix[x][y].hasTrap = 1;
                    }
                    if (random2 == 4 || random2 == 5 || random2 == 6)
                    {
                        matrix[x][y].hasTreasure = 1;
                    }
                    matrix[x][y].isVoid = 0;
                    instanciatedRooms++;
                    pushIntCoords(x, y);
                }
                else
                {
                    x++;
                }
            }
        }
        if (random1 == 1)
        {
            // move down
            // printw("RANDOM 1 with x: %d and y: %d- \n", x, y);
            refresh();

            if (x < size - 1)
            {

                x++;
                if (x < size && (checkIfOccupied(x, y) == 0) && (checkIfSurrounded(x, y) < 4) && (checkIfSurrounded(x + 1, y) < 3) && (checkIfSurrounded(x - 1, y) < 3) && (checkIfSurrounded(x, y + 1) < 3) && (checkIfSurrounded(x, y - 1) < 3))
                {
                    if (random2 == 0 || random2 == 1 || random2 == 2 || random2 == 3)
                    {
                        matrix[x][y].hasTrap = 1; // se instancia una habitacion con trampa
                    }
                    if (random2 == 4 || random2 == 5 || random2 == 6)
                    {
                        matrix[x][y].hasTreasure = 1; // se instancia una habitacion sin trampa
                    }
                    matrix[x][y].isVoid = 0; // se instancia una habitacion vacia
                    instanciatedRooms++;
                    pushIntCoords(x, y);
                }
                else
                {
                    x--;
                }
            }
        }
        if (random1 == 2)
        {
            // move left
            // printw("RANDOM 2 with x: %d and y: %d- \n", x, y);
            refresh();

            if (y > 0)
            {

                y--;
                if (y > -1 && (checkIfOccupied(x, y) == 0) && (checkIfSurrounded(x, y) < 4) && (checkIfSurrounded(x + 1, y) < 3) && (checkIfSurrounded(x - 1, y) < 3) && (checkIfSurrounded(x, y + 1) < 3) && (checkIfSurrounded(x, y - 1) < 3))
                {
                    if (random2 == 0 || random2 == 1 || random2 == 2 || random2 == 3)
                    {
                        matrix[x][y].hasTrap = 1;
                    }
                    if (random2 == 4 || random2 == 5 || random2 == 6)
                    {
                        matrix[x][y].hasTreasure = 1;
                    }
                    matrix[x][y].isVoid = 0;
                    instanciatedRooms++;
                    pushIntCoords(x, y);
                }
                else
                {
                    y++;
                }
            }
        }
        if (random1 == 3)
        {
            // move right
            // printw("RANDOM 3 with x: %d and y: %d- \n", x, y);
            refresh();
            if (y < size - 1)
            {
                y++;
                if (y < size && (checkIfOccupied(x, y) == 0) && (checkIfSurrounded(x, y) < 4) && (checkIfSurrounded(x + 1, y) < 3) && (checkIfSurrounded(x - 1, y) < 3) && (checkIfSurrounded(x, y + 1) < 3) && (checkIfSurrounded(x, y - 1) < 3))
                {
                    if (random2 == 0 || random2 == 1 || random2 == 2 || random2 == 3)
                    {
                        matrix[x][y].hasTrap = 1;
                    }
                    if (random2 == 4 || random2 == 5 || random2 == 6)
                    {
                        matrix[x][y].hasTreasure = 1;
                    }
                    matrix[x][y].isVoid = 0;
                    instanciatedRooms++;
                    pushIntCoords(x, y);
                }
                else
                {
                    y--;
                }
            }
        }
    }
    // printw("FINAL CAMINO\n");

    //------------------end of path creation------------------//
    sleep(1);
    int randomEnd;

    instanciatedRooms = 0;
    while (instanciatedRooms < 1)
    {
        randomEnd = randomNumber(size);
        if (randomEnd == size - 1)
            randomEnd--;

        x = Coords[randomEnd].axisX;
        y = Coords[randomEnd].axisY;
        // create a path from the first room to the last room
        int random = randomNumber(4);
        if (random == 0)
        {
            // move up
            if (x > 0)
            {
                x--;
                if (x > -1 && (checkIfOccupied(x, y) == 0) && (checkIfSurrounded(x, y) < 4) && (checkIfSurrounded(x + 1, y) < 3) && (checkIfSurrounded(x - 1, y) < 3) && (checkIfSurrounded(x, y + 1) < 3) && (checkIfSurrounded(x, y - 1) < 3))
                {
                    matrix[x][y].isVoid = 0;
                    matrix[x][y].isEnd = 1;
                    instanciatedRooms++;
                    pushIntCoords(x, y);
                }
                else
                {
                    x++;
                }
            }
        }
        if (random == 1)
        {
            // move down
            if (x < size - 1)
            {
                x++;
                if (x < size && (checkIfOccupied(x, y) == 0) && (checkIfSurrounded(x, y) < 4) && (checkIfSurrounded(x + 1, y) < 3) && (checkIfSurrounded(x - 1, y) < 3) && (checkIfSurrounded(x, y + 1) < 3) && (checkIfSurrounded(x, y - 1) < 3))
                {
                    matrix[x][y].isVoid = 0;
                    matrix[x][y].isEnd = 1;
                    instanciatedRooms++;
                    pushIntCoords(x, y);
                }
                else
                {
                    x--;
                }
            }
        }
        if (random == 2)
        {
            // move left
            if (y > 0)
            {
                y--;
                if (y > -1 && (checkIfOccupied(x, y) == 0) && (checkIfSurrounded(x, y) < 4) && (checkIfSurrounded(x + 1, y) < 3) && (checkIfSurrounded(x - 1, y) < 3) && (checkIfSurrounded(x, y + 1) < 3) && (checkIfSurrounded(x, y - 1) < 3))
                {
                    matrix[x][y].isVoid = 0;
                    matrix[x][y].isEnd = 1;
                    instanciatedRooms++;
                    pushIntCoords(x, y);
                }
                else
                {
                    y++;
                }
            }
        }
        if (random == 3)
        {
            // move right
            if (y < size - 1)
            {
                y++;
                if (y < size && (checkIfOccupied(x, y) == 0) && (checkIfSurrounded(x, y) < 4) && (checkIfSurrounded(x + 1, y) < 3) && (checkIfSurrounded(x - 1, y) < 3) && (checkIfSurrounded(x, y + 1) < 3) && (checkIfSurrounded(x, y - 1) < 3))
                {
                    matrix[x][y].isVoid = 0;
                    matrix[x][y].isEnd = 1;
                    instanciatedRooms++;
                    pushIntCoords(x, y);
                }
                else
                {
                    y--;
                }
            }
        }
    }
    sleep(1);
    return matrix;
}

void *damageAnimation(void *arg)
{
    playerTakeDamage = 1;
    sleep(1);
    playerTakeDamage = 0;
    pthread_exit(NULL);
}

void *heroActions()
{
    // lock MAP
    while (1)
    {
        // Input();
        // pthread_mutex_lock(&lockMAP);
        int actualRoom = 0;
        printf("Las Action %d\n", lastUserAction);
        switch (lastUserAction)
        {
        case PICK_TREASURE:
            printf("Pos %d %d\n", Hero.positionX, Hero.positionY);
            if (MAP[Hero.positionX][Hero.positionY].hasTreasure == 1)
            {
                int treasureType = rand() % 2;
                if (treasureType)
                {
                    // printf("You found a treasure!\n");
                    heroAttack++;
                    player1.attack_points++;
                }
                else
                {
                    // lock heroHealth
                    pthread_mutex_lock(&lockHero);
                    heroHealth++;
                    player1.health_points++;
                    pthread_mutex_unlock(&lockHero);
                    // printf("You didn't find any treasure!\n");
                }
                pthread_mutex_lock(&lockMAP);
                flagTrapActivated = 0;
                flagTreasurePicked = 1;
                MAP[Hero.positionX][Hero.positionY].hasTreasure = 0;
                MAP[Hero.positionX][Hero.positionY].hadTreasure = 1;
                pthread_mutex_unlock(&lockMAP);
            }
            else if (MAP[Hero.positionX][Hero.positionY].hasTrap == 1)
            {
                printf("You stepped on a trap!\n");
                pthread_mutex_lock(&lockHero);
                heroHealth--;
                player1.health_points--;

                pthread_t thread;
                pthread_create(&thread, NULL, &damageAnimation, NULL);
                pthread_mutex_unlock(&lockHero);

                pthread_mutex_lock(&lockMAP);
                flagTrapActivated = 1;
                flagTreasurePicked = 0;
                MAP[Hero.positionX][Hero.positionY].hasTrap = 0;
                MAP[Hero.positionX][Hero.positionY].hadTrap = 1;
                pthread_mutex_unlock(&lockMAP);
            }

            break;
        case ATTACK:;
            int currentMonsterIdx = whichMonster();
            if (currentMonsterIdx != -1 && monsterArray[currentMonsterIdx].isResting == 1)
            {
                // lock monsterHealth
                monsterArray[currentMonsterIdx].health = monsterArray[currentMonsterIdx].health - heroAttack;
                pthread_mutex_unlock(&lockMAP);
                // unlock MAP
            }
            // printf("You attacked the monster!\n");

            break;

        case MOVE_LEFT:
            if (Hero.positionY > 0 && MAP[Hero.positionX][Hero.positionY - 1].isVoid != 1 && isOtherMonsterThere(Hero.positionX, Hero.positionY - 1) == 0)
            {
                pthread_mutex_lock(&lockMAP);
                MAP[Hero.positionX][Hero.positionY].hasHero = 0;
                Hero.positionY--;
                MAP[Hero.positionX][Hero.positionY].hasHero = 1;
                pthread_mutex_unlock(&lockMAP);
            }

            break;
        case MOVE_RIGHT:
            if (Hero.positionY < size - 1 && MAP[Hero.positionX][Hero.positionY + 1].isVoid != 1 && isOtherMonsterThere(Hero.positionX, Hero.positionY + 1) == 0)
            {
                pthread_mutex_lock(&lockMAP);
                MAP[Hero.positionX][Hero.positionY].hasHero = 0;
                Hero.positionY++;
                MAP[Hero.positionX][Hero.positionY].hasHero = 1;
                pthread_mutex_unlock(&lockMAP);
            }

            break;
        case MOVE_UP:
            if (Hero.positionX > 0 && MAP[Hero.positionX - 1][Hero.positionY].isVoid != 1 && isOtherMonsterThere(Hero.positionX - 1, Hero.positionY) == 0)
            {

                pthread_mutex_lock(&lockMAP);
                MAP[Hero.positionX][Hero.positionY].hasHero = 0;
                Hero.positionX--;
                MAP[Hero.positionX][Hero.positionY].hasHero = 1;
                pthread_mutex_unlock(&lockMAP);
            }

            break;
        case MOVE_DOWN:
            if (Hero.positionX < size - 1 && MAP[Hero.positionX + 1][Hero.positionY].isVoid != 1 && isOtherMonsterThere(Hero.positionX + 1, Hero.positionY) == 0)
            {
                pthread_mutex_lock(&lockMAP);
                MAP[Hero.positionX][Hero.positionY].hasHero = 0;
                Hero.positionX++;
                MAP[Hero.positionX][Hero.positionY].hasHero = 1;
                pthread_mutex_unlock(&lockMAP);
            }

            break;
        }

        if (MAP[Hero.positionX][Hero.positionY].isEnd == 1)
        {
            // printf("You found the exit!\n");
            hasWon = 1;
            // printw("WINNER WINNER CHICKEN DINNER\n");
            refresh();

            pthread_exit(0);
        }
        // {
        //     // gane
        // }
        // pthread_mutex_unlock(&lockMAP);
        if (lastUserAction != IDLE)
        {
            usleep(100000);
        }
        // usleep(100000);
    }
}

// int main()
// {
//     WINDOW *w;
//     w = initscr();
//     timeout(-1);
//     // WINDOW *w;
//     // keypad(w, true);
//     // w = initscr();
//     // nodelay(w, true);
//     // noecho();

//     srand(time(NULL));
//     // system("clear");
//     erase();

//     // printw("INICIO 1\n");
//     refresh();

//     if (pthread_mutex_init(&lockMAP, NULL) != 0)
//     {
//         // printf("\n mutex init has failed\n");
//         return 1;
//     }
//     if (pthread_mutex_init(&lockHero, NULL) != 0)
//     {
//         // printf("\n mutex init has failed\n");
//         return 1;
//     }
//     int difficulty;
//     size = 0;

//     while (size == 0)
//     {
//         // print please enter a difficulty level
//         // printw("Please enter a difficulty level (1, 2, or 3): \n");

//         scanw("%d", &difficulty);
//         system("clear");

//         switch (difficulty)
//         {
//         case 1:
//             size = 10;
//             // printw("You have selected easy mode\n");
//             MAP = createMatrix();

//             break;
//         case 2:
//             size = 20;
//             // printw("You have selected medium mode\n");
//             MAP = createMatrix();
//             break;
//         case 3:
//             size = 30;
//             // printw("You have selected hard mode\n");
//             MAP = createMatrix();
//             break;

//         default:
//             system("clear");
//             // printw("Please enter a valid difficulty level (1, 2, or 3): \n");
//             break;
//         }
//     }
//     // printw("MITAD 1\n");
//     refresh();
//     // HERE WE GET OUR HERO
//     heroHealth = 5;
//     heroAttack = 1;
//     hasWon = 0;
//     // create thread for the hero
//     pthread_create(&Hero.heroAction, NULL, &heroActions, NULL);

//     // instance monster array in dynamic memory of size "size"
//     MAP = fillMonsterArray(MAP);
//     for (int i = 0; i < size / 2; i++)
//     {
//         monsterArray[i].action = pthread_create(&monsterArray[i].action, NULL, &monstersActions, &monsterArray[i]);
//     }

//     // print Monster array

//     int tiempo = 0;

//     // printw("MITAD 2\n");
//     refresh();
//     // Render
//     // sleep(1);

//     while (1)
//     {

//         // print every monster in monsterArray with its restingValue

//         usleep(100000);
//         // system("clear");
//         erase();
//         pthread_mutex_lock(&lockMAP);
//         pthread_mutex_lock(&lockHero);

//         // // printw("Tiempo: %d\n", tiempo);
//         // printw("Vida: %d\n", heroHealth);
//         // printw("Ataque: %d\n", heroAttack);
//         // printw("\n");
//         // printw("\n");

//         // printw("\n");

//         // printw("y=0    1    2    3    4    5    6    7    8    9\n");

//         for (int i = 0; i < size; i++)
//         {
//             // printw("%d|", i);

//             for (int j = 0; j < size; j++)
//             {

//                 if (MAP[i][j].isVoid == 1)
//                     // printw(" ");
//                 else if (MAP[i][j].isBeginning == 1)
//                     // printw("B");
//                 else if (MAP[i][j].isEnd == 1)
//                     // printw("E");
//                 else if (MAP[i][j].hasTreasure == 1)
//                     // printw("T");
//                 else if (MAP[i][j].hadTreasure == 1)
//                     // printw("t");
//                 else if (MAP[i][j].hasTrap == 1)
//                     // printw("X");
//                 else if (MAP[i][j].hadTrap == 1)
//                     // printw("x");
//                 else
//                     // printw("O");
//                 if (MAP[i][j].hasMonster == 1)
//                 {
//                     // printw("-M");
//                 }

//                 if (MAP[i][j].hasHero == 1)
//                 {
//                     // printw("-H");
//                 }
//                 else
//                 {
//                     ;
//                 }
//                 // printw("|");
//             }
//             // printw("\n");
//         }
//         // printw("\n");

//         // printw("\n");

//         // printw("\n");

//         // printw("\n");

//         // printMonsterArray(size);
//         pthread_mutex_unlock(&lockMAP);
//         pthread_mutex_unlock(&lockHero);
//         // print_end();
//         // print_monsters();
//         if (heroHealth == 0 || hasWon)
//         {
//             // // printw("You died\n");
//             break;
//         }
//         refresh();
//         // tiempo++;
//     }

//     if (hasWon)
//     {
//         // printw("You won\n");
//     }
//     else if (heroHealth == 0)
//     {
//         // printw("You died\n");
//     }
//     refresh();
//     sleep(10);
//     endwin();
//     // free the memory
//     pthread_mutex_destroy(&lockMAP);
//     pthread_mutex_destroy(&lockHero);

//     for (int i = 0; i < size; i++)
//     {
//         free(MAP[i]);
//     }
//     free(MAP);
//     free(Coords);
//     free(monsterArray);

//     return 0;
// }

void printMap()
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (MAP[i][j].isVoid == 1)
            {
                printf("0");
            }
            else
            {
                printf("1");
            }
            printf(" | ");
        }
        printf("\n");
    }
}