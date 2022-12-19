#include "Page.h"

#include "include/environment.h"

/**
 * Sub-Functions
 */

Vector2i deserializeVector(QJsonObject const &obj)
{
    Vector2i d;

    if (!obj.contains("x") || !obj.contains("y")) {
        throw std::invalid_argument("Vector2i: invalid JSON");
    }
    d.x = obj["x"].toInt();
    d.y = obj["y"].toInt();
    return d;
}

QPolygon deserializePolygon(QJsonValue const &obj)
{
    QPolygon poly;
    QJsonArray polyList = obj.toArray();
    if (polyList.size() != 4) {
        throw std::invalid_argument("Polygon: invalid JSON");
    }
    for (QJsonValue const& val : polyList) {
        QJsonArray point = val.toArray();
        if (point.size() != 2) {
            throw std::invalid_argument("Polygon: invalid JSON");
        }
        QPoint p;
        p.setX(point.at(0).toInt());
        p.setY(point.at(1).toInt());
        poly << p;
    }
    return poly;
}

/**
 * Blocks
 */

OCRBlock OCRBlock::deserialize(QJsonObject const &obj)
{
    OCRBlock b;
    QList<QString> required = {"angle", "pivot", "polygon", "size", "text"};

    for (QString const& req : required) {
        if (!obj.contains(req)) {
            throw std::invalid_argument("Block: invalid JSON, missing key "+req.toStdString());
        }
    }
    b.angle = obj["angle"].toDouble(0);
    b.text = obj["text"].toString("");
    b.pivot = deserializeVector(obj["pivot"].toObject());
    b.size = deserializeVector(obj["size"].toObject());
    b.polygon = deserializePolygon(obj["polygon"]);
    return b;
}

/**
 * Cluster
 */

BlockCluster BlockCluster::deserialize(QJsonObject const &obj)
{
    BlockCluster b;
    QList<QString> required = {"blocks", "cleanBox", "color", "font", "lineHeight", "polygon",
                               "sentence", "strokeWidth", "translation"};

    // Check keys
    for (QString const& req : required) {
        if (!obj.contains(req)) {
            throw std::invalid_argument("Cluster: invalid JSON, missing key "+req.toStdString());
        }
    }
    //
    b.cleanBox = obj["cleanBox"].toBool(true);
    b.color = QColor(obj["color"].toInteger(0));
    b.lineHeight = obj["lineHeight"].toDouble(DEF_EDIT_LINE_HEIGHT);
    b.strokeWidth = obj["strokeWidth"].toInt(DEF_EDIT_STROKE_WIDTH);
    b.font = QFont(obj["font"].toString(), -1, b.strokeWidth);
    b.translation = obj["translation"].toString();
    b.sentence = obj["sentence"].toString();
    b.polygon = deserializePolygon(obj["polygon"]);
    //const QJsonArray &blockList = obj["blocks"].toArray();
    //for (QJsonValue const &val : blockList) {
        //const QJsonObject &blockJson = val.toObject();
        //b.blocks.push_back(OCRBlock::deserialize(blockJson));
        // TODO: deserialize cluster's blocks
    //}
    return b;
}

/**
 * Page
 */

OCRPage OCRPage::deserialize(QJsonObject &data)
{
    OCRPage page;
    QList<QString> required = {"index", "srcImgPath", "cleanImgPath", "renderImgPath", "blocks", "clusters"};

    for (QString const& req : required) {
        if (!data.contains(req)) {
            throw std::invalid_argument("invalid JSON, missing key "+req.toStdString());
        }
    }
    // Check key exists
    int index = data["index"].toInt(-1);
    if (index < 0) {
        throw std::invalid_argument("invalid JSON, index");
    }
    page.index = index;
    page.imagePath = data["srcImgPath"].toString("");
    if (!FileUtils::checkImgFilePath(page.imagePath)) {
        throw std::invalid_argument("invalid JSON, source image");
    }
    page.cleanImagePath = data["cleanImgPath"].toString("#invalid");
    if (page.cleanImagePath == ""
            || !FileUtils::checkImgFilePath(page.cleanImagePath)
            || !FileUtils::checkDirExist(page.cleanImagePath)) {
        throw std::invalid_argument("invalid JSON, clean image");
    }
    page.renderImagePath = data["renderImgPath"].toString("#invalid");
    if (page.renderImagePath == ""
            || !FileUtils::checkImgFilePath(page.renderImagePath)
            || !FileUtils::checkDirExist(page.renderImagePath)) {
        throw std::invalid_argument("invalid JSON, rendered image");
    }
    const QJsonArray &blockList = data["blocks"].toArray();
    for (QJsonValue const &val : blockList) {
        const QJsonObject &blockJson = val.toObject();
        page.blocks.push_back(OCRBlock::deserialize(blockJson));
    }
    const QJsonArray &clusterList = data["clusters"].toArray();
    for (QJsonValue const &val : clusterList) {
        const QJsonObject &clusterJson = val.toObject();
        page.clusters.push_back(BlockCluster::deserialize(clusterJson));
    }
    return page;
}
