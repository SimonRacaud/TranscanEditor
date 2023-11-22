#ifndef ATEXTEDITTAB_H
#define ATEXTEDITTAB_H

#include "widget/editor/box/TextEditBox.h"
#include "NetEditTab.h"

/**
 * @brief The ATextEditTab class : Used by any viewer managing TextEditBoxes
 */
class ATextEditTab : public NetEditTab
{
    Q_OBJECT
public:
    ATextEditTab(APIClient &client, ImageMode modeImg, RectMode mode, bool autoReload);

    virtual void setPages(std::vector<OCRPage> const &pages) override;

    void createAreaRectAtCoord(QPointF const &coord);

    void removeRect();

    std::vector<BlockCluster> getClusters() const;

    /**
     * @brief getPage Convert local TextEditBox list to BlockClusters
     * @param index Index of the page to export
     * @return
     */
    virtual OCRPage getPage(size_t index) override;

    /**
     * @brief clearRects : Remove every rects from the view
     */
    void clearRects();

    virtual void loadAPI() override;

    virtual void unload() override;

protected:
    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    /**
     * @brief doubleClickEvent Mouse event on Graphic View
     * @param event
     */
    void doubleClickEvent(QMouseEvent *event);

    void createAreaRect(BlockCluster const& data, int pagePosY);

    void setPagesEditAreas(std::vector<OCRPage> const &pages);

    TextEditBox &getRectFromId(QUuid const &id);

    void foreachTextEditBox(std::function<void(TextEditBox &)> const &);

protected slots:
    void changeFocus(bool focused, TextEditBox *rect);

signals:
    /**
     * @brief sigRectFocusChanged
     *  - focused on click
     *  - lose focus when another rect is focussed or on click on the EditArea
     * @param rect
     */
    void sigRectFocusChanged(TextEditBox *rect);

private:
    RectMode _mode;

protected:
    QUuid _selectedItemId;
    bool _focusedRectLostFocus{false};

};

#endif // ATEXTEDITTAB_H
