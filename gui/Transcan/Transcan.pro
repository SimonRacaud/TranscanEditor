0QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
	main.cpp \
        window/MainWindow.cpp \
        window/ViewerWindow.cpp \
        page/editor/EditorView.cpp \
        page/editor/EditorController.cpp \
        page/home/HomePage.cpp \
        widget/editor/propertyTab/APropertyTab.cpp \
        widget/editor/propertyTab/ExtractionPropertyTab.cpp \
        widget/editor/propertyTab/CleanPropertyTab.cpp \
        widget/editor/propertyTab/EditPropertyTab.cpp \
        widget/editor/propertyTab/SavePropertyTab.cpp \
        widget/editor/editTab/ExtractionEditArea.cpp \
        widget/editor/editArea/EditAreaRect.cpp \
        widget/editor/editArea/SelectAreaRect.cpp \
        widget/editor/editTab/ATextEditArea.cpp \
        widget/editor/editTab/EditorEditArea.cpp \
        widget/editor/editTab/CleanEditArea.cpp \
        widget/editor/editTab/SaveEditArea.cpp \
        widget/imageViewer/ImageViewer.cpp \
        widget/imageViewer/PageGraphicsView.cpp \
        net/APIClient.cpp \
	utils/FileUtils.cpp 

HEADERS += \
    window/MainWindow.h \
    window/ViewerWindow.h \
    include/environment.h \
    include/models.h \
    include/model/OCRPage.h \
    include/model/Utils.h \
    page/editor/EditorView.h \
    page/editor/EditorController.h \
    page/home/HomePage.h \
    widget/editor/propertyTab/APropertyTab.h \
    widget/editor/propertyTab/ExtractionPropertyTab.h \
    widget/editor/propertyTab/CleanPropertyTab.h \
    widget/editor/propertyTab/EditPropertyTab.h \
    widget/editor/propertyTab/SavePropertyTab.h \
    widget/editor/editTab/ExtractionEditArea.h \
    widget/editor/editArea/EditAreaRect.h \
    widget/editor/editArea/SelectAreaRect.h \
    widget/editor/editTab/ATextEditArea.h \
    widget/editor/editTab/EditorEditArea.h \
    widget/editor/editTab/CleanEditArea.h \
    widget/editor/editTab/SaveEditArea.h \
    include/IEditTab.h \
    widget/imageViewer/ImageViewer.h \
    widget/imageViewer/PageGraphicsView.h \
    net/APIClient.h \
    utils/FileUtils.h

FORMS += \
    ui/home_page.ui

RESOURCES     = asset.qrc

TRANSLATIONS += \
    Transcan_fr_FR.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
