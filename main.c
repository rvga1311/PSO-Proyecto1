#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>

#define ROOM_SIZE 33

SDL_Renderer* rend;
SDL_Surface* surface;

struct room
{
    char *sprite;
    SDL_Surface *surface;
    SDL_Texture *texture;
    SDL_Rect location;
    int i;
    int j;
};

void renderMap(int size, struct room rooms[size]) {
    for (int i = 0; i < size; i++) {
        SDL_RenderCopy(rend, rooms[i].texture, NULL, &rooms[i].location);
    }
}

void drawMap(int size, struct room rooms[size]) {

    // Make a identity matrix of size 30 x 30
    int prueba[30][30];
    for (int i = 0; i < 30; i++) {
        for (int j = 0; j < 30; j++) {
            if (i == j) {
                prueba[i][j] = 1;
            } else {
                prueba[i][j] = 0;
            }
        }
    }

    int currentX = 0;
    int currentY = 0;
    int counter = 0;

    for (int i = 0; i < size; i++) {
        for (int y = 0; y < size; y++) {
            if (prueba[i][y] == 1) {
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
                counter++;
            }
            currentX += ROOM_SIZE;
        }
        currentX = 0;
        currentY += ROOM_SIZE;
    }
}

void destroyMap(int size, struct room rooms[size]) {
    for (int i = 0; i < size; i++) {
        SDL_DestroyTexture(rooms[i].texture);
    }
}
 
int main(int argc, char *argv[])
{
    
    int size = 30;
    struct room rooms[size];

    // returns zero on success else non-zero
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("error initializing SDL: %s\n", SDL_GetError());
    }
    SDL_Window* win = SDL_CreateWindow("GAME", // creates a window
                                       SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       size * ROOM_SIZE, size * ROOM_SIZE, 0);
    SDL_SetWindowResizable(win, SDL_FALSE);
 
    // triggers the program that controls
    // your graphics hardware and sets flags
    Uint32 render_flags = SDL_RENDERER_ACCELERATED;
 
    // creates a renderer to render our images
    rend = SDL_CreateRenderer(win, -1, render_flags);

    drawMap(size, rooms);
 
    // controls animation loop
    int close = 0;
 
    // speed of box
    int speed = 300;
 
    // animation loop
    while (!close) {
        SDL_Event event;
 
        // Events management
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
 
            case SDL_QUIT:
                // handling of close button
                close = 1;
                break;
 
        //     case SDL_KEYDOWN:
        //         // keyboard API for key pressed
        //         switch (event.key.keysym.scancode) {
        //         case SDL_SCANCODE_W:
        //         case SDL_SCANCODE_UP:
        //             surface = IMG_Load("1.png");
        //             tex = SDL_CreateTextureFromSurface(rend, surface);
        //             SDL_QueryTexture(tex, NULL, NULL, &dest.w, &dest.h);
        //             dest.w = 100;
        //             dest.h = 100;
        //             dest.y -= (speed / 30)*10;                    
        //             break;
        //         case SDL_SCANCODE_A:
        //         case SDL_SCANCODE_LEFT:
        //             surface = IMG_Load("2.png");
        //             tex = SDL_CreateTextureFromSurface(rend, surface);
        //             SDL_QueryTexture(tex, NULL, NULL, &dest.w, &dest.h);
        //             dest.w = 100;
        //             dest.h = 100;
        //             dest.x -= (speed / 30)*10;
        //             break;
        //         case SDL_SCANCODE_S:
        //         case SDL_SCANCODE_DOWN:
        //             surface = IMG_Load("3.png");
        //             tex = SDL_CreateTextureFromSurface(rend, surface);
        //             SDL_QueryTexture(tex, NULL, NULL, &dest.w, &dest.h);
        //             dest.w = 100;
        //             dest.h = 100;
        //             dest.y += (speed / 30)*10;
        //             break;
        //         case SDL_SCANCODE_D:
        //         case SDL_SCANCODE_RIGHT:
        //             surface = IMG_Load("2.png");
        //             tex = SDL_CreateTextureFromSurface(rend, surface);
        //             SDL_QueryTexture(tex, NULL, NULL, &dest.w, &dest.h);
        //             dest.w = 100;
        //             dest.h = 100;
        //             dest.x += (speed / 30)*10;
        //             break;
        //         default:
        //             break;
        //         }
            }
        }
 
        // right boundary
        // if (dest.x + dest.w > 1000)
        //     dest.x = 1000 - dest.w;
 
        // // left boundary
        // if (dest.x < 0)
        //     dest.x = 0;
 
        // // bottom boundary
        // if (dest.y + dest.h > 1000)
        //     dest.y = 1000 - dest.h;
 
        // // upper boundary
        // if (dest.y < 0)
        //     dest.y = 0;
 
        // clears the screen
        SDL_RenderClear(rend);
        // SDL_RenderCopy(rend, tex, NULL, &dest);
        renderMap(size, rooms);
 
        // triggers the double buffers
        // for multiple rendering
        SDL_RenderPresent(rend);
 
        // calculates to 60 fps
        SDL_Delay(1000 / 30);
    }
 
    // destroy texture
    destroyMap(size, rooms);
 
    // destroy renderer
    SDL_DestroyRenderer(rend);
 
    // destroy window
    SDL_DestroyWindow(win);
     
    // close SDL
    SDL_Quit();
 
    return 0;
}
