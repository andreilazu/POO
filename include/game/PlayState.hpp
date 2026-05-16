#pragma once

#include "game/GameState.hpp"

#include <SFML/System/Vector2.hpp>

class PlayState : public GameState {
public:
    void handleInput(Game& game, sf::Window& window) override;
    void update(Game& game, sf::Time deltaTime) override;
    void render(Game& game, sf::RenderTarget& target) const override;

private:
    sf::Vector2f moveDirection_{0.f, 0.f};
};
