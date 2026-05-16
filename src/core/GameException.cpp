#include "core/GameException.hpp"

GameException::GameException(std::string message) : message_(std::move(message)) {}

const char* GameException::what() const noexcept {
    return message_.c_str();
}
