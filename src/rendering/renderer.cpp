#include "renderer.hpp"

Renderer::Renderer(sf::RenderWindow& window, TextureManager& textures)
  : m_window(window), m_textures(textures)
{
}

void Renderer::DrawBoard()
{
}

void Renderer::DrawPieces(const Board& board)
{
}

void Renderer::DrawHighlights(const std::vector<Square>& squares)
{
}

void Renderer::DrawSelectedSquare(Square sq)
{
}

void Renderer::Render(const GameState& state, std::optional<Square> selected)
{
}

Square Renderer::ScreenToSquare(sf::Vector2i mousePos) const
{
  return Square();
}

sf::Color Renderer::GetTileColor(Square sq) const
{
  return sf::Color();
}

sf::FloatRect Renderer::GetSquareBounds(Square sq) const
{
  return sf::FloatRect();
}
