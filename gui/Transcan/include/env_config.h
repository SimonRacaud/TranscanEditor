#ifndef ENV_CONFIG_H
#define ENV_CONFIG_H

#include <QStringList>
#include <QFont>

/**
 * This file define global contants used in the software
 */

// URL of the CORE module's API
#define CORE_API_URL "http://127.0.0.1:5000"
#define NET_TIMEOUT_MS QNetworkRequest::DefaultTransferTimeoutConstant // 30 seconds

// Supported image extensions
const QList<QString> SUPPORTED_EXTENSION = {
    "*.png",
    "*.jpg",
    "*.jpeg"
};
const QList<QString> SUPPORTED_EXTENSION_SUFFIX = {
    "png",
    "jpg",
    "jpeg"
};
#define IMG_SELECT_FILTER "Images (*.png *.jpeg *.jpg)"

/////////////////////////////////
// Asset

#define IMG_LOADING ":/asset/img/loading.gif"
#define FONT_DIR_REG ":/asset/font/regular/"
#define FONT_DIR_SFX ":/asset/font/sfx/"

/////////////////////////////////
// Config

#define DEF_LINE_HEIGHT 100 // Editor -> Text
#define DEF_EDITAREA_SIZE 100, 50 // Editor

#define DEF_EDIT_LINE_HEIGHT 20 // Edition/render config
#define DEF_EDIT_STROKE_WIDTH 4 // Normal
#define DEF_EDIT_FONT "Wild Words"
#define DEF_EDIT_COLOR QColor(Qt::black)

static const QFont::Weight weightChoices[] = {
    QFont::Thin,
    QFont::ExtraLight,
    QFont::Light,
    QFont::Normal,
    QFont::Medium,
    QFont::DemiBold,
    QFont::Bold,
    QFont::ExtraBold,
    QFont::Black
};

// Editor page zoom key shortcut
#define ZOOM_SHIFT 0.05 // scale shift
#define ZOOM_MIN_INT 1
#define ZOOM_MAX_INT 500

enum class EditorTab {
    EXTRACT = 0,
    CLEAN = 1,
    EDIT = 2,
    SAVE = 3,
    LAST_VALUE // Nothing after this
};

// Notification
#define NOTIF_TRANSIT_TIME 1000 // ms
#define NOTIF_SHOW_TIME 3000 // ms
#define NOTIF_POS QPoint(20, 20)

// (Pages) Viewer Window
#define WIN_INIT_WIDTH 600 // px
#define WIN_INIT_HEIGHT 800 // px
#define TITLE_MAX_SIZE 450 // character
#define TIME_REFRESH_RESIZE 5 // ms

#endif // ENV_CONFIG_H
