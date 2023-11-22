QT       += core gui network svgwidgets

greaterThan(QT_MAJOR_VERSION, 5): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
	main.cpp \
        model/Page.cpp \
        utility/FontManager.cpp \
        widget/editor/box/TextEditBox.cpp \
        widget/editor/editTab/ATextEditTab.cpp \
        widget/editor/editTab/CleanEditTab.cpp \
        widget/editor/editTab/EditorEditTab.cpp \
        widget/editor/editTab/ExtractionEditTab.cpp \
        widget/editor/editTab/NetEditTab.cpp \
        widget/editor/editTab/SaveEditTab.cpp \
        widget/editor/header/EditorHeader.cpp \
        widget/misc/FontSelect.cpp \
        widget/misc/Notification.cpp \
        widget/misc/ZoomWidget.cpp \
        widget/editor/propertyTab/edit/ClusterStyleConfig.cpp \
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
        widget/editor/box/SelectAreaBox.cpp \
        widget/imageViewer/ImageViewer.cpp \
        widget/imageViewer/PageGraphicsView.cpp \
        net/APIClient.cpp \
        utility/FileUtils.cpp

HEADERS += \
    include/env_config.h \
    include/env_services.h \
    include/env_style.h \
    model/MathUtils.h \
    model/ServiceItem.h \
    include/resource_font.h \
    model/Page.h \
    model/configModels.h \
    utility/FontManager.h \
    widget/editor/box/TextEditBox.h \
    widget/editor/editTab/ATextEditTab.h \
    widget/editor/editTab/CleanEditTab.h \
    widget/editor/editTab/EditorEditTab.h \
    widget/editor/editTab/ExtractionEditTab.h \
    widget/editor/editTab/NetEditTab.h \
    widget/editor/editTab/SaveEditTab.h \
    widget/editor/header/EditorHeader.h \
    widget/misc/FontSelect.h \
    widget/misc/Notification.h \
    widget/misc/ZoomWidget.h \
    widget/editor/propertyTab/edit/ClusterStyleConfig.h \
    window/MainWindow.h \
    window/ViewerWindow.h \
    page/editor/EditorView.h \
    page/editor/EditorController.h \
    page/home/HomePage.h \
    widget/editor/propertyTab/APropertyTab.h \
    widget/editor/propertyTab/ExtractionPropertyTab.h \
    widget/editor/propertyTab/CleanPropertyTab.h \
    widget/editor/propertyTab/EditPropertyTab.h \
    widget/editor/propertyTab/SavePropertyTab.h \
    widget/editor/box/SelectAreaBox.h \
    widget/editor/editTab/IEditTab.h \
    widget/imageViewer/ImageViewer.h \
    widget/imageViewer/PageGraphicsView.h \
    net/APIClient.h \
    utility/FileUtils.h

FORMS += \
    ui/home_page.ui

RESOURCES     = asset.qrc \
                stylesheet.qss

TRANSLATIONS += \
    Transcan_fr_FR.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
