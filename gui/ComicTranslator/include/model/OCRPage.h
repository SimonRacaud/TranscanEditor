#ifndef OCRPAGE_H
#define OCRPAGE_H

#include <stdint.h>
#include <vector>
#include <QFont>
#include <QColor>

#include "Utils.h"

struct OCRBlock {
    bool cleanBlock;
    Rectangle box;
    int polygon[4][2];
    QString text;
    Vector2i pivot;
    Vector2i size;
    float angle;
};

struct BlockCluster {
    std::vector<OCRBlock *> blocks;
    int polygon[4][2];
    QString sentence;
    QString translation;
    QFont font;
    QColor color;
};

struct OCRPage {
    QString imagePath;
    QString cleanImagePath;
    QString resultImagePath;
    std::vector<OCRBlock> blocks;
    std::vector<BlockCluster> clusters;
};

#endif // OCRPAGE_H
