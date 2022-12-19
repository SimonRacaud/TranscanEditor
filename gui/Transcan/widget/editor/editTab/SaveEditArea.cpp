#include "SaveEditArea.h"

SaveEditArea::SaveEditArea()
{

}

/** PUBLIC **/

void SaveEditArea::load(std::vector<OCRPage> const &pages)
{
    // TODO
    // API call to render each page
}

void SaveEditArea::load(OCRPage const &page)
{
    this->updatePage(page);
}

void SaveEditArea::unload()
{
    // TODO
    // Stop any API call
}

/** PRIVATE **/

