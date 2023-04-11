#include "SaveEditArea.h"
#include <functional>
#include "utils/FileUtils.h"
#include <QFile>
#include <QMessageBox>

SaveEditArea::SaveEditArea(APIClient &client) : NetEditTab(client, RENDER)
{
}

/** PUBLIC **/

void SaveEditArea::loadAPI()
{
    for (OCRPage const &page : _pages) {
        this->loadPage(page);
    }
}

/** SLOTS **/

void SaveEditArea::onUpdateProjectDestinationPath(QString const &path)
{
    if (!_config) {
        throw std::runtime_error("SaveEditArea::onExport : null config.");
    }
    this->_config->destPath = path;
}

void SaveEditArea::onExport()
{
    vector<OCRPage> const &pages = this->getPages();

    if (!_config) {
        throw std::runtime_error("SaveEditArea::onExport : null config.");
    }
    for (OCRPage const &page : pages) {
        if (FileUtils::checkImgFilePath(page.renderImagePath)) {
            if (!FileUtils::copyToDestination(page.renderImagePath, _config->destPath)) {
                std::cerr << "SaveEditArea::onExport : Fail to copy file " << page.renderImagePath.toStdString()
                          << " to directory " << _config->destPath.toStdString() << std::endl;
            }
        }
    }
    QMessageBox::information(this, "Project export", "operation completed");
}

/** PRIVATE **/

