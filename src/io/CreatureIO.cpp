#include "io/CreatureIO.hpp"

#include <sstream>

std::ostream& operator<<(std::ostream& out, const Creature& creature) {
    const char* typeLabel = "unknown";
    switch (creature.creatureType()) {
    case CreatureType::Red:
        typeLabel = "red";
        break;
    case CreatureType::Blue:
        typeLabel = "blue";
        break;
    case CreatureType::Yellow:
        typeLabel = "yellow";
        break;
    }
    out << "Creature{type=" << typeLabel << ",power=" << creature.power()
        << ",pos=(" << creature.position().x << "," << creature.position().y << ")}";
    return out;
}

std::istream& operator>>(std::istream& in, Creature& creature) {
    std::string typeToken;
    int power = 1;
    float x = 0.f;
    float y = 0.f;
    char comma = '\0';

    if (!(in >> typeToken >> comma >> power >> comma >> x >> comma >> y)) {
        in.setstate(std::ios::failbit);
        return in;
    }

    CreatureType type = CreatureType::Red;
    if (typeToken == "blue") {
        type = CreatureType::Blue;
    } else if (typeToken == "yellow") {
        type = CreatureType::Yellow;
    }

    creature = Creature(type);
    creature.setPower(power);
    creature.setPosition({x, y});
    return in;
}
