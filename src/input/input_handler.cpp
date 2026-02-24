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
}


/// HANDLE CLICK (EVERYTHING RELATED TO CLICKING STARTS FROM HERE)
void InputHandler::HandleClick(sf::Vector2i mousePos)
{
    Square clicked = m_renderer.ScreenToSquare(mousePos);

    // ignore clicks in HUD/padding area
    if(clicked.row == -1 || clicked.col == -1)
        return;

    // update selected square
    HandleSelection(clicked);

    std::optional<Square> selected = m_state.GetSelectedSquare();
    if(selected.has_value())
        return;

    // move logic here
    // (NOT YET IMPLEMENTED)

    // clear selection after clicking somewhere else
    m_state.SetSelectedSquare(std::nullopt);
    m_state.SetCachedLegalMoves({});
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

    // clicked the same square then deselect
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
}
