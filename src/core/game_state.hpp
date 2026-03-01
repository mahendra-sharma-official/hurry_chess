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


class GameState
{

private:
    Board m_board;
    MoveValidator m_validator;
    Color m_currentTurn;
    Color m_visualCurrentTurn;
    GameStatus m_gameStatus;
    BoardStatus m_boardStatus;
    std::vector<Move> m_moveHistory;
    std::optional<Square> m_selected;
    std::vector<Square> m_legalMoves;
    std::unordered_map<Color, std::vector<PieceType>> m_capturedPieces;
    bool m_isPromotion;


public:
    GameState();

    void DoMove(Square from, Square to);
    void Undo();
    void Reset();

    Color GetCurrentTurn() const;
    GameStatus GetGameStatus() const;
    BoardStatus GetBoardStatus() const;
    const Board& GetBoard() const;
    const std::vector<Move>& GetMoveHistory() const;
    std::vector<Square> GetLegalMoves(Square from) const;
    std::optional<Square> GetSelectedSquare() const;
    const std::vector<Square>& GetCachedLegalMoves() const;
    const std::vector<PieceType>& GetCapturedPieces(Color color) const;
    bool IsPromotionOngoing() const;


    void SetGameStatus(GameStatus status);
    void SetBoardStatus(BoardStatus status);
    void SetCachedLegalMoves(std::vector<Square> moves);
    void SetSelectedSquare(std::optional<Square> selected);
    void SetPromotedPiece(PieceType promoted);
    void AddToMoveHistory(Move move);

    void SwitchTurn();
    void UpdateStatus();

};