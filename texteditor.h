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
    void findNext(const QString& str, Qt::CaseSensitivity cs);
    void findPrevious(const QString& str, Qt::CaseSensitivity cs);
};

#endif // MAINWINDOW_H
