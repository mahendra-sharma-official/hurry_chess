#pragma once
#include "board.hpp"
#include "move_validator.hpp"
#include <vector>
#include <optional>

enum class BoardStatus
{
    NORMAL, WHITEINCHECK, BLACKINCHECK,
    CHEKMATE, WHITEWIN, BLACKWIN, STALEMATE
};

enum class GameStatus
{
    PLAYING, PAUSING, ENDING
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
    GameStatus m_gameStatus;
    BoardStatus m_boardStatus;
    std::vector<Move> m_moveHistory;
    std::optional<Square> m_selected;
    std::vector<Square> m_legalMoves;


public:
    GameState();

    bool TryMove(Square from, Square to);
    void Reset();

    Color GetCurrentTurn() const;
    GameStatus GetGameStatus() const;
    BoardStatus GetBoardStatus() const;
    const Board& GetBoard() const;
    const std::vector<Move>& GetMoveHistory() const;
    std::vector<Square> GetLegalMoves(Square from) const;
    std::optional<Square> GetSelectedSquare() const;
    std::vector<Square> GetCachedLegalMoves() const;


    void SetGameStatus(GameStatus status);
    void SetLegalMoves(std::vector<Square> moves);
    void SetSelectedSquare(std::optional<Square> selected);

private:
    void SwitchTurn();
    void UpdateStatus();
};