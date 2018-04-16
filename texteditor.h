#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QTextEdit>

enum EndOfLineText
{
    Default1, Windows1, Unix1, Mac1
};

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
    EndOfLineText getLineFormat();

protected:
    void keyPressEvent(QKeyEvent* ev);

public slots:
    void findNext(const QString& str, Qt::CaseSensitivity cs);
    void findPrevious(const QString& str, Qt::CaseSensitivity cs);

private slots:
    void signalTextModification(bool changed);


private:
    EndOfLineText lineFormat = EndOfLineText::Default1;
};

#endif // MAINWINDOW_H
