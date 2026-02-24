#pragma once

enum class PieceType
{
    NONE, ROOK, KNIGHT, BISHOP, QUEEN, KING, PAWN
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