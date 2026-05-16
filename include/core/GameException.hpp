#pragma once

#include <exception>
#include <string>

class GameException : public std::exception {
public:
    explicit GameException(std::string message);
    const char* what() const noexcept override;

private:
    std::string message_;
};
