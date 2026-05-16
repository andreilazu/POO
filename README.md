# Pikmin Swarm

Joc 2D single-player inspirat de managementul unui roi de creaturi (Pikmin-like), implementat în **C++17** cu **SFML 3**. Playerul controlează un lider, iar creaturile pot fi comandate să adune resurse, să lupte cu inamici și să exploreze o lume generată procedural.

### Folosiți template-ul corespunzător grupei voastre!

| Laborant | Link template |
|----------|----------------------------------------------|
| Dragoș B | https://github.com/Ionnier/oop-template |
| Tiberiu M | https://github.com/MaximTiberiu/oop-template |
| Marius MC | https://github.com/mcmarius/oop-template |

## Tema 0

- [x] **Nume proiect:** Pikmin Swarm
- [x] **Descriere:** joc de tip swarm management cu meniu interactiv în consolă, entități vii (player, creaturi, inamici), obiecte de lume (colectibile, obstacole), generare procedurală de chunk-uri, sistem de combat și demo academic (`--demo`) pentru cerințele POO.

## Cerințe POO (rezumat implementare)

| Cerință | Implementare în proiect |
|---------|-------------------------|
| Meniu interactiv | `ConsoleMenu` — Joacă / Demo cerințe / Ieșire |
| Ierarhii de clase | `Entity` → `LivingEntity` → `Player`/`Creature`/`Enemy`; `WorldObject` → `Collectible`/`Obstacle`; excepții `GameException` → … |
| Operatori, ctor copiere, virtual | `Creature` (<<, >>, +, ++, cc/op=/dtor) |
| Upcast / downcast | `EntityInspector`, `AcademicDemo`, gameplay |
| Excepții + catch pe bază | `InvalidSpawnException`, `ResourceLoadException` |
| Clase template | `ObjectPool<T>` (Creature, Collectible) |
| STL + lambda | `std::vector`, `std::sort` / `std::for_each` în demo |
| Design patterns | **Factory** (`EntityFactory`), **Observer** (`EventBus`) |
| Fără variabile globale | da |
| Separare .h / .cpp | `include/` + `src/` |

## Instrucțiuni de compilare

Proiectul este configurat cu **CMake** (manifest [vcpkg](https://vcpkg.io/) pentru SFML 3).

### Pasul de configurare

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
# sau ./scripts/cmake.sh configure
```

Sau pe Windows cu GCC (Git Bash) / Ninja:

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -G Ninja
# sau ./scripts/cmake.sh configure -g Ninja
```

Cu vcpkg (recomandat dacă SFML nu e în sistem):

```sh
export VCPKG_ROOT=/path/to/vcpkg   # Linux/macOS
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_TOOLCHAIN_FILE="$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake"
```

### Pasul de compilare

```sh
cmake --build build --config Debug --parallel 6
# sau ./scripts/cmake.sh build
```

### Visual Studio 2022 (Windows)

```powershell
$env:VCPKG_ROOT = "C:\path\to\vcpkg"
cmake --preset vs2022-debug
cmake --build --preset vs2022-debug
.\build-vs\Debug\pikmin_swarm.exe
```

Folderele `build/`, `build-vs/` și `install_dir/` sunt în `.gitignore` (fișiere generate).

## Instrucțiuni pentru a rula executabilul

1. Din directorul de build:

```sh
./build/pikmin_swarm
```

2. Demo academic (fără meniu, folosit în CI):

```sh
./build/pikmin_swarm --demo
```

3. Intrări de la tastatură pentru teste automate: fișierul `tastatura.txt` (ex.: `2` = demo, `0` = ieșire).

### Controale (mod Joacă)

- WASD / săgeți: mișcare player
- Esc: închide fereastra

## Resurse

- [SFML 3 documentation](https://www.sfml-dev.org/documentation/3.0.0/)
- [CMake](https://cmake.org/documentation/)
- [vcpkg](https://vcpkg.io/)
- Template POO: https://github.com/MaximTiberiu/oop-template (structură README / CI)

## License

Proiectul folosește licența din fișierul [LICENSE](LICENSE) (AGPLv3, conform template-ului laborantului).
