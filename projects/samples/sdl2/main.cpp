#include <cmath>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

#include <SDL2/SDL.h>

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
//#include <SDL2/SDL_mixer.h>

struct Context
{
    SDL_Window *window;
    SDL_Renderer *renderer;

    int iteration;
    int width;
    int height;

    int offsetX;
    int offsetY;

    // all loaded textures
    std::map<std::string, SDL_Texture *> textures;

    // font and text data
    TTF_Font *font;
    SDL_Texture *textTexture;

    // mouse data
    int mouseX, mouseY, mouseDX, mouseDY;
    int mouseState;
    int canDrawLine;
    SDL_Rect rectLine;

    // width and height of the brush
    int brushSize = 32;

    // number of pixels between brush blots when forming a line
    int pixelsPerIteration = 5;

    // music
    //Mix_Music *music;

    // gui
    bool guiShowSampleWindow = true;

    // sdl
    bool done = false;
};

void loadImage(Context *ctx, const char *filename)
{
    if (ctx->textures.find(filename) == ctx->textures.end())
    {
        SDL_Surface *image = IMG_Load(filename);

        if (!image)
        {
            printf("Unable to load image: %s\n", IMG_GetError());
            return;
        }

        printf("New image loaded: %s\n", filename);

        ctx->textures[filename] = SDL_CreateTextureFromSurface(ctx->renderer, image);
        SDL_FreeSurface(image);
    }
}

void showImage(Context *ctx, const char *filename, SDL_Rect *rect)
{
    loadImage(ctx, filename);

    SDL_Texture *tex = ctx->textures[filename];
    SDL_RenderCopy(ctx->renderer, tex, nullptr, rect);
}

void loadBrush(Context *ctx)
{
    const auto filename = "assets/images/brush.png";
    loadImage(ctx, filename);
}

/*
Mix_Chunk *loadChunk(const char *filename)
{
    Mix_Chunk *chunk = Mix_LoadWAV(filename);

    if (chunk == nullptr)
    {
        printf("Unable to load chunk: %s", SDL_GetError());
        return nullptr;
    }

    printf("New chunk loaded: %s\n", filename);

    return chunk;
}
*/

/*
Mix_Music *loadMusic(const char *filename)
{
    Mix_Music *music = Mix_LoadMUS(filename);

    if (music == nullptr)
    {
        printf("Unable to load sfx: %s", SDL_GetError());
        return nullptr;
    }

    printf("New music loaded: %s\n", filename);

    return music;
}
*/

void writeText(Context *ctx, const char *text)
{
    SDL_Color fg = {0, 0, 0, 255};
    SDL_Surface *textSurface = TTF_RenderText_Blended(ctx->font, text, fg);
    ctx->textTexture = SDL_CreateTextureFromSurface(ctx->renderer, textSurface);
    SDL_FreeSurface(textSurface);
}

void drawLine(Context *ctx, float startx, float starty, float dx, float dy)
{
    float distance = sqrt(dx * dx + dy * dy);                // length of line segment (pythagoras)
    int iterations = distance / ctx->pixelsPerIteration + 1; // number of brush sprites to draw for the line
    float dx_prime = dx / iterations;                        // x-shift per iteration
    float dy_prime = dy / iterations;                        // y-shift per iteration
    SDL_Rect dstRect;                                        // rect to draw brush sprite into
    float x;
    float y;
    int i;

    dstRect.w = ctx->brushSize;
    dstRect.h = ctx->brushSize;

    // setup x and y for the location of the first sprite
    x = startx - ctx->brushSize / 2.0f;
    y = starty - ctx->brushSize / 2.0f;

    // draw a series of blots to form the line
    for (i = 0; i < iterations; i++)
    {
        dstRect.x = x;
        dstRect.y = y;

        // shift x and y for next sprite location
        x += dx_prime;
        y += dy_prime;

        // draw brush blot
        showImage(ctx, "assets/images/brush.png", &dstRect);
        //printf("Draw line at: %d, %d, %d, %d\n", dstRect.x, dstRect.y, dstRect.w, dstRect.h);
    }
}

