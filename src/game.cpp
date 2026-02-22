#include "game.hpp"

Game::Game()
  : m_window(), m_textures(), m_state()
  , m_renderer(Renderer(m_window, m_textures)), m_input(m_state, m_renderer)
{
}

void Game::Run()
{
}

void Game::ProcessEvents()
{
}

void Game::Update()
{
}

void Game::Draw()
{
}

bool Game::LoadAssets()
{
  return false;
}
