#include "SaveEditArea.h"

SaveEditArea::SaveEditArea()
{

}

/** PUBLIC **/

void SaveEditArea::load(std::vector<OCRPage> const &pages)
{
    // TODO : load save tab
    // API call to render each page
}

void SaveEditArea::loadPage(OCRPage const &page)
{
    this->updatePage(page);
}

void SaveEditArea::unload()
{
    // TODO : unload save tab
    // Stop any API call
}

/** PRIVATE **/

