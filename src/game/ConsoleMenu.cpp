#include "game/ConsoleMenu.hpp"

#include "game/AcademicDemo.hpp"
#include "game/Game.hpp"

#include <iostream>
#include <limits>
#include <string>

void ConsoleMenu::run() const {
    while (true) {
        std::cout << "\n=== Pikmin Swarm ===\n"
                  << "1. Joaca (fereastra SFML)\n"
                  << "2. Demo cerinte academice\n"
                  << "0. Iesire\n"
                  << "Alegere: ";

        std::string line;
        if (!std::getline(std::cin, line)) {
            break;
        }

        if (line == "1") {
            Game game;
            game.run();
        } else if (line == "2") {
            AcademicDemo::run();
        } else if (line == "0") {
            break;
        } else {
            std::cout << "Optiune invalida.\n";
        }
    }
}
