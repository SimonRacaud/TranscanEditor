#ifndef OCRPAGE_H
#define OCRPAGE_H

#include <stdint.h>
#include <vector>
#include <QFont>
#include <QColor>
#include <QRect>
#include <QPoint>

#include "Utils.h"

struct OCRBlock {
    bool cleanBlock;
    Rectangle box;
    QPoint polygon[4];
    QString text;
    Vector2i pivot;
    Vector2i size;
    float angle;
};

struct BlockCluster {
    std::vector<OCRBlock *> blocks;
    // Extraction (modified)
    QString sentence;
    // Extraction & Edition (modified)
    // Clean (used)
    QRect box;
    // Clean (used)
    QPoint polygon[4];
    // Edition
    QString translation;
    QFont font;
    QColor color;
    float line_height;
};

struct OCRPage {
    QString imagePath;
    QString cleanImagePath;
    QString resultImagePath;
    std::vector<OCRBlock> blocks;
    std::vector<BlockCluster> clusters;
};

#endif // OCRPAGE_H
