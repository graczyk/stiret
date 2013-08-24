#include "piece.hpp"
#include "board.hpp"

Piece::Piece(const std::vector<std::vector<unsigned int>> &initialBoard,
             const std::vector<unsigned int> &rotationSequence,
             const int &initialColumn, const Color &color)
    : initialBoard(initialBoard)
    , currentBoard(initialBoard)
    , rotationSequence(rotationSequence)
    , currentRow(0)
    , currentColumn(initialColumn)
    , color(color)
    , rotationState(0)
{
    setBounds();
    while(moveUp())
        ;
}

void Piece::rotate()
{
    if(rotationSequence.size())
    {
        if(rotationState == rotationSequence.size() - 1)
            rotationState = -1;
        ++rotationState;

        rotateRight(rotationSequence[rotationState]);

        // Undo rotate if we shouldn't be rotating.
        if(currentColumn + boundingColumnLeft < 0 ||
           currentColumn + boundingColumnRight >= 10 ||
           currentRow + boundingRowUp < 0 ||
           currentRow + boundingRowDown >= 20)
            undoRotate();
    }
}

void Piece::undoRotate()
{
    if(rotationSequence.size())
    {
        if(rotationState == 0)
            rotationState = rotationSequence.size();
        --rotationState;
        rotateRight(rotationSequence[rotationState]);
    }
}

bool Piece::moveUp()
{
    if(currentRow - 1 + boundingRowUp >= 0)
    {
        --currentRow;
        return true;
    }

    return false;
}

bool Piece::moveDown()
{
    if(currentRow + 1 + boundingRowDown < 20)
    {
        ++currentRow;
        return true;
    }

    return false;
}

void Piece::moveLeft()
{
    if(currentColumn - 1 + boundingColumnLeft >= 0)
        --currentColumn;
}

void Piece::moveRight()
{
    if(currentColumn + 1 + boundingColumnRight < 10)
        ++currentColumn;
}

void Piece::rotateRight(const unsigned int &times)
{
    currentBoard = initialBoard;

    int rows = currentBoard.size();
    int cols = currentBoard[0].size();
    for(unsigned int k = 0; k < times; ++k)
    {
        std::vector<std::vector<unsigned int>> tempBoard(currentBoard);
        for(unsigned int i = 0; i < rows; ++i)
            for(unsigned int j = 0; j < cols; ++j)
                currentBoard[j][rows - 1 - i] = tempBoard[i][j];
    }

    setBounds();
}

void Piece::setBounds()
{
    boundingColumnLeft = 0;
    boundingColumnRight = currentBoard[0].size() - 1;
    boundingRowUp = 0;
    boundingRowDown = currentBoard.size() - 1;

    auto columnEmpty = [&](const int &column) -> bool {
        for(unsigned int i = 0; i < currentBoard.size(); ++i)
            if(currentBoard[i][column])
                return false;
        return true;
    };

    auto rowEmpty = [&](const int &row) -> bool {
        for(unsigned int j = 0; j < currentBoard[row].size(); ++j)
            if(currentBoard[row][j])
                return false;
        return true;
    };

    while(columnEmpty(boundingColumnLeft))
        boundingColumnLeft++;
    while(columnEmpty(boundingColumnRight))
        boundingColumnRight--;
    while(rowEmpty(boundingRowUp))
        boundingRowUp++;
    while(rowEmpty(boundingRowDown))
        boundingRowDown--;
}
