#pragma once

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>

class Entity {
public:
    explicit Entity(sf::Vector2f position);
    virtual ~Entity();

    Entity(const Entity&) = delete;
    Entity& operator=(const Entity&) = delete;
    Entity(Entity&&) noexcept = default;
    Entity& operator=(Entity&&) noexcept = default;

    virtual void update(sf::Time deltaTime) = 0;
    virtual void draw(sf::RenderTarget& target) const = 0;
    virtual const char* typeName() const = 0;

    sf::Vector2f position() const;
    void setPosition(sf::Vector2f position);

protected:
    sf::Vector2f position_;
};
