#include "EditorEditArea.h"

using namespace std;

EditorEditArea::EditorEditArea(): ATextEditArea(RectMode::EDIT_TRAN)
{
    // -----------------------
    vector<OCRPage> debug = { // TODO: debug
    {
        .imagePath = "/media/work/personnal-projects/scanTranslator/scrapper/data_overlord/Chapter_1/5-o.jpg",
        .clusters = {
          BlockCluster { // DEBUG
              .blocks = {},
              .sentence = "Hello World",
              //.box = QRect(0, 0, 200, 100),
              .polygon = {QPoint(0, 0), QPoint(200, 0), QPoint(200, 100), QPoint(0, 100)},
              .translation = "LorenIpsum",
              .font = QFont(),
              .color = Qt::blue,
              .lineHeight = 100
          },
        },
    }};
    this->setPages(debug); // DEBUG
    //this->setSourceDirectory("/media/work/personnal-projects/scanTranslator/data/dataset/debug");
}
