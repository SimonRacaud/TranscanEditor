#include "ExtractionEditTab.h"
#include "model/Page.h"
#include <vector>
#include "widget/misc/Notification.h"
#include "include/env_messages.h"

using namespace std;

ExtractionEditTab::ExtractionEditTab(APIClient &client)
    : ATextEditTab(client, SOURCE, RectMode::EDIT_SENT, false)
{
}

void ExtractionEditTab::loadAPI()
{
    if (!_config) {
        throw std::runtime_error("ExtractionEditTab::loadAPI Cannot access project configuration.");
    }
    try {
        this->setLoadingState(true);
        size_t i = 0;
        for (OCRPage const &page : _pages) {
            using std::placeholders::_1;
            NetCallback callback = bind(&IEditTab::loadPage, this, _1);
            NetErrCallback errCallback = bind(&ExtractionEditTab::netError, this, _1);
            this->_api.sendToOCR((*_config), i, page.sourceImagePath, callback, errCallback);
            i++;
        }
    } catch (std::exception const &err) {
        qDebug() << "ExtractionEditTab::loadAPI exception : " << err.what();
        this->setLoadingState(false);
        Notification::Build(ERR_SEND_CORE_REQ, this);
    }
}
