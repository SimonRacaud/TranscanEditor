#ifndef PAGE_H
#define PAGE_H

#include <stdint.h>
#include <vector>
#include <QFont>
#include <QColor>
#include <QRect>
#include <QPolygon>
#include <QJsonObject>
#include <QJsonArray>

#include "include/model/Utils.h"

#include "utils/FileUtils.h"

struct OCRBlock {
    QPolygon polygon;
    QString text;
    Vector2i pivot;
    Vector2i size;
    float angle;

    static OCRBlock deserialize(QJsonObject const &obj);
};

struct BlockCluster {
    std::vector<OCRBlock *> blocks;
    QString sentence;
    bool cleanBox;
    QPolygon polygon;
    QString translation;
    QFont font;
    QColor color;
    float lineHeight;
    int strokeWidth;

    static BlockCluster deserialize(QJsonObject const &obj);
};

struct OCRPage {
    unsigned int index;
    QString imagePath;
    QString cleanImagePath;
    QString renderImagePath;
    std::vector<OCRBlock> blocks;
    std::vector<BlockCluster> clusters;

    static OCRPage deserialize(QJsonObject &data);
};

#endif // PAGE_H
