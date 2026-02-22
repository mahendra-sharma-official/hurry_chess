#pragma once
#include "board.hpp"
#include "move_validator.hpp"
#include <vector>

enum class GameStatus
{
  PLAYING, WHITEINCHECK, BLACKINCHECK,
  CHEKMATE, WHITEWIN, BLACKWIN, STALEMATE
};

struct Move
{
  Square from, to;
  Piece captured;
};

class GameState
{

private:
  Board m_board;
  MoveValidator m_validator;
  Color m_currentTurn;
  GameStatus m_status;
  std::vector<Move> m_moveHistory;


public:
  GameState();

  bool TryMove(Square from, Square to);
  void Reset();

  Color GetCurrentTurn() const;
  GameStatus GetStatus() const;
  const Board& GetBoard() const;
  const std::vector<Move>& GetMoveHistory() const;

private:
  void SwitchTurn();
  void UpdateStatus();
};