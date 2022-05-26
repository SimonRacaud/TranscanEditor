#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <QStringList>

const QStringList OCR_SERVICE_LIST = {
    "Local - Craft/Tesseract",
    "AWS Rekognition"
};

const QStringList TRANS_SERVICE_LIST = {
    "Disable",
    "Google",
    "Microsoft",
    "Deepl",
    "Linguee",
    "AWS",
    "Pons",
    "MyMemory",
    "Yandex",
    "Papago",
    "Libre"
};


#endif // ENVIRONMENT_H
