#pragma once
#include <SFML/Graphics.hpp>
#include "core/game_state.hpp"
#include "rendering/renderer.hpp"
#include "rendering/texture_manager.hpp"
#include "input/input_handler.hpp"


class Game
{

private:
    sf::RenderWindow m_window;
    TextureManager m_textures;
    GameState m_state;
    Renderer m_renderer;
    InputHandler m_input;


public:
    Game();
    void Init(sf::Vector2u windowSize, std::string windowTitle);
    void Run();


private:
    void ProcessEvents();
    void Update();
    void Draw();

    bool LoadAssets();
};