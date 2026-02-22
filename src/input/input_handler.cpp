#include "input/input_handler.hpp"

InputHandler::InputHandler(GameState& state, Renderer& renderer)
{
}

void InputHandler::HandleEvent(const sf::Event& event)
{
}

std::optional<Square> InputHandler::GetSelectedSquare() const
{
  return std::optional<Square>();
}

void InputHandler::HandleClick(sf::Vector2i mousePos)
{
}
