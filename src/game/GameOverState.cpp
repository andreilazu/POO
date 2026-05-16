#include "game/GameOverState.hpp"

#include "game/Game.hpp"

#include <SFML/Window/Keyboard.hpp>

void GameOverState::handleInput(Game& game, sf::Window& /*window*/) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::R)) {
        game.restartRun();
    }
}

void GameOverState::update(Game& /*game*/, sf::Time /*deltaTime*/) {}

void GameOverState::render(Game& game, sf::RenderTarget& target) const {
    game.renderScene(target);

    sf::RectangleShape overlay({960.f, 540.f});
    overlay.setFillColor(sf::Color(0, 0, 0, 150));
    target.draw(overlay);

    sf::RectangleShape panel({380.f, 100.f});
    panel.setPosition({290.f, 220.f});
    panel.setFillColor(sf::Color(50, 24, 24, 230));
    panel.setOutlineColor(sf::Color(220, 90, 90));
    panel.setOutlineThickness(2.f);
    target.draw(panel);

    sf::RectangleShape hint({140.f, 14.f});
    hint.setPosition({410.f, 280.f});
    hint.setFillColor(sf::Color(200, 200, 200));
    target.draw(hint);
}
