#pragma once

#include "game/GameState.hpp"

class GameOverState : public GameState {
public:
    void handleInput(Game& game, sf::Window& window) override;
    void update(Game& game, sf::Time deltaTime) override;
    void render(Game& game, sf::RenderTarget& target) const override;
};
