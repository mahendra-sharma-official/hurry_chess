#include "core/move_validator.hpp"


/// CONSTRUCTOR
MoveValidator::MoveValidator(Board& board, Color& currTurn, std::vector<Move>& moveHistory)
    : m_board(board), m_currTurn(currTurn), m_moveHistory(moveHistory)
{
    m_moveStorage.SetAllMoves();
}


/// GET THE LEGAL MOVES FROM A CERTAIN SQUARE DEPENDING ON THE PIECE 
std::vector<Square> MoveValidator::GetLegalMoves(Square from) const
{
    Piece piece = m_board.GetPiece(from);

    // gives only the playable moves (fully valid checking everything)
    return GetLegalMovesOf(piece, from);
}


/// CHECKS IF MAKING A MOVE FROM A SQUARE TO ANOTHER SQUARE IS LEGAL
/// RETURNS TRUE IF THE SQUARE IS EMPTY AND INSIDE BOARD
bool MoveValidator::IsEmpty(Square to) const
{
    // square is within the board
    if(!m_board.IsInsideBounds(to))
        return false;

    const Piece to_piece = m_board.GetPiece(to);
    if(to_piece.type != PieceType::NONE)
        return false;

    return true;
}


/// CHECKS IF KING OF CERTAIN COLOR IS IN "CHECK"
bool MoveValidator::IsInCheck(Color color) const
{
    Square king_square = FindKing(color);
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            Piece p = m_board.GetPiece({ i, j });
            if(p.color == color || p.color == Color::NONE)
                continue;

            std::vector<Square> rm = GetRawMoves({ i,j });

            for(Square& r : rm)
                if(king_square.row == r.row && king_square.col == r.col)
                    return true;

        }
    }

    return false;
}


/// CHECKS IF CHECKMATE HAPPEND OF A CERTAIN COLOR
bool MoveValidator::IsCheckmate(Color color) const
{
    Square king_square = FindKing(color);

    if(IsInCheck(color))
    {
        for(int i = 0; i < 8; i++)
        {
            for(int j = 0; j < 8; j++)
            {
                Piece p = m_board.GetPiece({ i, j });
                if(p.color != color)
                    continue;

                std::vector<Square> rm = GetRawMoves({ i,j });

                for(Square& r : rm)
                {
                    Piece original_to_piece = m_board.GetPiece(r);
                    m_board.MovePiece({ i,j }, r);
                    if(!IsInCheck(color))
                    {
                        m_board.MovePiece(r, { i,j });
                        m_board.SetPiece(r, original_to_piece);
                        return false;
                    }
                    m_board.MovePiece(r, { i,j });
                    m_board.SetPiece(r, original_to_piece);
                }


            }
        }
    }
    return true;
}


/// CHECKS IF STALEMATE HAPPEND OF A CERTAIN COLOR
bool MoveValidator::IsStalemate(Color color) const
{
    if(IsInCheck(color))
        return false;

    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            Piece p = m_board.GetPiece({ i, j });
            if(p.color != color)
                continue;

            if(GetLegalMoves({ i,j }).size() > 0)
                return false;
        }
    }

    return true;
}


/// GET RAW MOVES (ONLY INSIDE THE BOARD) FROM A SQUARE
std::vector<Square> MoveValidator::GetRawMoves(Square from) const
{
    Piece selected_piece = m_board.GetPiece(from);
    PieceType type = selected_piece.type;
    const std::vector<Square>& raw_moves = m_moveStorage.GetRawMovesOf(type);

    std::vector<Square> relative_raw_moves;
    relative_raw_moves.reserve(raw_moves.size());

    bool can_be_blocked = false;
    // these pieces get blocked if something is in the way
    if(type == PieceType::ROOK || type == PieceType::QUEEN || type == PieceType::BISHOP)
        can_be_blocked = true;

    // handle pawn separately
    if(type == PieceType::PAWN)
    {
        int is_black = m_board.GetPiece(from).color == Color::BLACK ? 1 : 0;

        for(const Square& rm : raw_moves)
        {
            Square actual_rm = { rm.row + is_black * 2 + from.row , rm.col + from.col };

            if(!m_board.IsInsideBounds(actual_rm))
                continue;

            Piece to_piece = m_board.GetPiece(actual_rm);
            if(to_piece.type == PieceType::NONE)
            {
                if(actual_rm.col == from.col)   // up/down move
                    relative_raw_moves.emplace_back(actual_rm);
            }
            else if(m_board.GetPiece(from).color != to_piece.color && actual_rm.col != from.col)
                relative_raw_moves.emplace_back(actual_rm);

        }
    }
    else if(!can_be_blocked)     // knight, king just needs to check if it is ally or enemy
    {
        for(const Square& rm : raw_moves)
        {
            Square actual_rm = { rm.row + from.row , rm.col + from.col };

            if(!m_board.IsInsideBounds(actual_rm))
                continue;
            else if(m_board.GetPiece(from).color != m_board.GetPiece(actual_rm).color)
                relative_raw_moves.emplace_back(actual_rm);
        }
    }
    else if(can_be_blocked)
    {
        Square temp_raw_move;
        int degree_of_freedom = raw_moves.size() / 7;   // only 7 moves per direction


        for(int i = 0; i < degree_of_freedom; i++)  // loop through each possible directions for movement
        {
            for(int j = 0; j < 7; j++)  // moves in a particular direction
            {
                Square rm = raw_moves[7 * i + j];

                // change from relative to {0,0} to current "from" square
                temp_raw_move = { from.row + rm.row, from.col + rm.col };
                if(!m_board.IsInsideBounds(temp_raw_move))
                    continue;

                if(!IsEmpty(temp_raw_move))
                {
                    // different color piece blocking so taking it is allowed
                    if(selected_piece.color != m_board.GetPiece(temp_raw_move).color)
                        relative_raw_moves.emplace_back(temp_raw_move);

                    // as the move is not empty meaning it will block that direction's further moves
                    break;
                }

                // the move is to empty square and isn't blocked by any piece 
                relative_raw_moves.emplace_back(temp_raw_move);
            }
        }
    }
    return relative_raw_moves;
}

