#include "rendering/renderer.hpp"
#include <iostream>

/// CONSTRUCTOR
Renderer::Renderer(sf::RenderWindow& window, TextureManager& textures)
    : m_window(window), m_textures(textures)
{
    UpdateViews(window.getSize());
}


/// RENDERS WHOLE APPLICATION / GAME
void Renderer::Render(GameState& state)
{
    std::optional<Square>& selected = state.GetSelectedSquare();
    std::vector<Square> legalMoves = state.GetCachedLegalMoves();

    m_window.clear(sf::Color::Black);

    DrawBoard();
    DrawHighlights(legalMoves);
    DrawSelectedSquare(selected);
    DrawPieces(state.GetBoard());
    DrawHUD(state);

    m_window.display();
}


/// UPDATES VIEW BASED ON CURRENT WINDOW SIZE (NEEDED WHEN RESIZING)
void Renderer::UpdateViews(sf::Vector2u windowSize)
{
    sf::Vector2f window_size = static_cast<sf::Vector2f>(windowSize);
    float scale = std::min(
        window_size.x / VIRTUAL_SIZE,
        window_size.y / VIRTUAL_SIZE
    );

    float scaledSize = VIRTUAL_SIZE * scale;
    float offsetX = (window_size.x - scaledSize) / 2.f;
    float offsetY = (window_size.y - scaledSize) / 2.f;

    sf::FloatRect viewport = {
        { offsetX / window_size.x, offsetY / window_size.y },
        { scaledSize / window_size.x, scaledSize / window_size.y }
    };

    m_view.setSize({ VIRTUAL_SIZE, VIRTUAL_SIZE });
    m_view.setCenter({ VIRTUAL_SIZE / 2.f, VIRTUAL_SIZE / 2.f });
    m_view.setViewport(viewport);
    m_window.setView(m_view);
}


/// CONVERTS MOUSE SCREEN COORDINATES TO SQUARE INDEX
Square Renderer::ScreenToSquare(sf::Vector2i mousePos) const
{
    sf::Vector2f world = ScreenToWorld(mousePos);

    // check if click is outside board area 
    if(world.x < PADDING ||
       world.y < PADDING ||
       world.x >= PADDING + BOARD_SIZE ||
       world.y >= PADDING + BOARD_SIZE
       )
        return { -1, -1 };

    int col = static_cast<int>((world.x - PADDING) / TILE_SIZE);
    int row = static_cast<int>((world.y - PADDING) / TILE_SIZE);

    return { row, col };
}


/// CONVERTS MOUSE SCREEN COORDINATES TO WORLD(VIEW) COORDINATES
sf::Vector2f Renderer::ScreenToWorld(sf::Vector2i mousePos) const
{
    sf::Vector2f world = m_window.mapPixelToCoords(mousePos, m_view);
    return world;
}


/// DRAWS THE BOARD (RECTANGLES)
void Renderer::DrawBoard()
{
    for(int row = 0; row < 8; row++)
    {
        for(int col = 0; col < 8; col++)
        {
            sf::RectangleShape tile(sf::Vector2f(TILE_SIZE, TILE_SIZE));
            tile.setPosition(sf::Vector2f(
                PADDING + col * TILE_SIZE,
                PADDING + row * TILE_SIZE
            ));
            tile.setFillColor(GetTileColor({ row, col }));
            m_window.draw(tile);
        }
    }
}


/// DRAWS THE PIECES (SPRITES, ORIGIN AT CENTER OF SPRITE)
void Renderer::DrawPieces(const Board& board)
{
    for(int row = 0; row < 8; row++)
    {
        for(int col = 0; col < 8; col++)
        {
            Piece piece = board.GetPiece({ row, col });
            if(piece.IsEmpty())
                continue;

            sf::Sprite sprite(m_textures.GetTexture("pieces"));
            sprite.setTextureRect(GetPieceTextureRect(piece));

            sf::Vector2u spriteSize = m_textures.GetSpriteSize("pieces");
            float scaleX = TILE_SIZE / spriteSize.x * SCALE_FACTOR;
            float scaleY = TILE_SIZE / spriteSize.y * SCALE_FACTOR;
            sprite.setScale(sf::Vector2f(scaleX, scaleY));
            sprite.setOrigin({ spriteSize.x * 0.5f, spriteSize.y * 0.5f });
            sprite.setPosition({ PADDING + TILE_SIZE * 0.5f + col * TILE_SIZE ,
                               PADDING + TILE_SIZE * 0.5f + row * TILE_SIZE });

            m_window.draw(sprite);
        }
    }
}


