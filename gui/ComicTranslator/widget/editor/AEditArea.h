#ifndef AEDITAREA_H
#define AEDITAREA_H

#include <QWidget>
#include <QScrollArea>
#include "include/model/OCRPage.h"
#include "include/environment.h"

class AEditArea : public QScrollArea
{
    Q_OBJECT
public:
    explicit AEditArea(QWidget *parent = nullptr);

    //virtual void refresh(std::vector<OCRPage> &pages) = 0;
signals:


private:

};

#endif // AEDITAREA_H
