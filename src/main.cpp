#include "game/AcademicDemo.hpp"
#include "game/ConsoleMenu.hpp"

#include <string>

int main(int argc, const char* argv[]) {
    for (int i = 1; i < argc; ++i) {
        const std::string arg = argv[i];
        if (arg == "--demo") {
            AcademicDemo::run();
            return 0;
        }
    }

    ConsoleMenu menu;
    menu.run();
    return 0;
}
