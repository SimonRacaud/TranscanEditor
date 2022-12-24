#include "ExtractionEditArea.h"
#include "model/Page.h"
#include <vector>

using namespace std;

ExtractionEditArea::ExtractionEditArea(APIClient &client)
    : ATextEditArea(client, SOURCE, RectMode::EDIT_SENT, false)
{
}

void ExtractionEditArea::loadAPI()
{
    if (!_config) {
        throw std::runtime_error("ExtractionEditArea::loadAPI Cannot access project configuration.");
    }
    this->setLoadingState(true);
    size_t i = 0;
    for (OCRPage const &page : _pages) {
        using std::placeholders::_1;
        NetCallback callback = bind(&IEditTab::loadPage, this, _1);
        NetErrCallback errCallback = bind(&ExtractionEditArea::netError, this, _1);
        this->_api.sendToOCR((*_config), i, page.imagePath, callback, errCallback);
        i++;
        // TODO : to improve (error management)
    }
}
