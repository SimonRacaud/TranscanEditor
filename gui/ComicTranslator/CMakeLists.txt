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
        asset.qrc
        window/MainWindow.cpp
        window/MainWindow.h
        window/ViewerWindow.h
        window/ViewerWindow.cpp
        ui/home_page.ui
        include/environment.h
        include/models.h
        include/model/OCRPage.h
        include/model/Utils.h
        page/editor/EditorView.h page/editor/EditorView.cpp
        page/editor/EditorController.h page/editor/EditorController.cpp
        page/home/HomePage.cpp
        page/home/HomePage.h
        widget/editor/propertyTab/APropertyTab.h widget/editor/propertyTab/APropertyTab.cpp
        widget/editor/propertyTab/ExtractionPropertyTab.h widget/editor/propertyTab/ExtractionPropertyTab.cpp
        widget/editor/propertyTab/CleanPropertyTab.h widget/editor/propertyTab/CleanPropertyTab.cpp
        widget/editor/propertyTab/EditPropertyTab.h widget/editor/propertyTab/EditPropertyTab.cpp
        widget/editor/propertyTab/SavePropertyTab.h widget/editor/propertyTab/SavePropertyTab.cpp
        widget/editor/editTab/ExtractionEditArea.h widget/editor/editTab/ExtractionEditArea.cpp
        widget/editor/editArea/EditAreaRect.h widget/editor/editArea/EditAreaRect.cpp
        widget/editor/editArea/SelectAreaRect.h widget/editor/editArea/SelectAreaRect.cpp
        widget/editor/editTab/ATextEditArea.h widget/editor/editTab/ATextEditArea.cpp
        widget/editor/editTab/EditorEditArea.h widget/editor/editTab/EditorEditArea.cpp
        widget/editor/editTab/CleanEditArea.h widget/editor/editTab/CleanEditArea.cpp
        widget/editor/editTab/SaveEditArea.h widget/editor/editTab/SaveEditArea.cpp
        include/ITextEditArea.h
        widget/imageViewer/ImageViewer.h widget/imageViewer/ImageViewer.cpp
        widget/imageViewer/PageGraphicsView.h widget/imageViewer/PageGraphicsView.cpp
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
