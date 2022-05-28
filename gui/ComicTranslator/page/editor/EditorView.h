#ifndef EDITORVIEW_H
#define EDITORVIEW_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>

class EditorView : public QWidget
{
    Q_OBJECT
public:
    explicit EditorView(QWidget *parent = nullptr);

private:
    void setupHeader();

protected:
    QVBoxLayout *_rootLayout;
    // Header
    QPushButton *_showSourceButton;
    QPushButton *_extractButton;
    QPushButton *_cleanButton;
    QPushButton *_editButton;
    QPushButton *_saveButton;
    QPushButton *_exitButton;
};

#endif // EDITORVIEW_H
