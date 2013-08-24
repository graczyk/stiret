#include "game.hpp"
#include "util.hpp"

#include <cstdlib>
#include <ctime>
#include <algorithm>

const int Game::SCREEN_WIDTH = 218 + 200;
const int Game::SCREEN_HEIGHT = 438;
const int Game::SCREEN_BPP = 32;

const Uint32 Game::TIME_MAX_WAIT = 25;

const char *Game::WINDOW_TITLE = "Stiret";

Game::Game()
    : screen(NULL)
    , running(true)
{
    if(SDL_Init(SDL_INIT_VIDEO))
        util::die("Couldn't initialize SDL: %s", SDL_GetError());

    screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP,
                              SDL_SWSURFACE);
    if(!screen)
        util::die("Couldn't set SDL video mode: %s", SDL_GetError());

    SDL_WM_SetCaption(WINDOW_TITLE, WINDOW_TITLE);
    SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY,
                        SDL_DEFAULT_REPEAT_INTERVAL);
}

Game::~Game()
{
    SDL_Quit();
}

void Game::run()
{
    SDL_Event event;
    unsigned int frameCount = 0;
    while(running)
    {
        Uint32 beginTime = SDL_GetTicks();

        render();

        while(SDL_PollEvent(&event))
            handleEvent(event);

        Uint32 endTime = SDL_GetTicks();
        Uint32 elapsedTime = endTime - beginTime;
        SDL_Delay(TIME_MAX_WAIT - std::min(TIME_MAX_WAIT, elapsedTime));

        ++frameCount;
        if(frameCount == 500 / TIME_MAX_WAIT)
        {
            if(!board.isGameOver())
                board.moveCurrentPieceDown();
            frameCount = 0;
        }
    }
}

void Game::handleEvent(const SDL_Event &event)
{
    if(board.handleEvent(event))
        return;

    switch(event.type)
    {
    case SDL_KEYDOWN:
        switch(event.key.keysym.sym)
        {
        case SDLK_ESCAPE:
            running = false;
            break;
        default:
            break;
        }
        break;
    case SDL_QUIT:
        running = false;
        break;
    default:
        break;
    }
}

void Game::render()
{
    SDL_FillRect(screen, NULL,
                 SDL_MapRGB(screen->format, 0x00, 0x00, 0x00));
    SDL_Rect rect = {.x = 0, .y = 0, .w = 220, .h = SCREEN_HEIGHT};
    SDL_FillRect(screen, &rect,
                     SDL_MapRGB(screen->format, 0x55, 0x55, 0x55));
    board.render(screen);
        SDL_Flip(screen);
}
