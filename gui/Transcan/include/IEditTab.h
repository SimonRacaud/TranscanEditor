#ifndef IEDITORAREA_H
#define IEDITORAREA_H

#include <QPoint>

#include "model/Page.h"

class IEditTab {
public:
    virtual ~IEditTab() = default;

    /**
     * @brief load : reload display with pages
     *  May launch API call.
     * @param pages : pages to display
     */
    virtual void load(std::vector<OCRPage> const &pages = {}) = 0;

    /**
     * @brief loadAPI : send API request for auto processing
     */
    virtual void loadAPI() = 0;

    /**
     * @brief load : reload display of one page
     * @param page
     */
    virtual void loadPage(OCRPage const &page) = 0;

    /**
     * @brief unload : Called when quitting the tab
     * Abort current API request (is needed)
     */
    virtual void unload() = 0;
};

#endif // IEDITORAREA_H
