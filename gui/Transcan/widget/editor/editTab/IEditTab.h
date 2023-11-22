#ifndef IEDITORAREA_H
#define IEDITORAREA_H

#include <QPoint>

#include "model/Page.h"

class IEditTab {
public:
    virtual ~IEditTab() = default;

    /**
     * @brief load : reload display with pages
     *  May start an API call.
     * @param pages : pages to display
     */
    virtual void load(std::vector<OCRPage> const &pages = {}) = 0;

    /**
     * @brief loadAPI : send API request (CORE module)
     */
    virtual void loadAPI() = 0;

    /**
     * @brief load : reload display of one page
     * @param page
     */
    virtual void loadPage(OCRPage const &page) = 0;

    /**
     * @brief unload : Called when unfocusing the tab
     * Abort current API request (if needed)
     */
    virtual void unload() = 0;
};

#endif // IEDITORAREA_H
