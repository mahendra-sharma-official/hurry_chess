#include "core/move_validator.hpp"


/// CONSTRUCTOR
MoveValidator::MoveValidator(const Board& board)
    : m_board(board)
{
    m_moveStorage.SetAllMoves();
}



/// GET THE LEGAL MOVES FROM A CERTAIN SQUARE DEPENDING ON THE PIECE 
std::vector<Square> MoveValidator::GetLegalMoves(Square from) const
{
    bool can_skip = true;
    PieceType type = m_board.GetPiece(from).type;

    // these types legal moves stop after something comes in between
    if(type == PieceType::ROOK || type == PieceType::QUEEN || type == PieceType::BISHOP)
        can_skip = false;


    std::vector<Square> raw_moves = GetRawMoves(from);

    std::vector<Square> legal_moves;
    legal_moves.reserve(raw_moves.size());

    // handle pawn separately
    if(type == PieceType::PAWN)
    {
        int basic_dir = m_board.GetPiece(from).color == Color::BLACK ? 1 : 0;

        for(Square& rm : raw_moves)
        {
            Square actual_rm = { rm.row + 2 * basic_dir, rm.col };
            // if not inside bounds then don't check anything else
            if(!m_board.IsInsideBounds(actual_rm))
            {

            }
            // if the piece in front is not empty then it is invalid
            else if(m_board.GetPiece(actual_rm).type != PieceType::NONE
                    && actual_rm.col == from.col)
            {

            }
            // if the piece in front is empty then it is valid
            else if(m_board.GetPiece(actual_rm).type == PieceType::NONE)
            {
                if(actual_rm.col == from.col)
                    legal_moves.emplace_back(actual_rm);
            }
            // if the enemy piece on side are available then they are also valid
            else if(m_board.GetPiece(from).color != m_board.GetPiece(actual_rm).color)
            {
                legal_moves.emplace_back(actual_rm);

            }
        }
    }
    // rook, king just needs to check if it is ally or enemy
    else if(can_skip)
    {
        for(Square& rm : raw_moves)
        {
            if(IsLegalMove(from, rm))
                legal_moves.emplace_back(rm);
            else if(!m_board.IsInsideBounds(rm))
            {

            }
            else if(m_board.GetPiece(from).color != m_board.GetPiece(rm).color)
                legal_moves.emplace_back(rm);
        }
    }
    // queen, rook, bishop needs to check if something is in their way
    else
    {
        int degree_of_freedom = raw_moves.size() / 7;
        for(int i = 0; i < degree_of_freedom; i++)
            for(int j = 0; j < 7; j++)
            {
                Square rm = raw_moves[7 * i + j];
                if(IsLegalMove(from, rm))
                    legal_moves.emplace_back(rm);
                else if(!m_board.IsInsideBounds(rm))
                {
                }
                else
                {
                    // encountered a first opponent piece
                    if(m_board.GetPiece(from).color != m_board.GetPiece(rm).color)
                    {
                        legal_moves.emplace_back(rm);
                        break;
                    }
                    break;

                }
            }

    }

    return legal_moves;
}


/// CHECKS IF MAKING A MOVE FROM A SQUARE TO ANOTHER SQUARE IS LEGAL
/// RETURNS TRUE IF THE SQUARE IS EMPTY AND INSIDE BOARD
bool MoveValidator::IsLegalMove(Square from, Square to) const
{
    // square is within the board
    if(!m_board.IsInsideBounds(to) || !m_board.IsInsideBounds(from))
        return false;

    const Piece from_piece = m_board.GetPiece(from);
    const Piece to_piece = m_board.GetPiece(to);


    // empty piece is legal
    if(to_piece.type == PieceType::NONE)
        return true;

    return false;
}


/// CHECKS IF KING OF CERTAIN COLOR IS IN "CHECK" 
bool MoveValidator::IsInCheck(Color color) const
{
    return false;
}


/// CHECKS IF CHECKMATE HAPPEND OF A CERTAIN COLOR
bool MoveValidator::IsCheckmate(Color color) const
{
    return false;
}


/// CHECKS IF STALEMATE HAPPEND OF A CERTAIN COLOR
bool MoveValidator::IsStalemate(Color color) const
{
    return false;
}


/// GET RAW MOVES FROM A SQUARE
std::vector<Square> MoveValidator::GetRawMoves(Square from) const
{
    PieceType selected_piece = m_board.GetPiece(from).type;
    const std::vector<Square>& raw_moves = m_moveStorage.GetRawMovesOf(selected_piece);

    std::vector<Square> rel_raw_moves;
    rel_raw_moves.reserve(raw_moves.size());

    Square temp_raw_move;
    for(Square rm : raw_moves)
    {
        // change from relative to {0,0} to current "from" square
        temp_raw_move = { from.row + rm.row, from.col + rm.col };
        rel_raw_moves.emplace_back(temp_raw_move);
    }

    return rel_raw_moves;
}


/// CHECKS IF PERFORMING A MOVE LEAVES KING IN CHECK
bool MoveValidator::DoesLeaveKingInCheck(Square from, Square to) const
{
    return false;
}


/// GIVES THE SQUARE WHERE KING IS AT FOR A CERTAIN COLOR 
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
