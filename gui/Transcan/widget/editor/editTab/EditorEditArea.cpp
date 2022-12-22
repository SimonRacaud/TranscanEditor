#include "EditorEditArea.h"
#include <functional>

using namespace std;

EditorEditArea::EditorEditArea(APIClient &client): ATextEditArea(client, CLEAN, RectMode::EDIT_TRAN)
{
    // -----------------------
//    vector<OCRPage> debug = { // TODO: debug
//    {
//        .imagePath = "/media/work/personnal-projects/scanTranslator/scrapper/data_overlord/Chapter_1/5-o.jpg",
//        .clusters = {
//          BlockCluster { // DEBUG
//              .blocks = {},
//              .sentence = "Hello World",
//              .cleanBox = true,
//              //.box = QRect(0, 0, 200, 100),
//              .polygon = {QPoint(0, 0), QPoint(200, 0), QPoint(200, 100), QPoint(0, 100)},
//              .translation = "LorenIpsum",
//              .font = QFont(),
//              .color = Qt::blue,
//              .lineHeight = 100,
//          },
//        },
//    }};
//    this->setPages(debug); // DEBUG
    //this->setSourceDirectory("/media/work/personnal-projects/scanTranslator/data/dataset/debug");
}

void EditorEditArea::load(std::vector<OCRPage> const &pages)
{
    // Load pages
    this->setPages(pages);
    // Translation API Call (if first load)
    if (this->_config == nullptr) {
        throw std::runtime_error("EditorEditArea::load null project config.");
    }
    this->setLoadingState(true);
    for (OCRPage const &page: pages) {
        NetCallback success = bind(&IEditTab::loadPage, this, std::placeholders::_1);
        NetErrCallback error = bind(&NetEditTab::netError, this, std::placeholders::_1);
        this->_api.sendToTranslate(page, _config->transService, success, error);
    }
}

void EditorEditArea::loadPage(OCRPage const &page)
{
    try {
        this->setLoadingState(false);
        this->updatePage(page);
    } catch (std::exception const &err) {
        std::cerr << "EditorEditArea::loadPage error: " << err.what() << std::endl;
    }
}
