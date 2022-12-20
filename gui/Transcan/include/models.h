#ifndef MODELS_H
#define MODELS_H

#include <QString>
#include <QList>
#include <vector>
#include <QFont>

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

#include <QJsonObject>

class ProjectConfig {
public:
    QString srcPath;
    QString destPath;
    QString OCRService;
    QString transService;
    QString transSrc;
    QString transDest;
    QFont font;

public:
    ProjectConfig(
            QString const &srcPath,
            QString const &destPath,
            QString const &OCRService,
            QString const &transService,
            QString const &transSrc,
            QString const &transDest,
            QFont const &font
            ) : srcPath(srcPath), destPath(destPath), OCRService(OCRService),
                transService(transService), transSrc(transSrc), transDest(transDest), font(font)
    {}

    QJsonObject serialize() const
    {
        QJsonObject o;
        o["srcImgPath"] = this->srcPath;
        o["outputFolder"] = this->destPath;
        o["ocrService"] = this->OCRService;
        //o["cache_path"] = "";
        //o["boxClusterSearchRange"] = 0;
        //o["boxClusterSearchStep"] = 0;
        return o;
    }
};

class RenderConfig {
public:
    unsigned int defLineHeight;
    QFont defFont;
    unsigned int defStrokeWidth;

    RenderConfig(unsigned int defLineHeight, QFont defFont, unsigned int defStrokeWidth)
        : defLineHeight(defLineHeight), defFont(defFont), defStrokeWidth(defStrokeWidth) {}

    QJsonObject serialize() const
    {
        QJsonObject obj;
        obj["lineHeight"] = (qint64)this->defLineHeight;
        obj["font"] = this->defFont.family();
        obj["strokeWidth"] = (qint64)this->defStrokeWidth;
        return obj;
    }
};

#endif // MODELS_H
