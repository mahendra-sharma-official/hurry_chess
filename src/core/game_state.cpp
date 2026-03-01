#include "core/game_state.hpp"
#include <iostream>

/// CONSTRUCTOR
GameState::GameState()
    : m_board(), m_validator(m_board, m_currentTurn, m_moveHistory), m_currentTurn(Color::WHITE)
    , m_visualCurrentTurn(Color::WHITE), m_gameStatus(GameStatus::PLAYING)
    , m_boardStatus(BoardStatus::NORMAL), m_isPromotion(false)
{
    m_capturedPieces[Color::WHITE] = {};
    m_capturedPieces[Color::BLACK] = {};
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
        {
            on = { from.row, to.col };
        }

        // promotion if reached last row
        if((to.row == 0 && fpiece.color == Color::WHITE) ||
           (to.row == 7 && fpiece.color == Color::BLACK))
            m_isPromotion = true;
    }

    // if something was captured store it
    if(m_board.GetPiece(on).type != PieceType::NONE)
    {
        SwitchTurn();
        m_capturedPieces[m_currentTurn].emplace_back(m_board.GetPiece(on).type);
        SwitchTurn();
    }

    // handle king castling
    if(fpiece.type == PieceType::KING)
    {
        // this means castling move was done
        if(to.col - from.col == 2)
        {
            m_board.SetPiece({ from.row, to.col - 1 }, m_board.GetPiece({ from.row, 7 }));
            on = { from.row, 7 };
        }
        else if(to.col - from.col == -2)
        {
            m_board.SetPiece({ from.row, to.col + 1 }, m_board.GetPiece({ from.row, 0 }));
            on = { from.row, 0 };
        }

    }

    AddToMoveHistory({ from, to, on, m_board.GetPiece(from), m_board.GetPiece(on) });
    m_board.SetPiece(on, {});   // empty the captured piece
    m_board.MovePiece(from, to);

    // handles promotion separately

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
    {
        // since undo does things in order we can safely remove last captured piece
        if(m_capturedPieces[m_currentTurn].size() != 0
           && last_move.captured.type == m_capturedPieces[m_currentTurn].back())
            m_capturedPieces[m_currentTurn].pop_back();

        m_board.SetPiece(last_move.on, last_move.captured);

        // handle castling undo
        if(last_move.played.type == PieceType::KING && last_move.captured.type == PieceType::ROOK)
        {
            // can only happen in castling (according to my logic)
            if(last_move.played.color == last_move.captured.color)
            {
                int diff = last_move.to.col - last_move.from.col;
                if(diff == 2)
                    m_board.SetPiece({ last_move.to.row, last_move.to.col - 1 }, {});
                else if(diff == -2)
                    m_board.SetPiece({ last_move.to.row, last_move.to.col + 1 }, {});

            }

        }
    }

    m_moveHistory.pop_back();
    SwitchTurn();
}


/// RESET THE GAME STATE
void GameState::Reset()
{
    m_gameStatus = GameStatus::PLAYING;
    m_currentTurn = Color::WHITE;
    m_visualCurrentTurn = Color::WHITE;
    m_isPromotion = false;
    m_selected = std::nullopt;
    m_moveHistory.clear();
    m_legalMoves.clear();
    m_capturedPieces.clear();
    m_board.Reset();

    m_capturedPieces[Color::WHITE] = {};
    m_capturedPieces[Color::BLACK] = {};
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

const std::vector<PieceType>& GameState::GetCapturedPieces(Color color) const
{
    return m_capturedPieces.at(color);
}

bool GameState::IsPromotionOngoing() const
{
    return m_isPromotion;
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

void GameState::SetPromotedPiece(PieceType promoted)
{
    // since promotion piece is selected when in opponents turn reverse the turn for now
    SwitchTurn();
    Move last_move = m_moveHistory.back();

    if(last_move.played.type == PieceType::PAWN)
        m_board.SetPiece(last_move.to, { promoted, m_currentTurn });
    m_isPromotion = false;

    // make the turn normal
    SwitchTurn();

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
    SetBoardStatus(BoardStatus::NORMAL);
    if(m_validator.IsInCheck(m_currentTurn))
    {

        SetBoardStatus(m_currentTurn == Color::WHITE ? BoardStatus::WHITEINCHECK : BoardStatus::BLACKINCHECK);

        if(m_validator.IsCheckmate(m_currentTurn))
        {
            SetBoardStatus(BoardStatus::CHEKMATE);
            SetBoardStatus(m_currentTurn == Color::WHITE ? BoardStatus::BLACKWIN : BoardStatus::WHITEWIN);
        }

    }
    else if(m_validator.IsStalemate(m_currentTurn))
    {
        SetBoardStatus(BoardStatus::STALEMATE);
    }

}
