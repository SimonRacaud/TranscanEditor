#include "ExtractionEditArea.h"
#include "include/model/OCRPage.h"
#include <vector>

using namespace std;

ExtractionEditArea::ExtractionEditArea() : ATextEditArea(RectMode::EDIT_SENT)
{
    // -----------------------
    vector<OCRPage> debug = { // TODO: debug
        {
            .imagePath = "/media/work/personnal-projects/scanTranslator/scrapper/data_overlord/Chapter_1/5-o.jpg",
            .clusters = {
              BlockCluster { // DEBUG
                  .blocks = {},
                  .sentence = "Hello World",
                  .box = QRect(0, 0, 200, 100),
                  .polygon = {QPoint(0, 0), QPoint(200, 0), QPoint(200, 100), QPoint(0, 100)},
                  .translation = "LorenIpsum",
                  .font = QFont(),
                  .color = Qt::blue,
                  .line_height = 100,
              },
              BlockCluster { // DEBUG
                  .blocks = {},
                  .sentence = "Hello World",
                  .box = QRect(100, 200, 200, 100),
                  .polygon = {},
                  .translation = "LorenIpsum",
                  .font = QFont(),
                  .color = Qt::blue,
                  .line_height = 100,
              },
            }
        },
        { .imagePath = "/media/work/personnal-projects/scanTranslator/scrapper/data_overlord/Chapter_1/5-o.jpg" },
        { .imagePath = "/media/work/personnal-projects/scanTranslator/scrapper/data_overlord/Chapter_1/5-o.jpg" },
    };
    this->setPages(debug); // DEBUG
    this->setSourceDirectory("/media/work/personnal-projects/scanTranslator/data/dataset/debug");
}
