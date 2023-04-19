#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <QStringList>

#include "ServiceItem.h"

const ServiceItem OCR_SERVICE_LIST[] = {
    ServiceItem("Local - Craft/Tesseract", "LOCAL_CRAFT_TESSERACT", false),
    ServiceItem("AWS Rekognition", "AWS_REKOGNITION", true, "AWS_ACCESS_KEY_ID and AWS_SECRET_ACCESS_KEY variables are required.")
};
const QString SERVICE_DEFAULT_HELP = "The service require configuration variables";

#define TRANSLATE_DISABLED "Disable"
const TranslationServiceItem TRANS_SERVICE_LIST[] = {
    TranslationServiceItem(TRANSLATE_DISABLED, "DISABLE", false),
    TranslationServiceItem("Google", "GOOGLE", false),
    TranslationServiceItem("Microsoft", "MICROSOFT", true, "TRAD_KEY_MICROSOFT required"),
    TranslationServiceItem("Deepl", "DEEPL", true, "TRAD_KEY_DEEPL is required, change TRAD_DEEPL_USE_FREE to use premium services"),
    TranslationServiceItem("Linguee", "LINGUEE", false),
    TranslationServiceItem("AWS", "AWS", true, "AWS_ACCESS_KEY_ID and AWS_SECRET_ACCESS_KEY are required"),
    TranslationServiceItem("Pons", "PONS", false),
    TranslationServiceItem("MyMemory", "MYMEMORY", false),
    TranslationServiceItem("Yandex", "YANDEX", true, "TRAD_KEY_YANDEX required"),
    TranslationServiceItem("Papago", "PAPAGO", true, "TRAD_KEY_PAPAGO and TRAD_CLIENT_ID_PAPAGO required"),
    TranslationServiceItem("Libre", "LIBRE", true, "TRAD_KEY_LIBRE and TRAD_LIBRE_BASE_URL variables are required")
};

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


#define API_URL "http://127.0.0.1:5000"

// Style

#define EDITOR_BG 0xF2F2F2
#define EDITOR_MIN_SIZE 800, 600
#define EDITOR_DEF_SIZE 1200, 720

#define EDITOR_EDITA_BG 0xBED8D4

#define EDITOR_HEAD_MIDDLE_SPACING 8
#define EDITOR_HEAD_MARGIN 5
#define EDITOR_HEAD_SEPARATOR ":/asset/img/leftArrow.svg"
#define EDITOR_HEAD_SEPARATOR_HEIGHT 12

#define EDITOR_PROPTAB_WIDTH 300
#define EDITOR_PROPTAB_BG 0xFFFFFF
#define EDITOR_PROP_HELP_MIN_HEIGHT 200
#define EDITOR_PROP_SPACING 20
#define EDITOR_PROP_FORM_WIDTH 260
#define EDITOR_TIME_REFRESH_RESIZE 5 // ms

#define LOADING_ICON_SIZE 80 // Page viewer

#define EDITOR_PAGE_SHADOW_RADIUS 80
#define EDITOR_PAGE_SHADOW_COLOR QColor(170, 170, 170)


// Asset

#define IMG_LOADING ":/asset/img/loading.gif"

// Config

#define DEF_LINE_HEIGHT 100 // Editor -> Text
#define DEF_EDITAREA_SIZE 100, 50 // Editor

#define DEF_EDIT_LINE_HEIGHT 30 // Edition/render config
#define DEF_EDIT_STROKE_WIDTH 4 // Normal
#define DEF_EDIT_FONT "Arial"
#define DEF_EDIT_COLOR QColor(Qt::black)

#include <QFont>
static const QFont::Weight weightChoices[] = {QFont::Thin, QFont::ExtraLight, QFont::Light, QFont::Normal, QFont::Medium, QFont::DemiBold, QFont::Bold, QFont::ExtraBold, QFont::Black};

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

#endif // ENVIRONMENT_H
