#pragma once
#include <utility> // std::pair

enum class PieceType
{
    NONE, PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING
};
enum class Color
{
    NONE, WHITE, BLACK
};

struct Piece
{
    PieceType type = PieceType::NONE;
    Color color = Color::NONE;

    bool IsEmpty() const;
};