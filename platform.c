#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 640

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>

#include "chip8.c"

typedef struct
{
    SDL_Renderer *renderer;
    SDL_Window *window;
    SDL_Texture *texture;
    uint32_t video_buffer[2048];
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

    app->window = SDL_CreateWindow("Emulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, windowFlags);
    if (!app->window)
    {
        printf("Failed to open %d x %d window: %s\n", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_GetError());
        exit(1);
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

    app->renderer = SDL_CreateRenderer(app->window, -1, rendererFlags);

    app->texture = SDL_CreateTexture(app->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, 64, 32);

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
    chip8 c;

    char *file_name = "Pong.ch8";

    App app;

    initSDL(&app);

    init_start(&c);

    read_file(file_name, &c);

    while (1)
    {

        for (int i = 0; i < 10; i++){
            chip8_cycle(&c);
        }

        if (c.delay_timer > 0)
            c.delay_timer --;
        
        if (c.sound_timer > 0)
            c.sound_timer --;
        
        
        for (int i = 0; i < 2048; i++){
            if (c.gfx[i] == 1)
                app.video_buffer[i] = 0xFFFFFFFF;
            else 
                app.video_buffer[i] = 0x000000FF;
            
        }

        SDL_UpdateTexture(app.texture, NULL, app.video_buffer, 256);

        SDL_RenderClear(app.renderer);

        SDL_RenderCopy(app.renderer, app.texture, NULL, NULL);

        SDL_RenderPresent(app.renderer);

        doInput();

        SDL_Delay(16);
    }
}