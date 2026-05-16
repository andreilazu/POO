#include "game/PlayState.hpp"

#include "game/EntityInspector.hpp"
#include "game/Game.hpp"
#include "world/Biome.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Window/Keyboard.hpp>

#include <algorithm>
#include <cmath>

namespace {

bool keyDown(sf::Keyboard::Scancode scancode) {
    return sf::Keyboard::isKeyPressed(scancode);
}

}  // namespace

void PlayState::handleInput(Game& game, sf::Window& /*window*/) {
    sf::Vector2f direction{0.f, 0.f};
    if (keyDown(sf::Keyboard::Scancode::W) || keyDown(sf::Keyboard::Scancode::Up)) {
        direction.y -= 1.f;
    }
    if (keyDown(sf::Keyboard::Scancode::S) || keyDown(sf::Keyboard::Scancode::Down)) {
        direction.y += 1.f;
    }
    if (keyDown(sf::Keyboard::Scancode::A) || keyDown(sf::Keyboard::Scancode::Left)) {
        direction.x -= 1.f;
    }
    if (keyDown(sf::Keyboard::Scancode::D) || keyDown(sf::Keyboard::Scancode::Right)) {
        direction.x += 1.f;
    }

    if (direction.x != 0.f || direction.y != 0.f) {
        const float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        direction /= length;
    }
    moveDirection_ = direction;
}

void PlayState::update(Game& game, sf::Time deltaTime) {
    game.movePlayer(moveDirection_, deltaTime);
    game.player().update(deltaTime);
    game.updateDifficulty();
    game.updateWorld();
    game.world().updateEnemies(game.player().position(), deltaTime);
    game.world().processEnemyAttacks(game.player(), game.swarm(), deltaTime);
    game.swarm().update(deltaTime, game.player(), game.world());
    const int combatReward = game.world().processCombatWithSwarm(game.swarm());
    if (combatReward > 0 && !game.swarm().creatures().empty()) {
        const int strikerPower =
            EntityInspector::combatPowerFor(*game.swarm().creatures().front());
        game.events().publish("enemy.defeated",
                              "reward x" + std::to_string(combatReward) + " pwr:" +
                                  std::to_string(strikerPower));
    }
    game.swarm().removeDeadCreatures();
    game.processWorldInteractions();
    game.checkPlayerDeath();
}

void PlayState::render(Game& game, sf::RenderTarget& target) const {
    game.renderScene(target);

    const int swarmCount = static_cast<int>(game.swarm().creatures().size());
    const int nectar = game.player().nectarCollected();
    const int growCost = game.swarmGrowCost();
    const int health = game.player().health();
    const Biome biome = game.world().biomeAt(game.player().position());
    const BiomeProfile& biomeInfo = biomeProfile(biome);

    sf::RectangleShape panel({240.f, 78.f});
    panel.setPosition({12.f, 12.f});
    panel.setFillColor(sf::Color(0, 0, 0, 160));
    target.draw(panel);

    sf::RectangleShape healthBar({200.f * (static_cast<float>(health) / 120.f), 8.f});
    healthBar.setPosition({22.f, 22.f});
    healthBar.setFillColor(sf::Color(80, 200, 120));
    target.draw(healthBar);

    sf::RectangleShape swarmBar({static_cast<float>(std::min(swarmCount * 4, 200)), 8.f});
    swarmBar.setPosition({22.f, 36.f});
    swarmBar.setFillColor(sf::Color(220, 80, 90));
    target.draw(swarmBar);

    const float nectarFill =
        growCost > 0 ? static_cast<float>(std::min(nectar, growCost)) / static_cast<float>(growCost)
                     : 0.f;
    sf::RectangleShape nectarBar({200.f * nectarFill, 8.f});
    nectarBar.setPosition({22.f, 50.f});
    nectarBar.setFillColor(sf::Color(240, 200, 60));
    target.draw(nectarBar);

    sf::RectangleShape biomeTag({120.f, 16.f});
    biomeTag.setPosition({22.f, 66.f});
    biomeTag.setFillColor(biomeInfo.ground);
    biomeTag.setOutlineColor(biomeInfo.outline);
    biomeTag.setOutlineThickness(1.f);
    target.draw(biomeTag);
    (void)biomeInfo.name;
}
