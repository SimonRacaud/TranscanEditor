#ifndef SERVICEITEM_H
#define SERVICEITEM_H

#include <QString>
#include <QStringList>

class ServiceItem {
public:
    const QString name;
    const QString apiLabel;
    bool needConfig; // Do this service require configuration variable(s) ?
    const QString helpMessage = nullptr;

    ServiceItem(const ServiceItem& copy)
        : name(copy.name), needConfig(copy.needConfig), helpMessage(copy.helpMessage)
    {}
    ServiceItem(const QString &name, const QString &apiLabel, bool needConfig, const QString &helpMessage = nullptr)
        : name(name), apiLabel(apiLabel), needConfig(needConfig), helpMessage(helpMessage)
    {}
};

class TranslationServiceItem : public ServiceItem {
public:
    const QStringList srcLang;
    const QStringList destLang;

    TranslationServiceItem(
            const QString &name,
            const QString &apiLabel,
            bool needConfig,
            const QString &helpMessage = nullptr,
            const QStringList &srcLanguages = {
                "en"
            },
            const QStringList &destLanguages = {
                "fr"
            })
        : ServiceItem(name, apiLabel, needConfig, helpMessage), srcLang(srcLanguages), destLang(destLanguages) {};
};

#endif // SERVICEITEM_H
