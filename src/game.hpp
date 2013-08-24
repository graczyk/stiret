#ifndef GAME__HPP
#define GAME__HPP

#include <SDL/SDL.h>
#include "board.hpp"
#include "piece.hpp"

class Game
{
public:
    Game();
    ~Game();

    void run();
private:
    void handleEvent(const SDL_Event &event);
    void render();

    Piece currentPiece;
    Board board;

    SDL_Surface *screen;
    bool running;

    static const int SCREEN_WIDTH;
    static const int SCREEN_HEIGHT;
    static const int SCREEN_BPP;

    static const Uint32 TIME_MAX_WAIT;

    static const char *WINDOW_TITLE;
};

#endif
