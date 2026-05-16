#include "game/PauseState.hpp"

#include "game/Game.hpp"
#include "game/PlayState.hpp"

#include <SFML/Graphics/View.hpp>
#include <SFML/Window/Keyboard.hpp>

#include <memory>

void PauseState::handleInput(Game& game, sf::Window& window) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::P)) {
        game.setState(std::make_unique<PlayState>());
    }
    (void)window;
}

void PauseState::update(Game& /*game*/, sf::Time /*deltaTime*/) {}

void PauseState::render(Game& game, sf::RenderTarget& target) const {
    const sf::Vector2f center = game.player().position();
    sf::View worldView;
    worldView.setSize({960.f, 540.f});
    worldView.setCenter(center);
    target.setView(worldView);

    game.renderWorld(target);
    game.player().draw(target);
    for (const auto& creature : game.swarm().creatures()) {
        creature->draw(target);
    }

    target.setView(target.getDefaultView());
}
