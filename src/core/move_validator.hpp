#pragma once
#include "board.hpp"
#include "move_storage.hpp"
#include <vector>

class MoveValidator
{

private:
    const Board& m_board;
    MoveStorage m_moveStorage;


public:
    explicit MoveValidator(const Board& board);

    std::vector<Square> GetLegalMoves(Square from) const;
    bool IsLegalMove(Square from, Square to) const;
    bool IsInCheck(Color color) const;
    bool IsCheckmate(Color color) const;
    bool IsStalemate(Color color) const;

private:

    std::vector<Square> GetRawMoves(Square from) const;
    bool DoesLeaveKingInCheck(Square from, Square to) const;
    Square FindKing(Color color) const;
};