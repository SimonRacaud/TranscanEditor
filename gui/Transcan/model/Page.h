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
#include "include/models.h"

using std::shared_ptr;
using std::vector;

struct OCRBlock {
    QString uuid;
    QPolygon polygon;
    QString text;
    Vector2i pivot;
    Vector2i size;
    float angle;

    static shared_ptr<OCRBlock> deserialize(QJsonObject const &obj);
    QJsonObject serialize() const;
};

struct BlockCluster {
    std::vector<shared_ptr<OCRBlock>> blocks;
    QString sentence;
    bool cleanBox;
    QPolygon polygon;
    QString translation;
    RenderConfig style;

    static BlockCluster deserialize(QJsonObject const &obj,
                                    vector<shared_ptr<OCRBlock>> &blocks);
    QJsonObject serialize() const;
};

struct OCRPage {
    unsigned int index;
    QString imagePath;
    QString cleanImagePath;
    QString renderImagePath;
    std::vector<shared_ptr<OCRBlock>> blocks;
    std::vector<BlockCluster> clusters;

    static OCRPage deserialize(QJsonObject &data);
    QJsonObject serialize() const;
};

#endif // PAGE_H
