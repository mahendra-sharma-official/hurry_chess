#include "core/game_state.hpp"

/// CONSTRUCTOR
GameState::GameState()
    : m_board(), m_validator(m_board), m_currentTurn(Color::WHITE)
    , m_gameStatus(GameStatus::PLAYING), m_boardStatus(BoardStatus::NORMAL)
{
}



/// CHECKS IF DOING A MOVE IS VALID
bool GameState::TryMove(Square from, Square to)
{
    return false;
}

void GameState::Reset()
{
}

Color GameState::GetCurrentTurn() const
{
    return m_currentTurn;
}

GameStatus GameState::GetGameStatus() const
{
    return m_gameStatus;
}

BoardStatus GameState::GetBoardStatus() const
{
    return m_boardStatus;
}

const Board& GameState::GetBoard() const
{
    return m_board;
}

const std::vector<Move>& GameState::GetMoveHistory() const
{
    return m_moveHistory;
}

std::vector<Square> GameState::GetLegalMoves(Square from) const
{
    return m_validator.GetLegalMoves(from);
}

std::optional<Square> GameState::GetSelectedSquare() const
{
    return m_selected;
}

std::vector<Square> GameState::GetCachedLegalMoves() const
{
    return m_legalMoves;
}

void GameState::SetGameStatus(GameStatus status)
{
    m_gameStatus = status;
}

void GameState::SetLegalMoves(std::vector<Square> moves)
{
    m_legalMoves = moves;
}

void GameState::SetSelectedSquare(std::optional<Square> selected)
{
    m_selected = selected;
}

void GameState::SwitchTurn()
{
    m_currentTurn = (m_currentTurn == Color::WHITE ? Color::BLACK : Color::WHITE);
}

void GameState::UpdateStatus()
{

}
