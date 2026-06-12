cmake_minimum_required(VERSION 3.10)
project(pvz C)

set(CMAKE_C_STANDARD 11)

include_directories(raylib/include)
link_directories(raylib/lib)

add_executable(pvz
        src/main.c
        src/ui_menu.c
        src/levels.c
        src/game.c
        src/grid.c
        src/plant.c
        src/ui_shop.c
        src/shop.c
        src/sun.c
        src/bullet.c
        src/zombie.c
        src/mower.c
        src/animation.c
        src/ui_menu.h
        src/levels.h
        src/game.h
        src/grid.h
        src/plant.h
        src/ui_shop.h
        src/shop.h
        src/sun.h
        src/bullet.h
        src/zombie.h
        src/mower.h
        src/animation.h
        src/sounds.h src/sounds.c)

target_link_libraries(pvz raylib opengl32 gdi32 winmm)
