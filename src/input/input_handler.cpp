#include "input/input_handler.hpp"
#include <iostream>


/// CONSTRUCTOR
InputHandler::InputHandler(GameState& state, Renderer& renderer)
    : m_state(state), m_renderer(renderer)
{
}


/// HANDLE THE PASSED EVENT
void InputHandler::HandleEvent(const sf::Event& event)
{
    if(const auto* mousePressed = event.getIf<sf::Event::MouseButtonPressed>())
    {
        if(mousePressed->button == sf::Mouse::Button::Left)
            HandleClick({ mousePressed->position.x, mousePressed->position.y });
    }
    else if(const auto* keyPressed = event.getIf<sf::Event::KeyReleased>())
    {
        if(keyPressed->code == sf::Keyboard::Key::U)
            m_state.Undo();
        else if(keyPressed->code == sf::Keyboard::Key::R)
            m_state.Reset();
        else if(keyPressed->code == sf::Keyboard::Key::S)
            m_state.SwitchTurn();
    }
}


void InputHandler::HandleHUDClick(sf::Vector2i mousePos)
{
    Square clicked = m_renderer.ScreenToSquareHUD(mousePos);

    // clicked in the board area
    if(clicked.row == -1 || clicked.col == -1)
        return;

    // Find which part of the hud was clicked (1 -> top, 2 -> left, 3 -> right, 4 -> bottom)
    int hud_part = SquareToHUDPart(clicked);

    // not clicked on valid hud part
    switch(hud_part)
    {
    case 1:
        HandleTopHUDClick(clicked);
        break;

    case 2:
        HandleLeftHUDClick(clicked);
        break;
    case 3:
        HandleRightHUDClick(clicked);
        break;
    case 4:
        HandleBottomHUDClick(clicked);
        break;
    default:
        break;
    }
}

/// HANDLE CLICK (EVERYTHING RELATED TO CLICKING STARTS FROM HERE)
void InputHandler::HandleClick(sf::Vector2i mousePos)
{
    Square clicked = m_renderer.ScreenToSquare(mousePos);

    // ignore clicks in HUD/padding area
    if(clicked.row == -1 || clicked.col == -1)
    {
        HandleHUDClick(mousePos);
        return;
    }

    if(m_state.IsPromotionOngoing())
        return;

    // update selected square
    HandleSelection(clicked);

    std::optional<Square> selected = m_state.GetSelectedSquare();
    if(selected.has_value())
        return;

    // move logic here


    // clear selection after clicking somewhere else
    m_state.SetSelectedSquare(std::nullopt);
    m_state.SetCachedLegalMoves({});
}

void InputHandler::HandleTopHUDClick(Square clicked)
{
}

void InputHandler::HandleLeftHUDClick(Square clicked)
{
}

void InputHandler::HandleRightHUDClick(Square clicked)
{
}

void InputHandler::HandleBottomHUDClick(Square clicked)
{
    // Bottom is only used for promotion as of now
    if(m_state.IsPromotionOngoing())
    {
        if(clicked.col > 4) // not clicked within promotion range
            return;
        const std::array<PieceType, 4> order = {
            PieceType::ROOK, PieceType::KNIGHT,
            PieceType::BISHOP, PieceType::QUEEN
        };
        PieceType clicked_piece = order[clicked.col - 1];    // shiffted right by 1
        m_state.SetPromotedPiece(clicked_piece);

    }
}

int InputHandler::SquareToHUDPart(Square clicked)
{
    int hud_part = -1;
    if(clicked.row == 0) hud_part = 1;
    else if(clicked.row == 9) hud_part = 4;
    else if(clicked.col == 0) hud_part = 2;
    else if(clicked.col == 9) hud_part = 3;

    return hud_part;
}


/// HANDLE SELECTION OF SQUARE 
void InputHandler::HandleSelection(Square clicked)
{
    std::optional<Square> selected = m_state.GetSelectedSquare();

    // nothing selected yet, tries to select if valid selection
    if(!selected.has_value())
    {
        const Board& board = m_state.GetBoard();
        Piece piece = board.GetPiece(clicked);

        // only select if clicked own piece
        if(piece.IsEmpty() || piece.color != m_state.GetCurrentTurn())
            return;

        m_state.SetSelectedSquare(clicked);
        m_state.SetCachedLegalMoves(m_state.GetLegalMoves(clicked));
        return;
    }

    Square from = selected.value();

    // clicked the same square, deselect
    if(clicked.row == from.row && clicked.col == from.col)
    {
        m_state.SetSelectedSquare(std::nullopt);
        m_state.SetCachedLegalMoves({});
        return;
    }

    // clicked a different own piece, reselect
    const Board& board = m_state.GetBoard();
    Piece piece = board.GetPiece(clicked);
    if(!piece.IsEmpty() && piece.color == m_state.GetCurrentTurn())
    {
        m_state.SetSelectedSquare(clicked);
        m_state.SetCachedLegalMoves(m_state.GetLegalMoves(clicked));
        return;
    }

    // clicked on a legal move
    for(const Square& lm : m_state.GetCachedLegalMoves())
    {
        if(clicked.row == lm.row && clicked.col == lm.col)
        {
            m_state.DoMove(from, lm);
            m_state.SwitchTurn();
            m_state.UpdateStatus();
            break;
        }
    }
}
