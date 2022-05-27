cmake_minimum_required(VERSION 3.5)

project(ComicTranslator VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)

set(TS_FILES ComicTranslator_fr_FR.ts)

set(PROJECT_SOURCES
        main.cpp
        window/MainWindow.cpp
        window/MainWindow.h
        window/ViewerWindow.h
        window/ViewerWindow.cpp
        page/home/HomePage.cpp
        page/home/HomePage.h
        ui/home_page.ui
        include/environment.h
        include/models.h
        page/editor/EditorPage.h page/editor/EditorPage.cpp
        ${TS_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(ComicTranslator
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(ComicTranslator SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(ComicTranslator
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_link_libraries(ComicTranslator PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(ComicTranslator PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(ComicTranslator)
endif()
