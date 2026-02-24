#include "core/move_storage.hpp"
#include "core/piece.hpp"


/// CONSTRUCTOR (DOES NOTHING)
MoveStorage::MoveStorage()
{
}



/// GETS RAW (ALL POSSIBLE, INVALID ALSO) MOVES RELATIVE TO PIECE BEING AT {0,0} 
const std::vector<Square>& MoveStorage::GetRawMovesOf(PieceType pieceType) const
{
    return m_moves.at(pieceType);
}



/// SETS AND STORES ALL PIECES MOVES
void MoveStorage::SetAllMoves()
{
    SetPawnMoves();
    SetKnightMoves();
    SetBishopMoves();
    SetRookMoves();
    SetQueenMoves();
    SetKingMoves();
}



/// SETS PAWN MOVE
void MoveStorage::SetPawnMoves()
{
    std::vector<Square> pawn_moves;

    // 1 move upward
    pawn_moves.emplace_back(-1, 0);

    m_moves[PieceType::PAWN] = pawn_moves;
}




/// SETS KNIGHT MOVES
void MoveStorage::SetKnightMoves()
{
    std::vector<Square> knight_moves;
    knight_moves.reserve(8);

    // 1 move vertical/horizontal combined with 2 moves horizontal/vertical

    // moves above knight 
    knight_moves.emplace_back(-1, 2);   // right
    knight_moves.emplace_back(-1, -2);  // left
    knight_moves.emplace_back(-2, 1);   // right
    knight_moves.emplace_back(-2, -1);  // left

    // moves below knight
    knight_moves.emplace_back(1, 2);    // right
    knight_moves.emplace_back(1, -2);   // left
    knight_moves.emplace_back(2, 1);    // right
    knight_moves.emplace_back(2, -1);   // left

    m_moves[PieceType::KNIGHT] = knight_moves;

}




/// SETS BISHOP MOVES
void MoveStorage::SetBishopMoves()
{
    std::vector<Square> bishop_moves;
    bishop_moves.reserve(28);

    // diagonal moves
    for(int i = 1; i < 8; i++)
    {
        bishop_moves.emplace_back(-i, i);   // to top right 
        bishop_moves.emplace_back(-i, -i);  // to top left 
        bishop_moves.emplace_back(i, i);    // to bottom right 
        bishop_moves.emplace_back(i, -i);   // to bottom left
    }

    m_moves[PieceType::BISHOP] = bishop_moves;

}




/// SETS ROOK MOVES
void MoveStorage::SetRookMoves()
{
    std::vector<Square> rook_moves;
    rook_moves.reserve(28);

    // straight moves
    for(int i = 1; i < 8; i++)
    {
        rook_moves.emplace_back(-i, 0); // to top 
        rook_moves.emplace_back(i, 0);  // to bottom
        rook_moves.emplace_back(0, i);  // to right 
        rook_moves.emplace_back(0, -i); // to left 
    }

    m_moves[PieceType::ROOK] = rook_moves;
}




/// SETS QUEEN MOVES
void MoveStorage::SetQueenMoves()
{
    std::vector<Square> queen_moves;
    queen_moves.reserve(56);


    for(int i = 1; i < 8; i++)
    {
        // straight moves
        queen_moves.emplace_back(-i, 0); // to top 
        queen_moves.emplace_back(i, 0);  // to bottom
        queen_moves.emplace_back(0, i);  // to right 
        queen_moves.emplace_back(0, -i); // to left 

        // diagonal moves
        queen_moves.emplace_back(-i, i);   // to top right 
        queen_moves.emplace_back(-i, -i);  // to top left 
        queen_moves.emplace_back(i, i);    // to bottom right 
        queen_moves.emplace_back(i, -i);   // to bottom left
    }

    m_moves[PieceType::QUEEN] = queen_moves;

}




/// SETS KING MOVES
void MoveStorage::SetKingMoves()
{
    std::vector<Square> king_moves;
    king_moves.reserve(8);

    // straight moves
    king_moves.emplace_back(-1, 0); // to top 
    king_moves.emplace_back(1, 0);  // to bottom
    king_moves.emplace_back(0, 1);  // to right 
    king_moves.emplace_back(0, -1); // to left 

    // diagonal moves
    king_moves.emplace_back(-1, 1);  // to top right 
    king_moves.emplace_back(-1, -1); // to top left 
    king_moves.emplace_back(1, 1);   // to bottom right
    king_moves.emplace_back(1, -1);  // to bottom left

    m_moves[PieceType::KING] = king_moves;
}
