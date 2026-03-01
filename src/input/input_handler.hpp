#pragma once
#include <SFML/Graphics.hpp>
#include "core/game_state.hpp"
#include "rendering/renderer.hpp"
#include <optional>

class InputHandler
{
public:
    InputHandler(GameState& state, Renderer& renderer);

    void HandleEvent(const sf::Event& event);
    void HandleSelection(Square selected);

private:
    GameState& m_state;
    Renderer& m_renderer;

    void HandleHUDClick(sf::Vector2i mousePos);
    void HandleClick(sf::Vector2i mousePos);

    // helpers for hud clicking
    void HandleTopHUDClick(Square clicked);
    void HandleLeftHUDClick(Square clicked);
    void HandleRightHUDClick(Square clicked);
    void HandleBottomHUDClick(Square clicked);
    int SquareToHUDPart(Square clicked);
};