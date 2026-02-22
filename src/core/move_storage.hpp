#pragma once
#include <unordered_map>   

enum class PieceType;
struct Piece;

struct Square
{
    int row, col;
};

class MoveStorage
{
private:
    std::unordered_map<PieceType, std::vector<Square>> m_moves;

public:
    MoveStorage();
    std::vector<Square> GetRawMovesOf(PieceType pieceType);

private:

    // Raw moves from {0, 0} 
    void SetAllMoves();
    void SetPawnMoves();
    void SetKnightMoves();
    void SetBishopMoves();
    void SetRookMoves();
    void SetQueenMoves();
    void SetKingMoves();

};