#ifndef MD5WIDGET_H
#define MD5WIDGET_H

#include <QWidget>
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>
#include <QtWidgets/QTextBrowser>

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
