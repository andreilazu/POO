#pragma once

#include "entities/LivingEntity.hpp"

#include <SFML/Graphics/Color.hpp>
#include <string>

enum class CreatureType { Red, Blue, Yellow };

class Creature : public LivingEntity {
public:
    Creature();
    explicit Creature(CreatureType type);
    Creature(CreatureType type, sf::Color color, int power);

    Creature(const Creature& other);
    Creature& operator=(const Creature& other);
    ~Creature() override;

    void update(sf::Time deltaTime) override;
    void draw(sf::RenderTarget& target) const override;
    const char* typeName() const override;

    Creature& operator+=(const Creature& other);
    Creature& operator++();

    CreatureType creatureType() const;
    sf::Color color() const;
    int power() const;
    float visualRadius() const;
    float drawAlpha() const;
    float phaseOffset() const;
    float followLag() const;
    float moveSpeed() const;

    void setPower(int power);
    void setDrawAlpha(float alpha);
    void initSwarmTraits(std::size_t index);

private:
    CreatureType type_;
    sf::Color color_;
    int power_;
    std::string nickname_;
    float visualRadius_ = 6.f;
    float drawAlpha_ = 255.f;
    float phaseOffset_ = 0.f;
    float followLag_ = 0.f;
};

Creature operator+(Creature lhs, const Creature& rhs);
