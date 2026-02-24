#pragma once
#include "piece.hpp"
#include "square.hpp"
#include <array>


class Board
{
public:
    Board();

    void Reset();
    Piece GetPiece(Square sq) const;
    void SetPiece(Square sq, Piece piece);
    void MovePiece(Square from, Square to);
    bool IsInsideBounds(Square sq) const;

private:
    std::array<std::array<Piece, 8>, 8> m_grid;

    void PlaceStartingPieces();
};