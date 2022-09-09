#include "ExtractionEditArea.h"
#include "include/model/OCRPage.h"
#include <vector>

ExtractionEditArea::ExtractionEditArea() : AEditArea()
{
    BlockCluster testData = { // DEBUG
        .blocks = {},
        .polygon = {{0, 0}, {200, 0}, {200, 100}, {0, 100}},
        .sentence = "Hello World",
        .translation = "LorenIpsum",
        .font = QFont(),
        .color = Qt::blue
    };
    this->_rect = new EditAreaRect(testData);

    vector<OCRPage> debug = { // TODO: debug
        { .imagePath = "/media/work/personnal-projects/scanTranslator/data/dataset/debug/10-o.jpg" },
        { .imagePath = "/media/work/personnal-projects/scanTranslator/data/dataset/debug/14-o.jpg" },
        { .imagePath = "/media/work/personnal-projects/scanTranslator/data/dataset/debug/9-o.jpg" },
    };
    this->setPages(debug);
}

void ExtractionEditArea::setPages(vector<OCRPage> const &pages)
{
    //AEditArea::setPages(pages);

    this->_rect->setPos(100,100);
    this->_scene->addItem(_rect);
    std::cerr << "TEST\n";
}
