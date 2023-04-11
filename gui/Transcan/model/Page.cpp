#include "Page.h"

#include "include/environment.h"
#include <iostream>

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

QJsonObject serializeVector(Vector2i const &v)
{
    QJsonObject obj;

    obj["x"] = v.x;
    obj["y"] = v.y;
    return obj;
}

//

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

QJsonArray serializePolygon(QPolygon const &p)
{
    QJsonArray polyList;

    for (QPoint const &pt : p) {
        QJsonArray point;
        point << pt.x() << pt.y();
        polyList << point;
    }
    return polyList;
}

/**
 * Blocks
 */

shared_ptr<OCRBlock> OCRBlock::deserialize(QJsonObject const &obj)
{
    shared_ptr<OCRBlock> b = std::make_shared<OCRBlock>();
    QList<QString> required = {"id", "angle", "pivot", "polygon", "size", "text"};

    for (QString const& req : required) {
        if (!obj.contains(req)) {
            throw std::invalid_argument("Block: invalid JSON, missing key "+req.toStdString());
        }
    }
    b->uuid = obj["id"].toString();
    if (b->uuid.isEmpty()) {
        throw std::invalid_argument("Block: invalid id field.");
    }
    b->angle = obj["angle"].toDouble(0);
    b->text = obj["text"].toString("");
    b->pivot = deserializeVector(obj["pivot"].toObject());
    b->size = deserializeVector(obj["size"].toObject());
    b->polygon = deserializePolygon(obj["polygon"]);
    return b;
}

QJsonObject OCRBlock::serialize() const
{
    QJsonObject obj;

    obj["id"] = this->uuid;
    obj["angle"] = this->angle;
    obj["pivot"] = serializeVector(this->pivot);
    obj["polygon"] = serializePolygon(this->polygon);
    obj["size"] = serializeVector(this->size);
    obj["text"] = this->text;
    return obj;
}

/**
 * Cluster
 */

BlockCluster BlockCluster::deserialize(QJsonObject const &obj, vector<shared_ptr<OCRBlock>> &blocks)
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
    b.style.color = QColor(obj["color"].toInteger(0));
    b.style.lineHeight = obj["lineHeight"].toDouble(DEF_EDIT_LINE_HEIGHT);
    b.style.strokeWidth = obj["strokeWidth"].toInt(DEF_EDIT_STROKE_WIDTH);
    b.style.font = QFont(obj["font"].toString());
    if (b.style.strokeWidth > 9 || !b.style.strokeWidth) {
        throw std::invalid_argument("Cluster deserialize, invalid stroke width");
    }
    b.style.font.setWeight(weightChoices[b.style.strokeWidth - 1]);
    b.translation = obj["translation"].toString();
    b.sentence = obj["sentence"].toString();
    b.polygon = deserializePolygon(obj["polygon"]);

    // deserialize cluster's blocks
    const QJsonArray &blockList = obj["blocks"].toArray(); // Blocks uuids
    for (QJsonValue const &val : blockList) {
        const QString &uuid = val.toString();
        auto it = std::find_if(blocks.begin(), blocks.end(),
                               [uuid](shared_ptr<OCRBlock> const &b){ return b->uuid == uuid; });
        if (it == blocks.end()) {
            std::cerr << "Deserializer: Block id " << uuid.toStdString() << " not found." << std::endl;
            throw std::invalid_argument("Cluster: block id not found.");
        }
        b.blocks.push_back(*it);
    }
    return b;
}

QJsonObject BlockCluster::serialize() const
{
    QJsonObject obj;
    QJsonArray blocksArray;

    for (shared_ptr<OCRBlock> const &b : this->blocks) {
        blocksArray << b->uuid;
    }
    obj["blocks"] = blocksArray;
    obj["cleanBox"] = this->cleanBox;
    obj["translation"] = this->translation;
    obj["polygon"] = serializePolygon(this->polygon);
    obj["sentence"] = this->sentence;

    const QJsonObject &styleObj = this->style.serialize();
    for (auto it = styleObj.constBegin(); it != styleObj.constEnd(); it++) {
        obj.insert(it.key(), it.value());
    }
    return obj;
}

/**
 * Page
 */

OCRPage OCRPage::deserialize(QJsonObject &data)
{
    OCRPage page;
    QList<QString> required = {"index", "srcImgPath", "cleanImgPath", "renderImgPath",
                               "blocks", "clusters"};

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
    if (page.cleanImagePath != ""
            && !FileUtils::checkImgFilePath(page.cleanImagePath)
            && !FileUtils::checkDirExist(page.cleanImagePath)) {
        throw std::invalid_argument("invalid JSON, clean image");
    }
    page.renderImagePath = data["renderImgPath"].toString("#invalid");
    if (page.renderImagePath != ""
            && !FileUtils::checkImgFilePath(page.renderImagePath)
            && !FileUtils::checkDirExist(page.renderImagePath)) {
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
        page.clusters.push_back(BlockCluster::deserialize(clusterJson, page.blocks));
    }
    return page;
}

QJsonObject OCRPage::serialize() const
{
    QJsonObject obj;

    obj["index"] = (qint64)this->index;
    obj["srcImgPath"] = this->imagePath;
    obj["cleanImgPath"] = this->cleanImagePath;
    obj["renderImgPath"] = this->renderImagePath;

    QJsonArray blockList;
    for (shared_ptr<OCRBlock> const &b : this->blocks) {
        blockList << b->serialize();
    }
    obj["blocks"] = blockList;

    QJsonArray clusterList;
    for (BlockCluster const &c : this->clusters) {
        clusterList << c.serialize();
    }
    obj["clusters"] = clusterList;
    return obj;
}
