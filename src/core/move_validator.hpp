#pragma once
#include "board.hpp"
#include "move_storage.hpp"
#include <vector>

struct Move
{
    Square from, to, on; // played from to, captured on
    Piece played, captured;

};

class MoveValidator
{

private:
    Board& m_board;
    MoveStorage m_moveStorage;
    std::vector<Move>& m_moveHistory;
    Color& m_currTurn;

public:
    explicit MoveValidator(Board& board, Color& m_currTurn, std::vector<Move>& m_moveHistory);

    std::vector<Square> GetLegalMoves(Square from) const;
    bool IsEmpty(Square to) const;
    bool IsInCheck(Color color) const;
    bool IsCheckmate(Color color) const;
    bool IsStalemate(Color color) const;

private:

    std::vector<Square> GetRawMoves(Square from) const;
    bool IsBlockingCheck(Square from, Square to) const;
    bool DoesLeaveKingInCheck(Square from, Square to) const;
    bool DoesKingGetInCheck(Square to) const;
    Square FindKing(Color color) const;
    std::vector<Square> GetLegalMovesOf(Piece piece, Square from) const;
    std::vector<Square> GetLegalPawnMoves(Piece piece, Square from) const;
    std::vector<Square> GetLegalKnightMoves(Piece piece, Square from) const;
    std::vector<Square> GetLegalBishopMoves(Piece piece, Square from) const;
    std::vector<Square> GetLegalRookMoves(Piece piece, Square from) const;
    std::vector<Square> GetLegalQueenMoves(Piece piece, Square from) const;
    std::vector<Square> GetLegalKingMoves(Piece piece, Square from) const;
};