#pragma once

#include "entities/Creature.hpp"

#include <iosfwd>

std::ostream& operator<<(std::ostream& out, const Creature& creature);
std::istream& operator>>(std::istream& in, Creature& creature);
