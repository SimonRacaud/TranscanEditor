#ifndef ENV_SERVICES_H
#define ENV_SERVICES_H

#include "model/ServiceItem.h"

// List of OCR Services available
const ServiceItem OCR_SERVICE_LIST[] = {
    ServiceItem("Local - Craft/Tesseract", "LOCAL_CRAFT_TESSERACT", false),
    ServiceItem("AWS Rekognition", "AWS_REKOGNITION", true, "AWS_ACCESS_KEY_ID and AWS_SECRET_ACCESS_KEY variables are required."),
    ServiceItem("AWS Textract", "AWS_TEXTRACT", true, "AWS_ACCESS_KEY_ID and AWS_SECRET_ACCESS_KEY variables are required.")
};
const QString SERVICE_DEFAULT_HELP = "The service require configuration variables";

// List of machine Translation services
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

#endif // ENV_SERVICES_H
