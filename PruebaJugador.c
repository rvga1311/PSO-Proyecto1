#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>

#define PLAYER_W 50
#define PLAYER_H 50

struct player
{
    char *sprite;
    SDL_Surface *surface;
    SDL_Texture *texture;
    SDL_Rect hitbox;
};

struct player player1;
SDL_Renderer *rend;

void drawplayer()
{
    player1.surface = IMG_Load(player1.sprite);
    player1.texture = SDL_CreateTextureFromSurface(rend, player1.surface);
    SDL_FreeSurface(player1.surface);
    SDL_QueryTexture(player1.texture, NULL, NULL, &player1.hitbox.w, &player1.hitbox.h);
    player1.hitbox.w = PLAYER_W;
    player1.hitbox.h = PLAYER_H;
    SDL_RenderCopy(rend, player1.texture, NULL, &player1.hitbox);
}

int main(int argc, char *argv[])
{

    // returns zero on success else non-zero
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        printf("error initializing SDL: %s\n", SDL_GetError());
    }
    SDL_Window *win = SDL_CreateWindow("GAME", // creates a window
                                       SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       500, 500, 0);
    SDL_SetWindowResizable(win, SDL_FALSE);

    // triggers the program that controls
    // your graphics hardware and sets flags
    Uint32 render_flags = SDL_RENDERER_ACCELERATED;

    // creates a renderer to render our images
    rend = SDL_CreateRenderer(win, -1, render_flags);

    int close = 0;
    int speed = 300;
    player1.sprite = "./Images/SpriteF.png";
    player1.hitbox.x = 50;
    player1.hitbox.y = 50;

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

                    player1.sprite = "./Images/SpriteB.png";

                    player1.hitbox.y -= (speed / 30) * 10;
                    break;
                case SDL_SCANCODE_A:
                case SDL_SCANCODE_LEFT:

                    player1.sprite = "./Images/SpriteL.png";
                    player1.hitbox.x -= (speed / 30) * 10;
                    break;
                case SDL_SCANCODE_S:
                case SDL_SCANCODE_DOWN:

                    player1.sprite = "./Images/SpriteF.png";
                    player1.hitbox.y += (speed / 30) * 10;
                    break;
                case SDL_SCANCODE_D:
                case SDL_SCANCODE_RIGHT:

                    player1.sprite = "./Images/SpriteR.png";
                    player1.hitbox.x += (speed / 30) * 10;
                    break;
                default:
                    break;
                }
            }
            SDL_RenderPresent(rend);

            // calculates to 60 fps
            SDL_Delay(1000 / 60);
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
        drawplayer();

        // triggers the double buffers
        // for multiple rendering

        SDL_RenderPresent(rend);

        // calculates to 60 fps
        SDL_Delay(1000 / 60);
    }

    // destroy texture
    SDL_DestroyTexture(player1.texture);

    // destroy renderer
    SDL_DestroyRenderer(rend);

    // destroy window
    SDL_DestroyWindow(win);

    // close SDL
    SDL_Quit();

    return 0;
}
