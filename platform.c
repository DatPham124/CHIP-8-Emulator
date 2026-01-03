#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 640

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct
{
    SDL_Renderer *renderer;
    SDL_Window *window;
    SDL_Texture *texture;
} App;

void initSDL(App *app)
{
    int rendererFlags, windowFlags;

    rendererFlags = SDL_RENDERER_ACCELERATED;

    windowFlags = 0;

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Couldn't initialize SDL: %s \n", SDL_GetError());
        exit(0);
    }

    app->window = SDL_CreateWindow("Shooter 01", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, windowFlags);
    if (!app->window)
    {
        printf("Failed to open %d x %d window: %s\n", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_GetError());
        exit(1);
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

    app->renderer = SDL_CreateRenderer(app->window, -1, rendererFlags);

    if (!app->renderer)
    {
        printf("Failed to create renderer: %s\n", SDL_GetError());
        exit(1);
    }
}

void doInput(void)
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            exit(0);
            break;
        default:
            break;
        }
    }
}

int main()
{
    App app;

    initSDL(&app);

    while (1)
    {
        doInput();
    }
}