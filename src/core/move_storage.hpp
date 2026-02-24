#pragma once
#include "square.hpp"
#include <unordered_map>   

enum class PieceType;
struct Piece;


class MoveStorage
{
private:
    std::unordered_map<PieceType, std::vector<Square>> m_moves;

public:
    MoveStorage();
    const std::vector<Square>& GetRawMovesOf(PieceType pieceType) const;
    void SetAllMoves();

private:

    // Raw moves from {0, 0} 
    void SetPawnMoves();
    void SetKnightMoves();
    void SetBishopMoves();
    void SetRookMoves();
    void SetQueenMoves();
    void SetKingMoves();

};