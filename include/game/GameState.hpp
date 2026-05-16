#pragma once

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Window/Window.hpp>

class Game;

class GameState {
public:
    virtual ~GameState() = default;
    virtual void handleInput(Game& game, sf::Window& window) = 0;
    virtual void update(Game& game, sf::Time deltaTime) = 0;
    virtual void render(Game& game, sf::RenderTarget& target) const = 0;
};
