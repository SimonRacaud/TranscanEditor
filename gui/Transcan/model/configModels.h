#ifndef CONFIGMODELS_H
#define CONFIGMODELS_H

#include "include/env_config.h"

#include <QString>
#include <QFont>
#include <QJsonObject>
#include <QColor>

/**
 * Config
 */

class RenderConfig {
public:
    unsigned int lineHeight;
    QFont font;
    unsigned int strokeWidth;
    QColor color;

    RenderConfig(
            unsigned int defLineHeight = DEF_EDIT_LINE_HEIGHT,
            QFont const &defFont = QFont(DEF_EDIT_FONT, -1, DEF_EDIT_STROKE_WIDTH),
            unsigned int defStrokeWidth = DEF_EDIT_STROKE_WIDTH,
            QColor defColor = DEF_EDIT_COLOR)
        : lineHeight(defLineHeight),
          font(defFont),
          strokeWidth(defStrokeWidth),
          color(defColor) {}

    QJsonObject serialize() const
    {
        QJsonObject obj;
        obj["lineHeight"] = (qint64)this->lineHeight;
        obj["font"] = this->font.family();
        obj["strokeWidth"] = (qint64)this->strokeWidth;
        obj["color"] = (qint64)this->color.rgba64();
        return obj;
    }
};

class ProjectConfig {
public:
    QString projectName;
    QString srcPath;
    QString destPath;
    QString OCRService;
    QString transService;
    QString transSrc;
    QString transDest;
    RenderConfig renderConf;

public:
    ProjectConfig(
            QString const &projectName,
            QString const &srcPath,
            QString const &destPath,
            QString const &OCRService,
            QString const &transService,
            QString const &transSrc,
            QString const &transDest,
            QFont const &font
            ) : projectName(projectName),
                srcPath(srcPath),
                destPath(destPath),
                OCRService(OCRService),
                transService(transService),
                transSrc(transSrc),
                transDest(transDest),
                renderConf(DEF_EDIT_LINE_HEIGHT, font, DEF_EDIT_STROKE_WIDTH, DEF_EDIT_COLOR)
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
        o["style"] = this->renderConf.serialize();
        return o;
    }
};

#endif // CONFIGMODELS_H
