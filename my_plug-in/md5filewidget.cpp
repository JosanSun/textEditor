#include <QCryptographicHash>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QFileInfo>

#include "md5filewidget.h"
#include "ui_md5filewidget.h"
#include "my_plug-in/myheaders.h"

MD5FileWidget::MD5FileWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MD5FileWidget)
{
    ui->setupUi(this);

    // 去除最大最小化按钮
    setWindowFlags(windowFlags() & ~Qt::WindowMinMaxButtonsHint);
    setFixedSize(542, 375);
    this->setWindowTitle(tr("从文件生成MD5值"));


    connect(ui->closeButton, &QPushButton::clicked, this, &MD5FileWidget::close);
    connect(ui->fileButton, &QPushButton::clicked, this, &MD5FileWidget::generateMD5);
    connect(ui->copyButton, &QPushButton::clicked, this, &MD5FileWidget::copyOutput);
}

MD5FileWidget::~MD5FileWidget()
{
    delete ui;
}

void MD5FileWidget::generateMD5()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("打开"), ".",
                                                    tr("All types (*.*);;"
                                                       "Normal text file (*.txt);;"
                                                       "C++ source file (*.h;*.hpp;*.hxx;*.cpp;*.cxx;*.cc)"));
    if(filePath.isEmpty())
    {
        qcout << "没有正确打开文件。 md5filewidget.cpp";
        return ;
    }
    ui->inputBox->setPlainText(filePath);

    QString md5;
    QCryptographicHash md(QCryptographicHash::Md5);

    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this, tr("TextEditor"),
                             tr("Cannot read file %1.\n%2.")
                             .arg(file.fileName())
                             .arg(file.errorString()));        // file.fileName()还是带路径的文件名
        return ;
    }

#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif

    const QByteArray fileContent = file.readAll();
    qcout << fileContent;

#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif
    md.addData(fileContent);
    md5.append(md.result().toHex());
    QFileInfo fileInfo(filePath);
    ui->outputBox->setPlainText(tr("%1  %2").arg(md5).arg(fileInfo.fileName()));
}

void MD5FileWidget::copyOutput()
{
    ui->outputBox->selectAll();
    ui->outputBox->copy();
}
