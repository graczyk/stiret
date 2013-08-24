#ifndef PIECE__H
#define PIECE__H

#include <vector>

#include "color.hpp"

class Board;

class Piece
{
public:
    Piece() {}
    Piece(const std::vector<std::vector<unsigned int>> &initialBoard,
          const std::vector<unsigned int> &rotationSequence,
          const int &initialColumn, const Color &color);
    bool moveUp();
    bool moveDown();
    void moveLeft();
    void moveRight();
    void rotate();
    void undoRotate();

    const int& getCurrentRow() const { return currentRow; }
    const int& getCurrentColumn() const { return currentColumn; }
    const int& getBoundingColumnLeft() const { return boundingColumnLeft; }
    const int& getBoundingColumnRight() const { return boundingColumnRight; }
    const int& getBoundingRowUp() const { return boundingRowUp; }
    const int& getBoundingRowDown() const { return boundingRowDown; }
    const std::vector<std::vector<unsigned int>>& getBoard() const
    { return currentBoard; }
    const Color& getColor() const { return color; }

private:
    void rotateRight(const unsigned int &times);
    void setBounds();

    std::vector<std::vector<unsigned int>> initialBoard;
    std::vector<std::vector<unsigned int>> currentBoard;
    std::vector<unsigned int> rotationSequence;
    int currentRow;
    int currentColumn;
    int boundingColumnLeft;
    int boundingColumnRight;
    int boundingRowUp;
    int boundingRowDown;
    int rotationState;
    Color color;
};

#endif
