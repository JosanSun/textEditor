#ifndef MD5FILEWIDGET_H
#define MD5FILEWIDGET_H

#include <QWidget>

namespace Ui {
class MD5FileWidget;
}

class MD5FileWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MD5FileWidget(QWidget *parent = nullptr);
    ~MD5FileWidget();

private:
    Ui::MD5FileWidget *ui;

private slots:
    void generateMD5();
    void copyOutput();
};

#endif // MD5FILEWIDGET_H
