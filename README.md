
<pre>
# Pikmin Swarm

Joc 2D single-player inspirat de managementul unui roi de creaturi (Pikmin-like), implementat în **C++17** cu **SFML 3**. Playerul controlează un lider, iar creaturile pot fi comandate să adune resurse, să lupte cu inamici și să exploreze o lume generată procedural.

## Instrucțiuni de compilare

Proiectul este configurat cu CMake.

Instrucțiuni pentru terminal:

**Pasul de configurare**
```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
# sau ./scripts/cmake.sh configure
```

Sau pe Windows cu GCC:
```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -G Ninja
# sau ./scripts/cmake.sh configure -g Ninja
```
La acest pas putem cere să generăm fișiere de proiect pentru diverse medii de lucru.

*Configurare opțională folosind vcpkg (pentru instalarea SFML):*
```sh
export VCPKG_ROOT=/path/to/vcpkg # Linux/macOS
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE="$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake"
```

**Pasul de compilare**
```sh
cmake --build build --config Debug --parallel 6
# sau ./scripts/cmake.sh build
```

**Rularea executabilului și controale**
- Din directorul de build: `./build/pikmin_swarm`
- Pentru modul demo (fără meniu, folosit în teste automate/CI): `./build/pikmin_swarm --demo`
- Pentru intrări automate: redirecționați un fișier de text (ex. `./build/pikmin_swarm < tastatura.txt`)
- Mișcarea jucătorului se face folosind săgețile sau tastele **WASD**. Apasă **Esc** pentru a închide fereastra.

---

## Cerințe obligatorii
Nerespectarea duce la nepunctarea proiectului

- [x] programul va fi scris în C++
- [x] programul va avea un meniu interactiv (doar pentru ilustrarea funcționalității) - *Implementat prin clasa `ConsoleMenu`*
- [x] programul nu are erori de compilare
- [x] fară variabile globale
- [x] datele membre private (sau protected)
- [x] GitHub Actions trecute
- [x] commit-uri pe Git adecvate si punctuale
- [x] (evitat) folosirea a funcționalităților limbajului fără sens
- [x] (evitat) folosirea a funcționlităților limbajului cu scopul de a încălca "legal" o altă regulă
- [x] (evitat) folosirea excesivă a claselor friend
- [x] (evitat) folosirea excesviă a elementelor statice
- [x] (evitat) lipsa separarea implementarii de definitie - *S-a respectat cu strictețe separarea `include/` și `src/`*

## Cerințe
pentru fiecare cerință (sau subcerință) neîndeplinită se scade 1 punct

- [x] definirea a minim 2-3 ieararhii de clase care sa interactioneze in cadrul temei alese (fie prin compunere, agregare sau doar sa apeleze metodele celeilalte intr-un mod logic)
- *`Entity` → `LivingEntity` → `Player` / `Creature` / `Enemy`*
- *`WorldObject` → `Collectible` / `Obstacle`*
- *Ierarhia excepțiilor: `GameException` → `InvalidSpawnException` / `ResourceLoadException`*
- [x] minim o clasa cu (*Ex. clasa `Creature`*):
- [x] constructori de inițializare *
- [x] constructor supraîncărcat *
- [x] constructori de copiere *
- [x] operator= de copiere *
- [x] destructor *
- [x] operator<< pentru afișare (std::ostream) *
- [x] operator>> pentru citire (std::istream) *
- [x] alt operator supraîncărcat ca funcție membră * (ex. `+`, `++`)
- [x] alt operator supraîncărcat ca funcție non-membră * - nu neaparat ca friend
- [x] in derivate
- [x] implementarea funcționalităților alese prin upcast și downcast (în `EntityInspector`, `AcademicDemo` și logică de gameplay)
- [x] aceasta va fi făcută prin 2-3 metode specifice temei alese
- [x] funcțiile pentru citire / afișare sau destructorul nu sunt incluse deși o să trebuiască să le implementați
- [x] apelarea constructorului din clasa de bază din constructori din derivate
- [x] suprascris cc/op= pentru copieri/atribuiri corecte
- [x] destructor virtual
- [x] pentru celelalte clase se va definii doar ce e nevoie
- [x] minim o ierarhie mai dezvoltata (cu 2-3 clase dintr-o clasa de baza) - *ierarhia extinsă `Entity`*
- [x] ierarhie de clasa se considera si daca exista doar o clasa de bază însă care nu moștenește dintr-o clasă din altă ierarhie
- [x] cât mai multe const *
- [x] funcții și atribute static (în clase) *
- [x] 1+ atribute statice non-triviale
- [x] 1+ funcții statice non-triviale
- [x] excepții * (*ex: `InvalidSpawnException`, `ResourceLoadException`*)
- [x] porniți de la std::exception
- [x] ilustrați propagarea excepțiilor
- [x] ilustrati upcasting-ul în blocurile catch
- [x] minim folosit într-un loc în care tratarea erorilor în modurile clasice este mai dificilă
- [x] folosirea unei clase abstracte *
- [x] clase template (*ex: `ObjectPool<T>`*)
- [x] crearea unei clase template *
- [x] 2 instanțieri ale acestei clase (*pentru `Creature` și `Collectible`*)
- [x] STL *
- [x] utilizarea a două structuri (containere) diferite (`std::vector`, etc.)
- [x] utilizarea a unui algoritm cu funcție lambda (`std::sort` / `std::for_each` folosite în modul demo)
- [x] Design Patterns *
- [x] utilizarea a două șabloane de proiectare (ex: **Factory** prin `EntityFactory` și **Observer** prin `EventBus`)

## Observații

Pot exista depunctări până la 2p pentru diferite aspecte precum:
- memory leak-uri
- nefolosirea destructorului virtual la nevoie
- abuzarea de diferite concepte (toate funcțiile declarate virtual)
- apelarea de funcții virtual în constructori

În general, acestea sunt prezente în CppCoreGuideline, dar nu e nevoie să parcurgeți documentul, doar să scrieți codul suficient de organizat

folderele build/ și install_dir/ sunt adăugate în fișierul .gitignore deoarece conțin fișiere generate și nu ne ajută să le versionăm.
</pre>
