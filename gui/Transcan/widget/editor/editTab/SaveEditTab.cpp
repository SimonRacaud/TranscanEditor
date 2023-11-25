#include "SaveEditTab.h"
#include <functional>
#include "utility/FileUtils.h"
#include <QFile>
#include <QMessageBox>

SaveEditTab::SaveEditTab(APIClient &client) : NetEditTab(client, RENDER)
{
}

/** PUBLIC **/

void SaveEditTab::loadAPI()
{
}

/** SLOTS **/

void SaveEditTab::onUpdateProjectDestinationPath(QString const &path)
{
    if (!_config) {
        throw std::runtime_error("SaveEditTab::onExport : null config.");
    }
    this->_config->destPath = path;
}

void SaveEditTab::onExport()
{
    vector<OCRPage> const &pages = this->getPages();

    if (!_config) {
        throw std::runtime_error("SaveEditTab::onExport : null config.");
    }
//    for (OCRPage const &page : pages) {
//        if (FileUtils::checkImgFilePath(page.renderImagePath)) {
//            if (!FileUtils::copyToDestination(page.renderImagePath, _config->destPath)) {
//                std::cerr << "SaveEditTab::onExport : Fail to copy file " << page.renderImagePath.toStdString()
//                          << " to directory " << _config->destPath.toStdString() << std::endl;
//            }
//        }
//    }
    //QMessageBox::information(this, "Project export", "operation completed");
}

/** PRIVATE **/

