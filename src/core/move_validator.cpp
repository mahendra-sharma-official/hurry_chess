#include "move_validator.hpp"

MoveValidator::MoveValidator(const Board& board)
    : m_board(board)
{
    m_moveStorage.SetAllMoves();
}

std::vector<Square> MoveValidator::GetLegalMoves(Square from) const
{
    return std::vector<Square>();
}

bool MoveValidator::IsLegalMove(Square from, Square to) const
{
    return false;
}

bool MoveValidator::IsInCheck(Color color) const
{
    return false;
}

bool MoveValidator::IsCheckmate(Color color) const
{
    return false;
}

bool MoveValidator::IsStalemate(Color color) const
{
    return false;
}

std::vector<Square> MoveValidator::GetRawMoves(Square from)
{
    PieceType selected_piece = m_board.GetPiece({ from.row, from.col }).type;
    std::vector<Square> raw_moves = m_moveStorage.GetRawMovesOf(selected_piece);

    // Change from relative to {0,0} to square from

    

    return raw_moves;
}

bool MoveValidator::DoesLeaveKingInCheck(Square from, Square to) const
{
    return false;
}

Square MoveValidator::FindKing(Color color) const
{
    return Square();
}
