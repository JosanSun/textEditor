#ifndef MD5WIDGET_H
#define MD5WIDGET_H

#include <QWidget>
#include <QDialog>
#include <QApplication>
#include <QCheckBox>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QTextBrowser>

class MD5Widget : public QDialog
{
    Q_OBJECT
public:
    explicit MD5Widget(QWidget *parent = nullptr);

private:
    QVBoxLayout *mainLayout;
    QWidget *widget;
    QGridLayout *gridLayout;
    QPlainTextEdit *inputBox;
    QCheckBox *splitEachRowBox;
    QTextBrowser *outputBox;
    QPushButton *copyButton;
    QWidget *widget_2;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *closeButton;
    QSpacerItem *horizontalSpacer_2;

private slots:
    void generateMD5();
    void copyOutput();
};

#endif // MD5WIDGET_H
