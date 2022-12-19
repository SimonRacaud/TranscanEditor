#include "ExtractionEditArea.h"
#include "model/Page.h"
#include <vector>

using namespace std;

ExtractionEditArea::ExtractionEditArea() : ATextEditArea(RectMode::EDIT_SENT)
{
//    vector<OCRPage> debug = { // TODO: debug
//        {
//            .imagePath = "/media/work/personnal-projects/scanTranslator/scrapper/data_overlord/Chapter_1/5-o.jpg",
//            .clusters = {
//              BlockCluster { // DEBUG
//                  .blocks = {},
//                  .sentence = "Hello World",
//                  .polygon = {QPoint(100, 200), QPoint(300, 200), QPoint(300, 300), QPoint(100, 300)},
//                  .translation = "LorenIpsum",
//                  .font = QFont(),
//                  .color = Qt::blue,
//                  .lineHeight = 100
//              },
//            }
//        },
//        { .imagePath = "/media/work/personnal-projects/scanTranslator/scrapper/data_overlord/Chapter_1/5-o.jpg" },
//        { .imagePath = "/media/work/personnal-projects/scanTranslator/scrapper/data_overlord/Chapter_1/5-o.jpg" },
//    };
//    this->setPages(debug); // DEBUG
}

void ExtractionEditArea::load(std::vector<OCRPage> const &pages)
{
    this->setPages(pages); // Apply pages to view
}

void ExtractionEditArea::loadPage(OCRPage const &page)
{
    // Replace page and Reload view
    this->updatePage(page);
}

void ExtractionEditArea::unload()
{
    // TODO
    // Stop any API call

}
