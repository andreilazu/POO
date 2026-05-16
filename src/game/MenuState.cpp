#include "game/MenuState.hpp"

#include "game/Game.hpp"
#include "game/PlayState.hpp"

#include <SFML/Window/Keyboard.hpp>

#include <memory>

void MenuState::handleInput(Game& game, sf::Window& /*window*/) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Enter)) {
        game.setState(std::make_unique<PlayState>());
    }
}

void MenuState::update(Game& /*game*/, sf::Time /*deltaTime*/) {}

void MenuState::render(Game& /*game*/, sf::RenderTarget& /*target*/) const {}