/// DRAW HIGHLIGHTS TO THE GIVEN SQUARES (GENERALLY FOR LEGAL MOVES)
void Renderer::DrawHighlights(const std::vector<Square>& squares)
{
    for(const Square& sq : squares)
    {
        // small semi-transparent circle centered on the tile
        float radius = TILE_SIZE * 0.20f;
        sf::CircleShape dot(radius);
        dot.setFillColor(HIGHLIGHT_COLOR);

        sf::FloatRect bounds = GetSquareBounds(sq);
        dot.setPosition(sf::Vector2f(
            bounds.position.x + (TILE_SIZE / 2.f) - radius,
            bounds.position.y + (TILE_SIZE / 2.f) - radius
        ));

        m_window.draw(dot);
    }
}


/// DRAWS THE SELECTED SQUARE (I.E HIGHLIGHTS WITH A COLOR)
void Renderer::DrawSelectedSquare(std::optional<Square> selected)
{
    if(!selected.has_value())
        return;

    sf::RectangleShape highlight(sf::Vector2f(TILE_SIZE, TILE_SIZE));
    highlight.setPosition(GetSquareBounds(*selected).position);
    highlight.setFillColor(SELECT_COLOR); // yellow tint

    m_window.draw(highlight);
}


/// DRAWS ALL THE HUD COMPONENTS
void Renderer::DrawHUD(const GameState& state)
{
    DrawTopBar(state);
    DrawBottomBar(state);
    DrawSidePanels(state);
}


/// DRAW TOP BAR ALONG WITH WHAT IT CONTAINS
void Renderer::DrawTopBar(const GameState& state)
{
    // top bar region: x: 0–VIRTUAL_SIZE, y: 0–PADDING
    // placeholder background (WILL BE REPLACED BY SOMETHING)
    sf::RectangleShape bar(sf::Vector2f(VIRTUAL_SIZE, PADDING));
    bar.setPosition(sf::Vector2f(0.f, 0.f));
    bar.setFillColor(sf::Color(30, 30, 30));
    m_window.draw(bar);
}


/// DRAW BOTTOM BAR ALONG WITH WHAT IT CONTAINS
void Renderer::DrawBottomBar(const GameState& state)
{
    // bottom bar region: x: 0–VIRTUAL_SIZE, y: PADDING+BOARD_SIZE–VIRTUAL_SIZE
    // placeholder background (WILL BE REPLACED BY SOMETHING)
    sf::RectangleShape bar(sf::Vector2f(VIRTUAL_SIZE, PADDING));
    bar.setPosition(sf::Vector2f(0.f, PADDING + BOARD_SIZE));
    bar.setFillColor(sf::Color(30, 30, 30));
    m_window.draw(bar);
}


/// DRAW SIDE (LEFT AND RIGHT) PANELS
void Renderer::DrawSidePanels(const GameState& state)
{
    // left panel: x: 0–PADDING, y: PADDING–PADDING+BOARD_SIZE
    // placeholder background (WILL BE REPLACED BY SOMETHING)
    sf::RectangleShape left(sf::Vector2f(PADDING, BOARD_SIZE));
    left.setPosition(sf::Vector2f(0.f, PADDING));
    left.setFillColor(sf::Color(40, 40, 40));
    m_window.draw(left);

    // right panel: x: PADDING+BOARD_SIZE–VIRTUAL_SIZE, y: PADDING–PADDING+BOARD_SIZE
    // placeholder background (WILL BE REPLACED BY SOMETHING)
    sf::RectangleShape right(sf::Vector2f(PADDING, BOARD_SIZE));
    right.setPosition(sf::Vector2f(PADDING + BOARD_SIZE, PADDING));
    right.setFillColor(sf::Color(40, 40, 40));
    m_window.draw(right);
}


/// GET THE COLOR OF THE TILE DEPENDING ON THE SQUARE
sf::Color Renderer::GetTileColor(Square sq) const
{
    bool isLight = (sq.row + sq.col) % 2 == 0;
    return isLight ? LIGHT_COLOR : DARK_COLOR;
}


/// GET THE BOUNDING BOX (POSITION, SIZE) OF THE SQUARE
sf::FloatRect Renderer::GetSquareBounds(Square sq) const
{
    return {
        { PADDING + sq.col * TILE_SIZE, PADDING + sq.row * TILE_SIZE },
        { TILE_SIZE, TILE_SIZE }
    };
}


/// GET THE BOUNDING BOX (POSITION, SIZE) OF SPRITE WITHIN THE TEXTURE (SPECIFICALLY FOR A PIECE)
sf::IntRect Renderer::GetPieceTextureRect(Piece piece) const
{
    sf::Vector2u spriteSize = m_textures.GetSpriteSize("pieces");
    int col = static_cast<int>(piece.type) - 1;
    int row = (piece.color == Color::WHITE) ? 1 : 0;

    return {
        { col * static_cast<int>(spriteSize.x), row * static_cast<int>(spriteSize.y) },
        { static_cast<int>(spriteSize.x), static_cast<int>(spriteSize.y) }
    };
}