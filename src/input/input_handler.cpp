#include "input/input_handler.hpp"
#include <iostream>

InputHandler::InputHandler(GameState& state, Renderer& renderer)
    : m_state(state), m_renderer(renderer)
{
}

void InputHandler::HandleEvent(const sf::Event& event)
{
    if(const auto* mousePressed = event.getIf<sf::Event::MouseButtonPressed>())
    {
        if(mousePressed->button == sf::Mouse::Button::Left)
            HandleClick({ mousePressed->position.x, mousePressed->position.y });
    }

}

void InputHandler::HandleClick(sf::Vector2i mousePos)
{
    Square clicked = m_renderer.ScreenToSquare(mousePos);

    // ignore clicks in HUD/padding area
    if(clicked.row == -1 || clicked.col == -1)
        return;

    std::optional<Square> selected = m_state.GetSelectedSquare();

    // nothing selected yet — try to select a piece
    if(!selected.has_value())
    {
        const Board& board = m_state.GetBoard();
        Piece piece = board.GetPiece(clicked);

        // only select if there's a piece belonging to current player
        if(piece.IsEmpty() || piece.color != m_state.GetCurrentTurn())
            return;

        m_state.SetSelectedSquare(clicked);
        m_state.SetLegalMoves(m_state.GetLegalMoves(clicked)); // cache legal moves for renderer
        return;
    }

    // something already selected
    Square from = selected.value();

    // clicked the same square — deselect
    if(clicked.row == from.row && clicked.col == from.col)
    {
        m_state.SetSelectedSquare(std::nullopt);
        m_state.SetLegalMoves({});  // empty legal moves
        return;
    }

    // clicked a different own piece — reselect
    const Board& board = m_state.GetBoard();
    Piece piece = board.GetPiece(clicked);
    if(!piece.IsEmpty() && piece.color == m_state.GetCurrentTurn())
    {
        m_state.SetSelectedSquare(clicked);
        m_state.SetLegalMoves(m_state.GetLegalMoves(clicked));
        return;
    }

    // move logic here
    // (NOT YET IMPLEMENTED)

    // clear selection after clicking somewhere
    m_state.SetSelectedSquare(std::nullopt);
    m_state.SetLegalMoves({});
}