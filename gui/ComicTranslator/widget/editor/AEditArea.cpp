#include "AEditArea.h"
#include <QHBoxLayout>

AEditArea::AEditArea(QWidget *parent)
    : QScrollArea{parent}
{
    // Set background color
    QPalette pal = QPalette(QColor(EDITOR_EDITA_BG));
    this->setAutoFillBackground(true);
    this->setPalette(pal);
    //
    QHBoxLayout *rootLayout = new QHBoxLayout;
    // TODO : image lists
    this->setLayout(rootLayout);
    this->setStyleSheet("QScrollArea { border: 0; }");
}
