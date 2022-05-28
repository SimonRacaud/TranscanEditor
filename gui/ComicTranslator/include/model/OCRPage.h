#ifndef OCRPAGE_H
#define OCRPAGE_H

#include <stdint.h>
#include <vector>
#include <QFont>
#include <QColor>

#include "Utils.h"

struct BlockCluster {
    std::vector<uint32_t> blocks;
    int polygon[4][2];
    QString sentence;
    QString translation;
    QFont font;
    QColor color;
};

struct OCRBlock {
    int id;
    bool cleanBlock;
    Rectangle box;
    int polygon[4][2];
    QString text;
    Vector2i pivot;
    Vector2i size;
    float angle;
};

struct OCRPage {
    std::vector<OCRBlock> blocks;
    QString imagePath;
    std::vector<BlockCluster> clusters;
};

#endif // OCRPAGE_H
