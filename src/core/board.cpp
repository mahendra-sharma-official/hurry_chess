#include "core/board.hpp"

/// CONSTRUCTOR
Board::Board()
{
    m_grid.fill({ PieceType::NONE, Color::NONE });
    PlaceStartingPieces();
}


/// RESETS THE BOARD TO INITIAL STATE
void Board::Reset()
{
    m_grid.fill({ PieceType::NONE, Color::NONE });
    PlaceStartingPieces();
}


/// GET THE PIECE AT THE SQUARE
Piece Board::GetPiece(Square sq) const
{
    return m_grid[sq.row][sq.col];
}


/// SET THE PIECE AT THE SQUARE
void Board::SetPiece(Square sq, Piece piece)
{
    m_grid[sq.row][sq.col] = piece;
}


/// MOVE THE PIECE FROM A SQUARE TO ANOTHER
void Board::MovePiece(Square from, Square to)
{
    m_grid[to.row][to.col] = m_grid[from.row][from.col];
    m_grid[from.row][from.col] = { PieceType::NONE, Color::NONE };
}


/// CHECKS IF SQUARE IS WITHIN THE BOARD
bool Board::IsInsideBounds(Square sq) const
{
    bool is_inside_row = (0 <= sq.row && sq.row < 8);
    bool is_inside_col = (0 <= sq.col && sq.col < 8);

    return is_inside_row && is_inside_col;
}

std::array<std::array<Piece, 8>, 8> Board::GetGrid() const
{
    return m_grid;
}


/// PLACE THE BOARD PIECES TO STARTING POSITION 
void Board::PlaceStartingPieces()
{
    // order to place the pieces in (left to right)
    PieceType piece_order[8] =
    {
        PieceType::ROOK, PieceType::KNIGHT, PieceType::BISHOP, PieceType::QUEEN,
        PieceType::KING, PieceType::BISHOP, PieceType::KNIGHT, PieceType::ROOK
    };

    // setting pieces 
    for(int i = 0; i < 8; i++)
    {
        // black
        SetPiece({ 0, i }, { piece_order[i], Color::BLACK });   // back rank
        SetPiece({ 1, i }, { PieceType::PAWN, Color::BLACK });  // pawns 

        // white
        SetPiece({ 7, i }, { piece_order[i], Color::WHITE });   // back rank
        SetPiece({ 6, i }, { PieceType::PAWN, Color::WHITE });  // pawns 
    }
}