void processEvents(Context *ctx)
{
    ctx->canDrawLine = false;

    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            ctx->done = 1;
        }
        switch (event.key.keysym.sym)
        {
        case SDLK_ESCAPE:
            ctx->done = true;
            break;
        case SDLK_UP:
            if (event.key.type == SDL_KEYDOWN)
                ctx->offsetY = ctx->offsetY - 1;
            break;
        case SDLK_DOWN:
            if (event.key.type == SDL_KEYDOWN)
                ctx->offsetY = ctx->offsetY + 1;
            break;
        case SDLK_LEFT:
            if (event.key.type == SDL_KEYDOWN)
                ctx->offsetX = ctx->offsetX - 1;
            break;
        case SDLK_RIGHT:
            if (event.key.type == SDL_KEYDOWN)
                ctx->offsetX = ctx->offsetX + 1;
            break;
        default:
            break;
        }

        switch (event.type)
        {
        case SDL_MOUSEMOTION:
            // printf("SDL mouse motion event\n");
            ctx->mouseState = SDL_GetMouseState(&ctx->mouseX, &ctx->mouseY);
            SDL_GetRelativeMouseState(&ctx->mouseDX, &ctx->mouseDY);

            if (ctx->mouseState & SDL_BUTTON_LMASK)
            {
                ctx->canDrawLine = true;
                ctx->rectLine = SDL_Rect{.x = ctx->mouseX - ctx->mouseDX, .y = ctx->mouseY - ctx->mouseDY, .w = ctx->mouseDX, .h = ctx->mouseDY};

                // load music
                /*
                if (ctx->music == nullptr)
                {
                    Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);
                    ctx->music = loadMusic("assets/sounds/bg.ogg");
                    Mix_PlayMusic(ctx->music, -1);
                }
                */
            }
            break;
        }
    }
}

void loop(void *arg)
{
    Context *ctx = static_cast<Context *>(arg);

    // events
    processEvents(ctx);

    // background
    SDL_SetRenderDrawColor(ctx->renderer, 230, 230, 230, 255);
    SDL_RenderClear(ctx->renderer);

    // moving rectangle
    SDL_Rect imageRect;
    imageRect.x = (ctx->iteration % ctx->width);
    imageRect.y = (ctx->iteration % ctx->height);
    imageRect.w = 50 + ctx->offsetX;
    imageRect.h = 50 + ctx->offsetY;

    showImage(ctx, "assets/images/logo.png", &imageRect);

    // draw line
    if (ctx->canDrawLine)
    {
        drawLine(ctx, ctx->rectLine.x, ctx->rectLine.y, ctx->rectLine.w, ctx->rectLine.h);
    }

    // text
    SDL_Rect textRect = {.x = (ctx->width / 2), .y = (ctx->height / 2), .w = 0, .h = 0};
    SDL_QueryTexture(ctx->textTexture, nullptr, nullptr, &textRect.w, &textRect.h);

    textRect.x = textRect.x - (textRect.w / 2);
    textRect.y = textRect.y - (textRect.h / 2);

    SDL_RenderCopy(ctx->renderer, ctx->textTexture, nullptr, &textRect);

    // present
    SDL_RenderPresent(ctx->renderer);

    ctx->iteration++;

#ifdef __EMSCRIPTEN__
    if (done)
    {
        emscripten_cancel_main_loop();
    }
#endif
}

// main function called first
int main(int argc, char *argv[])
{
    printf("App Loaded\n");

    double canvasWidth = 1024;
    double canvasHeight = 600;

    printf("Canvas size: %f, %f\n", canvasWidth, canvasHeight);

    // setup sdl
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    SDL_Window *window = SDL_CreateWindow(nullptr, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, canvasWidth, canvasHeight, (SDL_WINDOW_SHOWN));
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // create context
    Context ctx;
    ctx.window = window;
    ctx.renderer = renderer;
    ctx.iteration = 0;
    ctx.width = canvasWidth;
    ctx.height = canvasHeight;
    ctx.textures = {};

    // load font
    TTF_Init();
    ctx.font = TTF_OpenFont("assets/fonts/edosz.ttf", 30);
    writeText(&ctx, "ezored");

    // load brush
    loadBrush(&ctx);

// loop
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop_arg(loop, &ctx, 0, 1);
#else
    while (!ctx.done)
    {
        loop(&ctx);
    }
#endif

    // destroy all
    for (std::map<std::string, SDL_Texture *>::iterator it = ctx.textures.begin(); it != ctx.textures.end(); ++it)
    {
        SDL_DestroyTexture(it->second);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    //Mix_FreeMusic(ctx.music);
    //Mix_CloseAudio();

    SDL_Quit();

    return EXIT_SUCCESS;
}
