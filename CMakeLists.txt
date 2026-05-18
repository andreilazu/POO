cmake_minimum_required(VERSION 3.16)

project(pikmin_swarm VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(cmake/Options.cmake)
include(cmake/CompilerFlags.cmake)
include(cmake/CopyHelper.cmake)

find_package(SFML 3 REQUIRED COMPONENTS Graphics Window System)

set(PIKMIN_SOURCES
    src/main.cpp
    src/core/GameException.cpp
    src/core/InvalidSpawnException.cpp
    src/core/ResourceLoadException.cpp
    src/entities/Entity.cpp
    src/entities/LivingEntity.cpp
    src/entities/Player.cpp
    src/entities/Creature.cpp
    src/entities/Enemy.cpp
    src/io/CreatureIO.cpp
    src/world/WorldObject.cpp
    src/world/Collectible.cpp
    src/world/Obstacle.cpp
    src/world/WorldChunk.cpp
    src/world/Biome.cpp
    src/world/ProceduralGenerator.cpp
    src/swarm/SwarmManager.cpp
    src/swarm/CombatSystem.cpp
    src/game/GameState.cpp
    src/game/PlayState.cpp
    src/game/GameOverState.cpp
    src/game/PauseState.cpp
    src/game/MenuState.cpp
    src/game/EntityFactory.cpp
    src/game/EntityInspector.cpp
    src/game/EventBus.cpp
    src/game/Game.cpp
    src/game/ConsoleMenu.cpp
    src/game/AcademicDemo.cpp
)

add_executable(pikmin_swarm ${PIKMIN_SOURCES})

target_include_directories(pikmin_swarm PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(pikmin_swarm PRIVATE SFML::Graphics SFML::Window SFML::System)

set_compiler_flags(RUN_SANITIZERS TRUE TARGET_NAMES pikmin_swarm)

if(WIN32)
    set_property(TARGET pikmin_swarm PROPERTY WIN32_EXECUTABLE FALSE)
    set(VCPKG_APPLOCAL_DEPS ON)
endif()

set_property(TARGET pikmin_swarm PROPERTY VS_DEBUGGER_WORKING_DIRECTORY
    "${CMAKE_SOURCE_DIR}")

install(TARGETS pikmin_swarm DESTINATION ${DESTINATION_DIR})

enable_testing()
add_test(NAME academic_demo COMMAND pikmin_swarm --demo)
