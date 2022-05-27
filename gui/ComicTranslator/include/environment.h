#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <QStringList>

#include "models.h"

const ServiceItem OCR_SERVICE_LIST[] = {
    ServiceItem("Local - Craft/Tesseract", false),
    ServiceItem("AWS Rekognition", true, "AWS_ACCESS_KEY_ID and AWS_SECRET_ACCESS_KEY variables are required.")
};
const QString SERVICE_DEFAULT_HELP = "The service require configuration variables";

const ServiceItem TRANS_SERVICE_LIST[] = {
    ServiceItem("Disable", false),
    ServiceItem("Google", false),
    ServiceItem("Microsoft", true, "TRAD_KEY_MICROSOFT required"),
    ServiceItem("Deepl", true, "TRAD_KEY_DEEPL is required, change TRAD_DEEPL_USE_FREE to use premium services"),
    ServiceItem("Linguee", false),
    ServiceItem("AWS", true, "AWS_ACCESS_KEY_ID and AWS_SECRET_ACCESS_KEY are required"),
    ServiceItem("Pons", false),
    ServiceItem("MyMemory", false),
    ServiceItem("Yandex", true, "TRAD_KEY_YANDEX required"),
    ServiceItem("Papago", true, "TRAD_KEY_PAPAGO and TRAD_CLIENT_ID_PAPAGO required"),
    ServiceItem("Libre", true, "TRAD_KEY_LIBRE and TRAD_LIBRE_BASE_URL variables are required")
};


#endif // ENVIRONMENT_H
