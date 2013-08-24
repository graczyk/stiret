#include "board.hpp"

#include <cstdlib>
#include <ctime>
#include <algorithm>

const unsigned int Board::NUM_ROWS = 20;
const unsigned int Board::NUM_COLS = 10;

const unsigned int Board::BLOCK_SIZE = 20;
const unsigned int Board::SPACER_SIZE = 2;

const std::vector<Piece> Board::PIECES =
{
    {
        {{0, 0, 0, 0},
         {0, 0, 0, 0},
         {1, 1, 1, 1},
         {0, 0, 0, 0}},
        {0, 3},
        3,
        {0x00, 0xf0, 0xf0}
    },
    {
        {{1, 1},
         {1, 1}},
        {},
        3,
        {0xf0, 0xf0, 0x00}
    },
    {
        {{0, 0, 0},
         {1, 1, 1},
         {0, 0, 1}},
        {0, 1, 2, 3},
        3,
        {0x00, 0x00, 0xf0}
    },
    {
        {{0, 0, 0},
         {1, 1, 1},
         {1, 0, 0}},
        {0, 1, 2, 3},
        3,
        {0xf0, 0xa0, 0x00}
    },
    {
        {{0, 0, 0},
         {0, 1, 1},
         {1, 1, 0}},
        {0, 3},
        3,
        {0x00, 0xf0, 0x00}
    },
    {
        {{0, 0, 0},
         {1, 1, 1},
         {0, 1, 0}},
        {0, 1, 2, 3},
        3,
        {0xa0, 0x00, 0xf0}
    },
    {
        {{0, 0, 0},
         {1, 1, 0},
         {0, 1, 1}},
        {0, 3},
        3,
        {0xf0, 0x00, 0x00}
    }
};

Board::Board()
{
    initialize();
}

void Board::initialize()
{
    board.clear();
    board.insert(board.begin(), NUM_ROWS, std::vector<Uint32>(NUM_COLS, 0));

    pieceBagState = -1;
    for(unsigned int i = 0; i < PIECES.size(); ++i)
        pieceBag.push_back(i);
    shufflePieceBag();

    gameOver = false;
    resetCurrentPiece();
}

void Board::shufflePieceBag()
{
    static bool seeded = false;
    if(!seeded)
    {
        srand(time(NULL));
        seeded = true;
    }
    std::random_shuffle(pieceBag.begin(), pieceBag.end());
}

void Board::clearBoard()
{
    for(auto &i : board)
        for(auto &j : i)
            j = 0;
}

void Board::renderCurrentPiece(SDL_Surface *surface)
{
    renderPiece(surface, currentPiece);
}

void Board::renderPiece(SDL_Surface *surface, const Piece &piece,
                        unsigned int offsetX, unsigned int offsetY)
{
    int currentRow = piece.getCurrentRow();
    int currentColumn = piece.getCurrentColumn();
    int boundingColumnLeft = piece.getBoundingColumnLeft();
    int boundingColumnRight = piece.getBoundingColumnRight();
    int boundingRowUp = piece.getBoundingRowUp();
    int boundingRowDown = piece.getBoundingRowDown();

    SDL_Rect rect = {.x = 0, .y = 0, .w = BLOCK_SIZE, .h = BLOCK_SIZE};
    for(int i = 0; i < boundingRowDown - boundingRowUp + 1; ++i)
        for(int j = 0; j < boundingColumnRight - boundingColumnLeft + 1; ++j)
            if(piece.getBoard()[i + boundingRowUp]
               [j + boundingColumnLeft])
            {
                rect.x = (currentColumn + boundingColumnLeft + j) *
                    (BLOCK_SIZE + SPACER_SIZE) + offsetX;
                rect.y = (currentRow + boundingRowUp + i) *
                    (BLOCK_SIZE + SPACER_SIZE) + offsetY;
                SDL_FillRect(surface, &rect,
                             SDL_MapRGB(surface->format,
                                        piece.getColor().red,
                                        piece.getColor().green,
                                        piece.getColor().blue));
            }
}

void Board::render(SDL_Surface *surface)
{
    SDL_Rect rect = {.x = 0, .y = 0, .w = BLOCK_SIZE, .h = BLOCK_SIZE};
    for(unsigned int i = 0; i < NUM_ROWS; ++i)
        for(unsigned int j = 0; j < NUM_COLS; ++j)
        {
            rect.x = j * (BLOCK_SIZE + SPACER_SIZE);
            rect.y = i * (BLOCK_SIZE + SPACER_SIZE);
            Uint8 red = (board[i][j] & 0x00ff0000u) >> 16;
            Uint8 green = (board[i][j] & 0x0000ff00u) >> 8;
            Uint8 blue = (board[i][j] & 0x000000ffu);
            if(!red && !green && !blue)
                red = green = blue = 0x55;
            SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format,
                                                    red, green, blue));
        }

    int nextState = pieceBagState + 1;
    if(pieceBagState == pieceBag.size())
        nextState = 0;
    renderPiece(surface, PIECES[pieceBag[nextState]], 235, 200);

    if(!isGameOver())
        renderCurrentPiece(surface);
}

