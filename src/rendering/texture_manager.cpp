#include "rendering/texture_manager.hpp"
#include <stdexcept>

bool TextureManager::Load(const std::string& id, const std::string& filepath, sf::Vector2u spriteSize)
{
    if(Has(id))
        return true; // already loaded, skip

    TextureData data;
    if(!data.texture.loadFromFile(filepath))
        return false;

    data.spriteSize = spriteSize;
    m_textures.emplace(id, std::move(data));
    return true;
}

const sf::Texture& TextureManager::GetTexture(const std::string& id) const
{
    auto it = m_textures.find(id);
    if(it == m_textures.end())
        throw std::runtime_error("TextureManager: texture not found: " + id);

    return it->second.texture;
}

sf::Vector2u TextureManager::GetSpriteSize(const std::string& id) const
{
    auto it = m_textures.find(id);
    if(it == m_textures.end())
        throw std::runtime_error("TextureManager: texture not found: " + id);

    return it->second.spriteSize;
}

bool TextureManager::Has(const std::string& id) const
{
    return m_textures.find(id) != m_textures.end();
}