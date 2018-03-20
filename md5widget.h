#ifndef MD5WIDGET_H
#define MD5WIDGET_H

#include <QWidget>
#include <QVariant>
#include <QAction>
#include <QApplication>
#include <QButtonGroup>
#include <QCheckBox>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QTextBrowser>

class MD5Widget : public QWidget
{
    Q_OBJECT
public:
    explicit MD5Widget(QWidget *parent = nullptr);

private:
    QGridLayout *gridLayout;
    QPlainTextEdit *inputBox;
    QTextBrowser *outputBox;
    QPushButton *copyButton;
    QPushButton *closeButton;
    QHBoxLayout *splitEachRowLayout;
    QCheckBox *splitEachRowBox;
    QLabel *splitEachRowLabel;

signals:

public slots:

private slots:
    void exitMD5();
    void generateMD5();
    void copyOutput();
};

#endif // MD5WIDGET_H
