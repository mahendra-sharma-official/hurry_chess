#include "core/move_validator.hpp"


//
// CONSTRUCTOR
MoveValidator::MoveValidator(const Board& board)
    : m_board(board)
{
    m_moveStorage.SetAllMoves();
}



// GET THE LEGAL MOVES FROM A CERTAIN SQUARE DEPENDING ON THE PIECE 
std::vector<Square> MoveValidator::GetLegalMoves(Square from) const
{
    std::vector<Square> raw_moves = GetRawMoves(from);

    std::vector<Square> legal_moves;
    legal_moves.reserve(raw_moves.size());

    for(Square& rm : raw_moves)
    {
        if(IsLegalMove(from, rm))
            legal_moves.emplace_back(rm);
    }

    return legal_moves;
}



// CHECKS IF MAKING A MOVE FROM A SQUARE TO ANOTHER SQUARE IS LEGAL
bool MoveValidator::IsLegalMove(Square from, Square to) const
{
    const Piece from_piece = m_board.GetPiece(from);
    const Piece to_piece = m_board.GetPiece(to);

    // friendly piece
    if(from_piece.color == to_piece.color)
        return false;

    // moves to king
    if(to_piece.type == PieceType::KING)
        return false;

    // if all above cases fail then it is a legal move
    return true;
}



// CHECKS IF KING OF CERTAIN COLOR IS IN "CHECK" 
bool MoveValidator::IsInCheck(Color color) const
{
    return false;
}



// CHECKS IF CHECKMATE HAPPEND OF A CERTAIN COLOR
bool MoveValidator::IsCheckmate(Color color) const
{
    return false;
}



// CHECKS IF STALEMATE HAPPEND OF A CERTAIN COLOR
bool MoveValidator::IsStalemate(Color color) const
{
    return false;
}



// GET VALID RAW (POSSIBLE) MOVES FROM A SQUARE
std::vector<Square> MoveValidator::GetRawMoves(Square from) const
{
    PieceType selected_piece = m_board.GetPiece({ from.row, from.col }).type;
    std::vector<Square> raw_moves = m_moveStorage.GetRawMovesOf(selected_piece);

    std::vector<Square> valid_raw_moves;
    valid_raw_moves.reserve(raw_moves.size() / 2);

    Square temp_raw_move;
    for(Square& rm : raw_moves)
    {
        // change from relative to {0,0} to current "from" square
        temp_raw_move = { from.row + rm.row, from.col + rm.col };

        if(m_board.IsInsideBounds(temp_raw_move))
            valid_raw_moves.emplace_back(temp_raw_move);
    }

    return valid_raw_moves;
}



// CHECKS IF PERFORMING A MOVE LEAVES KING IN CHECK
bool MoveValidator::DoesLeaveKingInCheck(Square from, Square to) const
{
    return false;
}



// GIVES THE SQUARE WHERE KING IS AT FOR A CERTAIN COLOR 
Square MoveValidator::FindKing(Color color) const
{
    for(int i = 0; i < 8; i++)
        for(int j = 0; j < 8; j++)
        {
            if(m_board.GetPiece({ i, j }).type == PieceType::KING
               && m_board.GetPiece({ i, j }).color == color)
                return { i,j };
        }
}
