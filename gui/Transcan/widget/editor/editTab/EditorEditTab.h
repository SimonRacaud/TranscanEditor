#ifndef EDITOREDITTAB_H
#define EDITOREDITTAB_H

#include "ATextEditTab.h"

class EditorEditTab : public ATextEditTab
{
    Q_OBJECT
public:
    EditorEditTab(APIClient &client);

    virtual void loadAPI() override;

    virtual void unload() override;

public slots:
    void updateAllClusterStyle(RenderConfig const &style);
    void updateSelectedClusterStyle(RenderConfig const &style);

private:
    void onRectFocusChange(TextEditBox *rect);

    void renderSceneToFiles();

    /**
     * @brief enableEditBoxesRect : Show or Hide editBoxes borders
     * @param enable
     */
    void enableEditBoxesRect(bool enable = true);

signals:
    void sigFocusEditRect(RenderConfig const &style);
    void sigUnfocusEditRect();
};

#endif // EDITOREDITTAB_H
