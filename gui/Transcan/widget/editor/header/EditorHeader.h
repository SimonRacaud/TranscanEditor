#ifndef EDITORHEADER_H
#define EDITORHEADER_H

#include <QWidget>
#include "include/environment.h"
#include <QPushButton>
#include <QtSvgWidgets/QSvgWidget>

class EditorHeader : public QWidget
{
    Q_OBJECT
public:
    explicit EditorHeader(QWidget *parent = nullptr);

    /**
     * @brief Disable current tab button
     */
    void setSelectionTabHeader(EditorTab currentTab);

    void setShowSourceButtonState(bool enable);
signals:
    void sigChangeTab(EditorTab tab);
    void sigShowPageSourceTab(bool enable);

private slots:
    void showSourceButtonClickedSlot(bool checked);
    void extractButtonClickedSlot(bool checked);
    void cleanButtonClickedSlot(bool checked);
    void editButtonClickedSlot(bool checked);
    void saveButtonClickedSlot(bool checked);
    void exitButtonClickedSlot(bool checked);

private:
    QPushButton *_showSourceButton;
    QPushButton *_extractButton;
    QPushButton *_cleanButton;
    QPushButton *_editButton;
    QPushButton *_saveButton;
    QPushButton *_exitButton;
    QSvgWidget *_separator1;
    QSvgWidget *_separator2;
    QSvgWidget *_separator3;
};

#endif // EDITORHEADER_H
