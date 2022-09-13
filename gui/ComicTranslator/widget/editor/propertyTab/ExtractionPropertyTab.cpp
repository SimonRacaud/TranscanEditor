#include "ExtractionPropertyTab.h"

ExtractionPropertyTab::ExtractionPropertyTab(QWidget *parent) : APropertyTab(parent)
{
    this->fillHelp();
}

/** Internal functions **/

void ExtractionPropertyTab::fillHelp()
{
    this->_help->setText(
                "Extraction:\n"
                "- Create, resize and delete text boxes\n"
                "- Fix the extracted text\n\n"

                "Controls:\n"
                "- Double click to create a new text box\n"
                "- Select the edges to resize a box\n"
                "- Double click on a box to edit its context\n"
                );
}

void ExtractionPropertyTab::initProperties()
{

}
