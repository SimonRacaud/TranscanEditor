#ifndef MODELS_H
#define MODELS_H

#include <QString>
#include <QList>
#include <vector>

class ServiceItem {
public:
    const QString name;
    bool needConfig; // Do this service require configuration variable(s) ?
    const QString helpMessage = nullptr;

    ServiceItem(const ServiceItem& copy)
        : name(copy.name), needConfig(copy.needConfig), helpMessage(copy.helpMessage)
    {}
    ServiceItem(const QString &name, bool needConfig, const QString &helpMessage = nullptr)
        : name(name), needConfig(needConfig), helpMessage(helpMessage)
    {}
};

class TranslationServiceItem : public ServiceItem {
public:
    const QStringList srcLang;
    const QStringList destLang;

    TranslationServiceItem(
            const QString &name,
            bool needConfig,
            const QString &helpMessage = nullptr,
            const QStringList &srcLanguages = {
                "en"
            },
            const QStringList &destLanguages = {
                "fr"
            })
        : ServiceItem(name, needConfig, helpMessage), srcLang(srcLanguages), destLang(destLanguages) {};
};

#endif // MODELS_H
