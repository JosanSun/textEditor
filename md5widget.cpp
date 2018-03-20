#include <QCryptographicHash>

#include "md5widget.h"

MD5Widget::MD5Widget(QWidget *parent) : QWidget(parent)
{
    this->resize(400, 300);
    gridLayout = new QGridLayout(this);
    gridLayout->setSpacing(6);
    gridLayout->setContentsMargins(11, 11, 11, 11);
    gridLayout->setObjectName(QStringLiteral("gridLayout"));
    inputBox = new QPlainTextEdit(this);
    inputBox->setObjectName(QStringLiteral("inputBox"));
    inputBox->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    inputBox->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    inputBox->setLineWrapMode(QPlainTextEdit::NoWrap);

    gridLayout->addWidget(inputBox, 0, 0, 1, 1);

    outputBox = new QTextBrowser(this);
    outputBox->setObjectName(QStringLiteral("outputBox"));
    outputBox->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    outputBox->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    outputBox->setLineWrapMode(QTextBrowser::NoWrap);
    outputBox->setTextInteractionFlags(Qt::TextSelectableByMouse);
    outputBox->setAcceptDrops(true);

    gridLayout->addWidget(outputBox, 1, 0, 1, 1);

    copyButton = new QPushButton(this);
    copyButton->setObjectName(QStringLiteral("copyButton"));

    gridLayout->addWidget(copyButton, 1, 1, 1, 1);

    closeButton = new QPushButton(this);
    closeButton->setObjectName(QStringLiteral("closeButton"));
    QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(closeButton->sizePolicy().hasHeightForWidth());
    closeButton->setSizePolicy(sizePolicy);

    gridLayout->addWidget(closeButton, 2, 0, 1, 2);

    splitEachRowLayout = new QHBoxLayout();
    splitEachRowLayout->setSpacing(6);
    splitEachRowLayout->setObjectName(QStringLiteral("splitEachRowLayout"));
    splitEachRowBox = new QCheckBox(this);
    splitEachRowBox->setObjectName(QStringLiteral("splitEachRowBox"));

    splitEachRowLayout->addWidget(splitEachRowBox);

    splitEachRowLabel = new QLabel(this);
    splitEachRowLabel->setObjectName(QStringLiteral("splitEachRowLabel"));
    splitEachRowLabel->setWordWrap(true);

    splitEachRowLayout->addWidget(splitEachRowLabel);


    gridLayout->addLayout(splitEachRowLayout, 0, 1, 1, 1);

    this->setWindowTitle(tr("MD5校验工具"));
    copyButton->setText(tr("复制结果"));
    closeButton->setText(tr("关闭"));
    splitEachRowBox->setText(QString());
    splitEachRowLabel->setText(tr("将每一行单独视为一个字符串"));

    connect(closeButton, &QPushButton::clicked, this, &MD5Widget::exitMD5);
    connect(inputBox, &QPlainTextEdit::textChanged, this, &MD5Widget::generateMD5);
    connect(splitEachRowBox, &QCheckBox::stateChanged, this, &MD5Widget::generateMD5);
    connect(copyButton, &QPushButton::clicked, this, &MD5Widget::copyOutput);
}

void MD5Widget::exitMD5()
{
    close();
}

void MD5Widget::generateMD5()
{
    QString md5;
    QString pwd;
    QByteArray ba, bb;
    QCryptographicHash md(QCryptographicHash::Md5);


    if(!splitEachRowBox->isChecked()){
        pwd = inputBox->toPlainText();
        ba.append(pwd);
        md.addData(ba);
        bb = md.result();
        md5.append(bb.toHex());
        outputBox->setPlainText(md5);
    }
    else{
        pwd = inputBox->toPlainText();
        QStringList list = pwd.split("\n");
        foreach(QString str, list)
        {
            ba.clear();
            ba.append(str);
            QCryptographicHash md(QCryptographicHash::Md5);
            md.addData(ba);
            bb = md.result();
            md5.append(bb.toHex() + "\n");
            outputBox->setPlainText(md5);
        }
    }
}

void MD5Widget::copyOutput()
{
    outputBox->selectAll();
    outputBox->copy();
}
