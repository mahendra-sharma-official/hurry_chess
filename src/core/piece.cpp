#include "core/piece.hpp"

/// CHECKS IF PIECE HAS A TYPE OR NOT
bool Piece::IsEmpty() const
{
    return type == PieceType::NONE ? true : false;
}
