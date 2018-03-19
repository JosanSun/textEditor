#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QTextEdit>

class TextEditor : public QTextEdit
{
    Q_OBJECT
signals:
    void modifiedTextEditor();

private slots:
    void textEditorChanged();

public:
    TextEditor(QWidget* parent = nullptr);
    bool readFile(const QString& fileName);
    bool writeFile(const QString& fileName);



};

#endif // MAINWINDOW_H
