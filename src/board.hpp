#ifndef BOARD__HPP
#define BOARD__HPP

#include <SDL/SDL.h>
#include <vector>

#include "piece.hpp"
#include "color.hpp"

class Board
{
public:
    Board();
    void render(SDL_Surface *surface);
    bool handleEvent(const SDL_Event &event);

    void dropCurrentPiece();
    bool moveCurrentPieceDown();
    void moveCurrentPieceLeft();
    void moveCurrentPieceRight();
    void rotateCurrentPiece();

    bool isGameOver() { return gameOver; }

private:
    void initialize();
    void renderCurrentPiece(SDL_Surface *surface);
    void renderPiece(SDL_Surface *surface, const Piece &piece,
                     unsigned int offsetX = 0, unsigned int offsetY = 0);
    void shufflePieceBag();
    void resetCurrentPiece();
    void clearBoard();
    void removeFilledRows();
    void placePiece();
    bool isCurrentPieceColliding();

    std::vector<std::vector<unsigned int>> board;
    std::vector<unsigned int> pieceBag;
    int pieceBagState;
    Piece currentPiece;
    bool gameOver;

    static const unsigned int NUM_ROWS;
    static const unsigned int NUM_COLS;

    static const unsigned int BLOCK_SIZE;
    static const unsigned int SPACER_SIZE;

    static const std::vector<Piece> PIECES;
};

#endif
