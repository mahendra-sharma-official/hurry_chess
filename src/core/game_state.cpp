#include "core/game_state.hpp"

/// CONSTRUCTOR
GameState::GameState()
    : m_board(), m_validator(m_board, m_currentTurn, m_moveHistory), m_currentTurn(Color::WHITE)
    , m_gameStatus(GameStatus::PLAYING), m_boardStatus(BoardStatus::NORMAL)
{
}


/// DOES A MOVE ON BOARD
void GameState::DoMove(Square from, Square to)
{
    Piece fpiece = m_board.GetPiece(from);
    Piece tpiece = m_board.GetPiece(to);

    Square on = to;
    if(fpiece.type == PieceType::PAWN)
    {
        // going to side and their not being a piece means it's en passant
        if(to.col != from.col && tpiece.type == PieceType::NONE)
            on = { from.row, to.col };
    }

    AddToMoveHistory({ from, to, on, m_board.GetPiece(from), m_board.GetPiece(to) });


    m_board.MovePiece(from, to);
    SetSelectedSquare(std::nullopt);
    SetCachedLegalMoves({});
}


/// REVERSE THE LAST MOVE
void GameState::Undo()
{
    int len = m_moveHistory.size();
    if(len == 0)
        return;

    Move last_move = m_moveHistory.back();

    m_board.SetPiece(last_move.from, last_move.played);
    m_board.SetPiece(last_move.to, {});
    if(last_move.captured.type != PieceType::NONE)
        m_board.SetPiece(last_move.on, last_move.captured);

    m_moveHistory.pop_back();
    SwitchTurn();
}


/// RESET THE GAME STATE
void GameState::Reset()
{
}


/// GETS CURRENT TURN (COLOR)
Color GameState::GetCurrentTurn() const
{
    return m_currentTurn;
}


/// GET GAME'S CURRENT STATUS
GameStatus GameState::GetGameStatus() const
{
    return m_gameStatus;
}


/// GET BOARD'S CURRENT STATUS
BoardStatus GameState::GetBoardStatus() const
{
    return m_boardStatus;
}


/// GET REFERENCE TO BOARD
const Board& GameState::GetBoard() const
{
    return m_board;
}


/// GET SUCCESSFUL MOVES' MOVE HISTORY 
const std::vector<Move>& GameState::GetMoveHistory() const
{
    return m_moveHistory;
}


/// GET PLAYABLE VALID MOVES FROM A SQUARE
std::vector<Square> GameState::GetLegalMoves(Square from) const
{

    return m_validator.GetLegalMoves(from);
}


/// GET THE SELECTED SQUARE (OPTIONAL)
std::optional<Square> GameState::GetSelectedSquare() const
{
    return m_selected;
}


/// GET THE PREVIOUSLY CACHED LEGAL MOVES 
const std::vector<Square>& GameState::GetCachedLegalMoves() const
{
    return m_legalMoves;
}


/// SET THE GAME'S STATUS 
void GameState::SetGameStatus(GameStatus status)
{
    m_gameStatus = status;
}

void GameState::SetBoardStatus(BoardStatus status)
{
    m_boardStatus = status;
}


/// SET THE LEGAL MOVES (CACHED)
void GameState::SetCachedLegalMoves(std::vector<Square> moves)
{
    m_legalMoves = moves;
}


/// SET THE CACHED SELECTED SQUARE
void GameState::SetSelectedSquare(std::optional<Square> selected)
{
    m_selected = selected;
}

void GameState::AddToMoveHistory(Move move)
{
    m_moveHistory.emplace_back(move);
}


/// SWITCH THE TURN TO OPPOSITE OF CURRENT TURN
void GameState::SwitchTurn()
{
    m_currentTurn = (m_currentTurn == Color::WHITE ? Color::BLACK : Color::WHITE);
}


/// UPDATES THE STATUS DEPENDING ON THE CURRENT GAME STATE
void GameState::UpdateStatus()
{
    if(m_validator.IsInCheck(m_currentTurn))
        SetBoardStatus(m_currentTurn == Color::WHITE ? BoardStatus::WHITEINCHECK : BoardStatus::BLACKINCHECK);
}
