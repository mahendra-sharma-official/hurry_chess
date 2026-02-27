#pragma once
#include <SFML/Graphics.hpp>
#include "core/game_state.hpp"
#include "rendering/texture_manager.hpp"
#include <optional>

class Renderer
{
public:
    Renderer(sf::RenderWindow& window, TextureManager& textures);

    void Render(GameState& state);
    void UpdateViews(sf::Vector2u windowSize);

    // coordinate conversion
    Square ScreenToSquare(sf::Vector2i mousePos) const;
    sf::Vector2f ScreenToWorld(sf::Vector2i mousePos) const;

private:
    sf::RenderWindow& m_window;
    TextureManager& m_textures;

    sf::View m_view;

    static constexpr float TILE_SIZE = 64.f;
    static constexpr float BOARD_SIZE = TILE_SIZE * 8;
    static constexpr float PADDING = TILE_SIZE;
    static constexpr float VIRTUAL_SIZE = BOARD_SIZE + PADDING * 2;
    static constexpr float SCALE_FACTOR = 0.95;
    static constexpr sf::Color LIGHT_COLOR = sf::Color(240, 217, 181);
    static constexpr sf::Color DARK_COLOR = sf::Color(181, 136, 99);
    static constexpr sf::Color HIGHLIGHT_COLOR = sf::Color(0, 0, 0, 80);
    static constexpr sf::Color SELECT_COLOR = sf::Color(255, 255, 0, 100);



    // board drawing
    void DrawBoard();
    void DrawPieces(const Board& board);
    void DrawHighlights(const std::vector<Square>& squares);
    void DrawSelectedSquare(std::optional<Square> selected);

    // hud drawing
    void DrawHUD(const GameState& state);
    void DrawTopBar(const GameState& state);
    void DrawBottomBar(const GameState& state);
    void DrawSidePanels(const GameState& state);

    // helpers
    sf::Color GetTileColor(Square sq) const;
    sf::FloatRect GetSquareBounds(Square sq) const;
    sf::IntRect GetPieceTextureRect(Piece piece) const;
};