#include "entities/Creature.hpp"

#include <SFML/Graphics/CircleShape.hpp>

#include <algorithm>

namespace {

sf::Color defaultColor(CreatureType type) {
    switch (type) {
    case CreatureType::Red:
        return sf::Color::Red;
    case CreatureType::Blue:
        return sf::Color::Blue;
    case CreatureType::Yellow:
        return sf::Color::Yellow;
    }
    return sf::Color::White;
}

}  // namespace

Creature::Creature()
    : LivingEntity({0.f, 0.f}, 1, 80.f),
      type_(CreatureType::Red),
      color_(defaultColor(CreatureType::Red)),
      power_(1),
      nickname_("sprout") {}

Creature::Creature(CreatureType type)
    : LivingEntity({0.f, 0.f}, 1, 80.f),
      type_(type),
      color_(defaultColor(type)),
      power_(1),
      nickname_("sprout") {
    switch (type) {
    case CreatureType::Red:
        power_ = 2;
        speed_ = 95.f;
        health_ = 2;
        break;
    case CreatureType::Yellow:
        power_ = 1;
        speed_ = 130.f;
        health_ = 1;
        break;
    case CreatureType::Blue:
        power_ = 1;
        speed_ = 75.f;
        health_ = 4;
        break;
    }
}

Creature::Creature(CreatureType type, sf::Color color, int power)
    : LivingEntity({0.f, 0.f}, 1, 80.f),
      type_(type),
      color_(color),
      power_(power),
      nickname_("sprout") {}

Creature::Creature(const Creature& other)
    : LivingEntity(other.position(), other.health(), other.speed()),
      type_(other.type_),
      color_(other.color_),
      power_(other.power_),
      nickname_(other.nickname_),
      visualRadius_(other.visualRadius_),
      drawAlpha_(other.drawAlpha_),
      phaseOffset_(other.phaseOffset_),
      followLag_(other.followLag_) {}

Creature& Creature::operator=(const Creature& other) {
    if (this != &other) {
        position_ = other.position_;
        health_ = other.health_;
        speed_ = other.speed_;
        type_ = other.type_;
        color_ = other.color_;
        power_ = other.power_;
        nickname_ = other.nickname_;
        visualRadius_ = other.visualRadius_;
        drawAlpha_ = other.drawAlpha_;
        phaseOffset_ = other.phaseOffset_;
        followLag_ = other.followLag_;
    }
    return *this;
}

Creature::~Creature() = default;

void Creature::update(sf::Time /*deltaTime*/) {}

void Creature::draw(sf::RenderTarget& target) const {
    sf::CircleShape shape(visualRadius_);
    sf::Color fill = color_;
    fill.a = static_cast<std::uint8_t>(drawAlpha_);
    shape.setFillColor(fill);
    shape.setPosition({position_.x - visualRadius_, position_.y - visualRadius_});
    target.draw(shape);
}

const char* Creature::typeName() const {
    return "Creature";
}

Creature& Creature::operator+=(const Creature& other) {
    power_ += other.power_;
    return *this;
}

Creature& Creature::operator++() {
    ++power_;
    return *this;
}

CreatureType Creature::creatureType() const {
    return type_;
}

sf::Color Creature::color() const {
    return color_;
}

int Creature::power() const {
    return power_;
}

void Creature::setPower(int power) {
    power_ = power;
}

float Creature::visualRadius() const {
    return visualRadius_;
}

float Creature::drawAlpha() const {
    return drawAlpha_;
}

float Creature::phaseOffset() const {
    return phaseOffset_;
}

float Creature::followLag() const {
    return followLag_;
}

float Creature::moveSpeed() const {
    return speed_;
}

void Creature::setDrawAlpha(float alpha) {
    drawAlpha_ = std::clamp(alpha, 40.f, 255.f);
}

void Creature::initSwarmTraits(std::size_t index) {
    visualRadius_ = 4.5f + static_cast<float>((index * 7U) % 5U) * 0.55f;
    phaseOffset_ = static_cast<float>((index * 17U) % 360U) * (3.14159265f / 180.f);
    followLag_ = static_cast<float>((index * 13U) % 10U) * 0.04f;
}

Creature operator+(Creature lhs, const Creature& rhs) {
    lhs += rhs;
    return lhs;
}
