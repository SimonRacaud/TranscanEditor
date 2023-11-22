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

#include "model/MathUtils.h"
#include "model/configModels.h"

using std::shared_ptr;
using std::vector;

/***
 * This file define the main structures defining an edited document
 * They are all serializable in JSON to be exchanged with the core module
 **/

struct OCRBlock {
    QString uuid;
    QPolygon polygon;
    QString text;
    Vector2i pivot;
    Vector2i size;
    float angle{0.0f};

    static shared_ptr<OCRBlock> deserialize(QJsonObject const &obj);
    QJsonObject serialize() const;
};

struct BlockCluster {
    std::vector<shared_ptr<OCRBlock>> blocks;
    QString sentence;
    bool cleanBox{true};
    QPolygon polygon;
    QString translation;
    RenderConfig style;

    static BlockCluster deserialize(QJsonObject const &obj,
                                    vector<shared_ptr<OCRBlock>> &blocks);
    QJsonObject serialize() const;
    void scale(int scale);
};

struct OCRPage {
    // main attributes:
    unsigned int index{0};
    QString imagePath; // Original image path
    QString sourceImagePath; // Resized original image path
    QString cleanImagePath; // Clean source image
    QString renderImagePath; // Edited cleaned image
    std::vector<shared_ptr<OCRBlock>> blocks;
    std::vector<BlockCluster> clusters;

    static OCRPage deserialize(QJsonObject &data);
    QJsonObject serialize() const;
};

#endif // PAGE_H
