#include "piece.hpp"


bool Piece::IsEmpty() const
{
    return type == PieceType::NONE ? true : false;
}