bool MoveValidator::IsBlockingCheck(Square from, Square to) const
{

    Piece none_piece = { PieceType::NONE, Color::NONE };
    Piece original_from_piece = m_board.GetPiece(from);
    Piece original_to_piece = m_board.GetPiece(to);

    bool is_inside = m_board.IsInsideBounds(to);
    if(is_inside)
        m_board.MovePiece(from, to);
    else
        m_board.SetPiece(from, none_piece);

    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            Piece p = m_board.GetPiece({ i, j });

            if(p.color == m_currTurn || p.color == Color::NONE)
                continue;

            if(p.type == PieceType::KNIGHT || p.type == PieceType::PAWN)
                continue;


            if(IsInCheck(m_currTurn))
            {
                if(is_inside)
                {
                    m_board.SetPiece(from, original_from_piece);
                    m_board.SetPiece(to, original_to_piece);
                }
                else
                {
                    m_board.SetPiece(from, original_from_piece);
                }

                return true;
            }



        }
    }

    if(is_inside)
    {
        m_board.SetPiece(from, original_from_piece);
        m_board.SetPiece(to, original_to_piece);
    }
    else
    {
        m_board.SetPiece(from, original_from_piece);
    }
    return false;
}


/// CHECKS IF PERFORMING A MOVE LEAVES KING IN CHECK
bool MoveValidator::DoesLeaveKingInCheck(Square from, Square to) const
{
    return IsBlockingCheck(from, to);
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

    return { -1, -1 };
}


/// GETS THE LEGAL (ONLY PLAYABLE) MOVES OF A PIECE FROM A CERTAIN SQUARE
std::vector<Square> MoveValidator::GetLegalMovesOf(Piece piece, Square from) const
{
    std::vector<Square> legal_moves;
    switch(piece.type)
    {
    case PieceType::PAWN:
        legal_moves = GetLegalPawnMoves(piece, from);
        break;

    case PieceType::KNIGHT:
        legal_moves = GetLegalKnightMoves(piece, from);
        break;
    case PieceType::BISHOP:
        legal_moves = GetLegalBishopMoves(piece, from);
        break;
    case PieceType::ROOK:
        legal_moves = GetLegalRookMoves(piece, from);
        break;
    case PieceType::QUEEN:
        legal_moves = GetLegalQueenMoves(piece, from);
        break;
    case PieceType::KING:
        legal_moves = GetLegalKingMoves(piece, from);
        break;
    default:
        break;
    }

    return legal_moves;
}


/// GIVES ONLY PLAYABLE PAWN MOVES (HANDLES EN PASSSANT AS WELL)
std::vector<Square> MoveValidator::GetLegalPawnMoves(Piece piece, Square from) const
{
    std::vector<Square> valid_moves = GetRawMoves(from);
    std::vector<Square> legal_moves;
    legal_moves.reserve(valid_moves.size() + 1);

    // if never moved then add one extra move
    if(piece.color == Color::WHITE && from.row == 6) // second last row meaning never moved
    {
        if(m_board.GetPiece({ from.row - 2, from.col }).type == PieceType::NONE
           && m_board.GetPiece({ from.row - 1, from.col }).type == PieceType::NONE)
            valid_moves.emplace_back(from.row - 2, from.col);
    }
    else if(piece.color == Color::BLACK && from.row == 1)
    {
        if(m_board.GetPiece({ from.row + 2, from.col }).type == PieceType::NONE
           && m_board.GetPiece({ from.row + 1, from.col }).type == PieceType::NONE)
            valid_moves.emplace_back(from.row + 2, from.col);
    }

    // en passant
    if(m_moveHistory.size() > 0)
    {
        int result_offset = 0;
        Move last_move = m_moveHistory.back();
        if(last_move.played.type == PieceType::PAWN && last_move.played.color != m_currTurn)
        {
            int diff = last_move.from.row - last_move.to.row;
            if(diff == 2 || diff == -2)   // black pawn played two squares at once
            {
                if(last_move.to.row == from.row)
                {
                    if(last_move.to.col == from.col - 1)    // can en passant to left
                        result_offset = -1;
                    else if(last_move.to.col == from.col + 1)   // can en passant to right
                        result_offset = 1;
                }
                // diff/2 => -1 if black can en passant, +1 if white can en passant
                valid_moves.emplace_back(from.row + 1 * diff / 2, from.col + result_offset);
            }

        }
    }

    // Check if doing any moves leave the king in check
    for(Square& vm : valid_moves)
    {
        if(!DoesLeaveKingInCheck(from, vm))
            legal_moves.emplace_back(vm);
    }
    return legal_moves;
}


