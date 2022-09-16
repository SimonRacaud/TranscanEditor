#ifndef IEDITORAREA_H
#define IEDITORAREA_H

#include <QPoint>

#include "include/model/OCRPage.h"

class ITextEditArea {
public:
    virtual ~ITextEditArea() = default;

    /**
     * @brief createAreaRectAtCoord : Create a new AreaRect at the coordinates coord
     * @param coord
     */
    virtual void createAreaRectAtCoord(QPoint const &coord) = 0;

    /**
     * @brief removeRect : remove currently focused rect
     */
    virtual void removeRect() = 0;
};

#endif // IEDITORAREA_H
