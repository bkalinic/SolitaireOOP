cmake_minimum_required(VERSION 3.10)
project(SolitaireOOP LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_PREFIX_PATH "C:/Qt/6.10.1/msvc2022_64/")
find_package(Qt6 CONFIG REQUIRED COMPONENTS Core Widgets Gui)

set(OOP_SOURCES
    src/cpp/card.cpp
    src/cpp/deck.cpp
    src/cpp/pile.cpp
    src/cpp/tablepile.cpp
    src/cpp/foundationpile.cpp  
    src/cpp/stockpile.cpp
    src/cpp/wastepile.cpp
    src/cpp/command.cpp
    src/cpp/gameengine.cpp
)
set(OOP_HEADERS
    src/header/card.h
    src/header/types.h
    src/header/deck.h
    src/header/pile.h
    src/header/tablepile.h
    src/header/foundationpile.h
    src/header/stockpile.h
    src/header/wastepile.h
    src/header/command.h
    src/header/gameengine.h
)

add_executable(SolitaireOOP
    main.cpp
    ${OOP_SOURCES}

    src/gui/MainWindow.cpp
    src/gui/CardItem.cpp
    src/gui/PileItem.cpp
    src/gui/GameManager.cpp

    src/guiHeader/MainWindow.h
    src/guiHeader/CardItem.h
    src/guiHeader/PileItem.h
    src/guiHeader/GameManager.h

    resources/cards.qrc
)

target_include_directories(SolitaireOOP PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/header
    ${CMAKE_CURRENT_SOURCE_DIR}/src/guiHeader
)

target_link_libraries(SolitaireOOP PRIVATE
    Qt6::Core    
    Qt6::Widgets
    Qt6::Gui
)