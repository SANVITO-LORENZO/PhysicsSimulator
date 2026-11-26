cmake_minimum_required(VERSION 3.14)
project(PhysicsSimulator CXX)

# Voglio usare le feature moderne del C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Risolve il problema delle DLL mancanti su Windows (le ingloba direttamente nell'exe)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libs" FORCE)

# Scarica SFML in automatico da GitHub tramite ZIP (il comando tramite Git dava problemi)
include(FetchContent)
FetchContent_Declare(SFML
    URL https://github.com/SFML/SFML/archive/refs/tags/2.6.1.zip
)
FetchContent_MakeAvailable(SFML)

# Creo il mio eseguibile dal main
add_executable(PhysicsSimulator src/main.cpp)

# Collego le librerie di SFML necessarie per rendering e finestre
target_link_libraries(PhysicsSimulator PRIVATE sfml-graphics sfml-window sfml-system)