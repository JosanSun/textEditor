#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QTextEdit>

class TextEditor : public QTextEdit
{
    Q_OBJECT
signals:
    void overwriteModeChanged();

public:
    TextEditor(QWidget* parent = nullptr);
    bool readFile(const QString& fileName);
    bool writeFile(const QString& fileName);

protected:
    void keyPressEvent(QKeyEvent* ev);

public slots:
    void find();
};

#endif // MAINWINDOW_H
