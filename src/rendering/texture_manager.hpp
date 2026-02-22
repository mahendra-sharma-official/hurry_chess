#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>

class TextureManager
{
public:
  bool Load(const std::string& id, const std::string& filepath);
  const sf::Texture& Get(const std::string& id) const;
  bool Has(const std::string& id) const;

private:
  std::unordered_map<std::string, sf::Texture> m_textures;
};