bool Board::handleEvent(const SDL_Event &event)
{
    if(isGameOver())
        return false;

    if(event.type == SDL_KEYDOWN)
    {
        switch(event.key.keysym.sym)
        {
        case SDLK_LEFT:
        case SDLK_a:
            moveCurrentPieceLeft();
            return true;
        case SDLK_RIGHT:
        case SDLK_d:
            moveCurrentPieceRight();
            return true;
        case SDLK_UP:
        case SDLK_w:
            rotateCurrentPiece();
            return true;
        case SDLK_DOWN:
        case SDLK_s:
            dropCurrentPiece();
            return true;
        default:
            return false;
        }
    }

    return false;
}

void Board::resetCurrentPiece()
{
    ++pieceBagState;
    currentPiece = PIECES[pieceBag[pieceBagState]];
    if(pieceBagState == PIECES.size() - 1)
    {
        pieceBagState = -1;
        shufflePieceBag();
    }

    if(isCurrentPieceColliding())
        gameOver = true;
}

void Board::removeFilledRows()
{
    for(int i = board.size() - 1; i > 0; --i)
    {
        if(std::find(board[i].begin(), board[i].end(), 0) == board[i].end())
        {
            for(int k = i; k > 0; --k)
                std::copy(board[k - 1].begin(), board[k - 1].end(),
                          board[k].begin());
            ++i;
        }
    }
}

void Board::placePiece()
{
    int currentRow = currentPiece.getCurrentRow();
    int currentColumn = currentPiece.getCurrentColumn();
    int boundingColumnLeft = currentPiece.getBoundingColumnLeft();
    int boundingColumnRight = currentPiece.getBoundingColumnRight();
    int boundingRowDown = currentPiece.getBoundingRowDown();
    int boundingRowUp = currentPiece.getBoundingRowUp();

    for(int i = 0; i < boundingRowDown - boundingRowUp + 1; ++i)
        for(int j = 0; j < boundingColumnRight - boundingColumnLeft + 1; ++j)
        {
            unsigned int pieceBit =
                currentPiece.getBoard()[i + boundingRowUp]
                [j + boundingColumnLeft];
            if(pieceBit)
                board[currentRow + boundingRowUp + i]
                    [currentColumn + boundingColumnLeft + j] =
                    currentPiece.getColor().red << 16 |
                    currentPiece.getColor().green << 8 |
                    currentPiece.getColor().blue;
        }

    removeFilledRows();
}

bool Board::isCurrentPieceColliding()
{
    int currentRow = currentPiece.getCurrentRow();
    int currentColumn = currentPiece.getCurrentColumn();
    int boundingColumnLeft = currentPiece.getBoundingColumnLeft();
    int boundingColumnRight = currentPiece.getBoundingColumnRight();
    int boundingRowDown = currentPiece.getBoundingRowDown();
    int boundingRowUp = currentPiece.getBoundingRowUp();

    for(int i = 0; i < boundingRowDown - boundingRowUp + 1; ++i)
        for(int j = 0; j < boundingColumnRight - boundingColumnLeft + 1; ++j)
            if(board[currentRow + i + boundingRowUp]
               [currentColumn + j + boundingColumnLeft]
               && currentPiece.getBoard()[i + boundingRowUp]
               [j + boundingColumnLeft])
                return true;

    return false;
}

bool Board::moveCurrentPieceDown()
{
    bool couldMove = currentPiece.moveDown();
    bool isColliding = isCurrentPieceColliding();

    if(isColliding)
        currentPiece.moveUp();

    if(!couldMove || isColliding)
    {
        placePiece();
        resetCurrentPiece();
        return false;
    }

    return true;
}

void Board::dropCurrentPiece()
{
    while(moveCurrentPieceDown())
        ;
}

void Board::moveCurrentPieceLeft()
{
    currentPiece.moveLeft();

    if(isCurrentPieceColliding())
        currentPiece.moveRight();
}

void Board::moveCurrentPieceRight()
{
    currentPiece.moveRight();

    if(isCurrentPieceColliding())
        currentPiece.moveLeft();
}

void Board::rotateCurrentPiece()
{
    currentPiece.rotate();

    if(isCurrentPieceColliding())
        currentPiece.undoRotate();
}
