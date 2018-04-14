#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QTextEdit>

class TextEditor : public QTextEdit
{
    Q_OBJECT
signals:
    void overwriteModeChanged();
    void modificationChanged(bool changed);

public:
    TextEditor(QWidget* parent = nullptr);
    bool readFile(const QString& fileName);
    bool writeFile(const QString& fileName);

protected:
    void keyPressEvent(QKeyEvent* ev);
    void dragEnterEvent(QDragEnterEvent *e);
    void dropEvent(QDropEvent *e);

public slots:
    void findNext(const QString& str, Qt::CaseSensitivity cs);
    void findPrevious(const QString& str, Qt::CaseSensitivity cs);

private slots:
    void signalTextModification(bool changed);

};

#endif // MAINWINDOW_H
