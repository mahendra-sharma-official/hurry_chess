#pragma once
#include <SFML/Graphics.hpp>
#include "core/game_state.hpp"
#include "rendering/texture_manager.hpp"
#include <optional>

class Renderer
{
public:
  Renderer(sf::RenderWindow& window, TextureManager& textures);

  void DrawBoard();
  void DrawPieces(const Board& board);
  void DrawHighlights(const std::vector<Square>& squares);
  void DrawSelectedSquare(Square sq);
  void Render(const GameState& state, std::optional<Square> selected);

  Square ScreenToSquare(sf::Vector2i mousePos) const;

private:
  sf::RenderWindow& m_window;
  TextureManager& m_textures;

  static constexpr float TILE_SIZE = 80.f;

  sf::Color GetTileColor(Square sq) const;
  sf::FloatRect GetSquareBounds(Square sq) const;
};