/// GET ONLY PLAYABLE KNIGHT MOVES
std::vector<Square> MoveValidator::GetLegalKnightMoves(Piece piece, Square from) const
{
    std::vector<Square> valid_moves = GetRawMoves(from);
    std::vector<Square> legal_moves;
    legal_moves.reserve(valid_moves.size());

    for(Square& vm : valid_moves)
    {
        if(!DoesLeaveKingInCheck(from, vm))
            legal_moves.emplace_back(vm);
    }
    return legal_moves;
}


/// GET ONLY PLAYABLE BISHOP MOVES
std::vector<Square> MoveValidator::GetLegalBishopMoves(Piece piece, Square from) const
{
    std::vector<Square> valid_moves = GetRawMoves(from);
    std::vector<Square> legal_moves;
    legal_moves.reserve(valid_moves.size());

    for(Square& vm : valid_moves)
    {
        if(!DoesLeaveKingInCheck(from, vm))
            legal_moves.emplace_back(vm);
    }
    return legal_moves;
}


/// GET ONLY PLAYABLE ROOK MOVES (CASTLING NOT YET HANDLED)
std::vector<Square> MoveValidator::GetLegalRookMoves(Piece piece, Square from) const
{
    std::vector<Square> valid_moves = GetRawMoves(from);
    std::vector<Square> legal_moves;
    legal_moves.reserve(valid_moves.size());

    for(Square& vm : valid_moves)
    {
        if(!DoesLeaveKingInCheck(from, vm))
            legal_moves.emplace_back(vm);
    }


    return legal_moves;
}


/// GET ONLY PLAYABLE QUEEN MOVES
std::vector<Square> MoveValidator::GetLegalQueenMoves(Piece piece, Square from) const
{
    std::vector<Square> valid_moves = GetRawMoves(from);
    std::vector<Square> legal_moves;
    legal_moves.reserve(valid_moves.size());

    for(Square& vm : valid_moves)
    {
        if(!DoesLeaveKingInCheck(from, vm))
            legal_moves.emplace_back(vm);
    }
    return legal_moves;
}


/// GET ONLY PLAYABLE KING MOVES (CASTLING, KING MECHANICS NOT YET HANDLED) 
std::vector<Square> MoveValidator::GetLegalKingMoves(Piece piece, Square from) const
{
    std::vector<Square> valid_moves = GetRawMoves(from);
    std::vector<Square> legal_moves;
    legal_moves.reserve(valid_moves.size());

    for(Square& vm : valid_moves)
    {
        if(!DoesLeaveKingInCheck(from, vm))
            legal_moves.emplace_back(vm);
    }

    // CASTLING
    // check if king has never moved
    bool king_moved = false;
    for(const Move& m : m_moveHistory)
        if(m.played.type == PieceType::KING && m.played.color == piece.color)
            king_moved = true;

    if(!king_moved && !IsInCheck(piece.color))
    {
        int rook_cols[] = { 0, 7 };
        for(int rook_col : rook_cols)
        {
            Piece candidate = m_board.GetPiece({ from.row, rook_col });
            if(candidate.type != PieceType::ROOK || candidate.color != piece.color)
                continue;

            // check if the castling rook hasn't moved
            bool this_rook_moved = false;
            for(const Move& m : m_moveHistory)
                if(m.from.row == from.row && m.from.col == rook_col)
                {
                    this_rook_moved = true;
                    break;
                }
            if(this_rook_moved) continue;

            // check if path is clear between king and rook
            int step = (rook_col > from.col) ? 1 : -1;
            bool path_clear = true;
            for(int col = from.col + step; col != rook_col; col += step)
                if(m_board.GetPiece({ from.row, col }).type != PieceType::NONE)
                {
                    path_clear = false;
                    break;
                }
            if(!path_clear) continue;

            // check if king can passes through and land on safe squares
            Square king_pass = { from.row, from.col + step };
            Square king_land = { from.row, from.col + step * 2 };

            if(!DoesLeaveKingInCheck(from, king_pass) && !DoesLeaveKingInCheck(from, king_land))
                legal_moves.emplace_back(king_land);
        }

    }
    return legal_moves;
}
