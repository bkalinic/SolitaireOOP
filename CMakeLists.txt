cmake_minimum_required (VERSION 3.22)

project(ftxui-starter
  LANGUAGES CXX
  VERSION 1.0.0
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# --- Fetch FTXUI --------------------------------------------------------------
include(FetchContent)

set(FETCHCONTENT_UPDATES_DISCONNECTED ON)
set(FETCHCONTENT_QUIET OFF)

FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/arthursonzogni/ftxui.git
  GIT_TAG        v5.0.0
  GIT_PROGRESS   TRUE
  GIT_SHALLOW    TRUE
  EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(ftxui)
# ------------------------------------------------------------------------------
set(OOP_SOURCE
    src/cpp/Card.cpp
    src/cpp/Deck.cpp
    src/cpp/FoundationPile.cpp
    src/cpp/GameEngine.cpp
    src/cpp/PileVirt.cpp
    src/cpp/TablePile.cpp
    src/cpp/WastePile.cpp
)
set(OOP_HEADERS
	src/header/Card.h
    src/header/Deck.h
    src/header/FoundationPile.h
    src/header/GameEngine.h
    src/header/PileVirt.h
    src/header/TablePile.h
    src/header/WastePile.h
)

add_executable (SolitaireFixed 
    "SolitaireFixed.cpp" "SolitaireFixed.h"
    ${OOP_SOURCE}
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET SolitaireFixed PROPERTY CXX_STANDARD 20)
endif()

target_include_directories(SolitaireFixed PRIVATE src)

target_link_libraries(SolitaireFixed
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component
)
