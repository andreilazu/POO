#pragma once

#include <SFML/System/Vector2.hpp>
#include <string>

class WorldObject {
public:
    explicit WorldObject(sf::Vector2f position, std::string name);
    virtual ~WorldObject();

    virtual std::string describe() const;
    sf::Vector2f position() const;

protected:
    sf::Vector2f position_;
    std::string name_;
};
