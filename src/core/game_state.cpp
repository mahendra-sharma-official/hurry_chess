#include "game_state.hpp"

GameState::GameState()
  : m_board(), m_validator(m_board), m_currentTurn(Color::NONE), m_status(GameStatus::PLAYING)
{
}

bool GameState::TryMove(Square from, Square to)
{
  return false;
}

void GameState::Reset()
{
}

Color GameState::GetCurrentTurn() const
{
  return Color();
}

GameStatus GameState::GetStatus() const
{
  return GameStatus();
}

const Board& GameState::GetBoard() const
{
  // TODO: insert return statement here
}

const std::vector<Move>& GameState::GetMoveHistory() const
{
  // TODO: insert return statement here
}

void GameState::SwitchTurn()
{
}

void GameState::UpdateStatus()
{
}
