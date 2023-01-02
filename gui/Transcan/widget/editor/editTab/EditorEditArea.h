#ifndef EDITOREDITAREA_H
#define EDITOREDITAREA_H

#include "ATextEditArea.h"

class EditorEditArea : public ATextEditArea
{
    Q_OBJECT
public:
    EditorEditArea(APIClient &client);

    virtual void loadAPI() override;

public slots:
    void updateAllClusterStyle(RenderConfig const &style);
    void updateSelectedClusterStyle(RenderConfig const &style);

private:
    void onRectFocusChange(EditAreaRect *rect);

signals:
    void sigFocusEditRect(RenderConfig const &style);
    void sigUnfocusEditRect();
};

#endif // EDITOREDITAREA_H
