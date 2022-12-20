#ifndef MODELS_H
#define MODELS_H

#include "environment.h"

#include <QString>
#include <QFont>
#include <QJsonObject>

/**
 * Config
 */

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

class ProjectConfig {
public:
    QString srcPath;
    QString destPath;
    QString OCRService;
    QString transService;
    QString transSrc;
    QString transDest;
    QFont font;
    RenderConfig renderConf = RenderConfig(DEF_EDIT_LINE_HEIGHT, QFont(DEF_EDIT_FONT, -1, DEF_EDIT_STROKE_WIDTH), DEF_EDIT_STROKE_WIDTH);

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

#endif // MODELS_H
