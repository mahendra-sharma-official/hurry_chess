#include "board.hpp"

Board::Board()
{
    PlaceStartingPieces();
}

void Board::Reset()
{
    PlaceStartingPieces();
}

Piece Board::GetPiece(Square sq) const
{
    return m_grid[sq.row][sq.col];
}

void Board::SetPiece(Square sq, Piece piece)
{
    m_grid[sq.row][sq.col] = piece;
}

void Board::MovePiece(Square from, Square to)
{
    m_grid[to.row][to.col] = m_grid[from.row][from.col];
    m_grid[from.row][from.col] = { PieceType::NONE, Color::NONE };
}

bool Board::IsInsideBounds(Square sq) const
{
    bool is_inside_row = (0 <= sq.row && sq.row >= 8);
    bool is_inside_col = (0 <= sq.col && sq.col >= 8);

    return is_inside_row && is_inside_col;
}

void Board::PlaceStartingPieces()
{
    // Order to place the pieces in (left to right)
    PieceType piece_order[8] =
    {
        PieceType::ROOK, PieceType::KNIGHT, PieceType::BISHOP, PieceType::QUEEN,
        PieceType::KING, PieceType::BISHOP, PieceType::KNIGHT, PieceType::ROOK
    };

    // Setting pieces 
    for(int i = 0; i < 8; i++)
    {
        // Black
        SetPiece({ 0, i }, { piece_order[i], Color::BLACK });   // back rank
        SetPiece({ 1, i }, { PieceType::PAWN, Color::BLACK });  // pawns 

        // White
        SetPiece({ 7, i }, { piece_order[i], Color::WHITE });   // back rank
        SetPiece({ 6, i }, { PieceType::PAWN, Color::WHITE });  // pawns 
    }
}
