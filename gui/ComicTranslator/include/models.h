#ifndef MODELS_H
#define MODELS_H

#include <QString>
#include <vector>

class ServiceItem {
public:
    const QString name;
    bool needConfig; // Do this service require configuration variable(s) ?
    const QString helpMessage = "";

    ServiceItem(const ServiceItem& copy)
        : name(copy.name), needConfig(copy.needConfig), helpMessage(copy.helpMessage)
    {}
    ServiceItem(const QString &name, bool needConfig, const QString &helpMessage = nullptr)
        : name(name), needConfig(needConfig), helpMessage(helpMessage)
    {}
};

class TranslationServiceItem : public ServiceItem {
    const std::vector<QString> srcLang;
    const std::vector<QString> destLang;

    TranslationServiceItem(
            const QString &name,
            bool needConfig,
            const std::vector<QString> &srcLanguages,
            const std::vector<QString> &destLanguages,
            const QString &helpMessage = nullptr);
};

#endif // MODELS_H
