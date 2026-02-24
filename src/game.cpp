#include "game.hpp"
#include <iostream>

Game::Game()
    : m_window(), m_textures(), m_state()
    , m_renderer(Renderer(m_window, m_textures)), m_input(m_state, m_renderer)
{

}

void Game::Init(sf::Vector2u windowSize, std::string windowTitle)
{
    m_window = sf::RenderWindow(sf::VideoMode(windowSize), windowTitle);
    m_renderer.UpdateViews(m_window.getSize());
    LoadAssets();
}

void Game::Run()
{

    while(m_state.GetGameStatus() == GameStatus::PLAYING)
    {
        ProcessEvents();
        Update();
        Draw();

    }
}

void Game::ProcessEvents()
{
    while(const std::optional<sf::Event> event = m_window.pollEvent())
    {
        if(event->is<sf::Event::Closed>())
        {
            m_window.close();
            m_state.SetGameStatus(GameStatus::ENDING);
        }
        if(const auto* resized = event->getIf<sf::Event::Resized>())
            m_renderer.UpdateViews(resized->size);

        m_input.HandleEvent(*event); // forward everything else to input handler
    }
}

void Game::Update()
{
}

void Game::Draw()
{
    m_renderer.Render(m_state);
}

bool Game::LoadAssets()
{
    if(m_textures.Load("pieces", "assets/textures/pieces.png", { 64,64 }))
        std::cout << "Loaded pieces successfully" << std::endl;
    return false;
}
