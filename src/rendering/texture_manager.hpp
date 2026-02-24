#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>

struct TextureData
{
    sf::Texture texture;
    sf::Vector2u spriteSize; // {0,0} for non-spritesheet textures
};

class TextureManager
{
public:
    bool Load(const std::string& id, const std::string& filepath, sf::Vector2u spriteSize = { 0u, 0u });
    const sf::Texture& GetTexture(const std::string& id) const;
    sf::Vector2u GetSpriteSize(const std::string& id) const;
    bool Has(const std::string& id) const;

private:
    std::unordered_map<std::string, TextureData> m_textures;
};