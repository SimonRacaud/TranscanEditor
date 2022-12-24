#ifndef EDITOREDITAREA_H
#define EDITOREDITAREA_H

#include "ATextEditArea.h"

class EditorEditArea : public ATextEditArea
{
    Q_OBJECT
public:
    EditorEditArea(APIClient &client);

    virtual void loadAPI() override;
};

#endif // EDITOREDITAREA_H
