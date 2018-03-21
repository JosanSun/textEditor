#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QTextEdit>

class TextEditor : public QTextEdit
{
    Q_OBJECT
public:
    TextEditor(QWidget* parent = nullptr);
    bool readFile(const QString& fileName);
    bool writeFile(const QString& fileName);

public slots:
    void deleteText();
    void find();

};

#endif // MAINWINDOW_H
