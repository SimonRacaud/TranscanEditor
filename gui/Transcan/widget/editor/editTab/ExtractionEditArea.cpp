#include "ExtractionEditArea.h"
#include "model/Page.h"
#include <vector>

using namespace std;

ExtractionEditArea::ExtractionEditArea(APIClient &client) : ATextEditArea(client, SOURCE, RectMode::EDIT_SENT)
{
}

void ExtractionEditArea::load(std::vector<OCRPage> const &pages)
{
    this->setPages(pages); // Apply pages to view
}

void ExtractionEditArea::loadPage(OCRPage const &page)
{
    this->setLoadingState(false);
    // Replace page and Reload view
    this->updatePage(page);
}

