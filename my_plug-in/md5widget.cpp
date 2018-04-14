#include <QCryptographicHash>

#include "md5widget.h"

MD5Widget::MD5Widget(QWidget *parent) : QDialog(parent)
{
    if (objectName().isEmpty())
    {
        setObjectName(QStringLiteral("md5Widget"));
    }
    // 设置固定窗口大小
    setFixedSize(542, 375);
    mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(6);
    mainLayout->setContentsMargins(11, 11, 11, 11);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    widget = new QWidget(this);
    widget->setObjectName(QStringLiteral("widget"));
    gridLayout = new QGridLayout(widget);
    gridLayout->setSpacing(6);
    gridLayout->setContentsMargins(11, 11, 11, 11);
    gridLayout->setObjectName(QStringLiteral("gridLayout"));
    inputBox = new QPlainTextEdit(widget);
    inputBox->setObjectName(QStringLiteral("inputBox"));
    inputBox->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    inputBox->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    inputBox->setLineWrapMode(QPlainTextEdit::NoWrap);
    gridLayout->addWidget(inputBox, 0, 0, 1, 1);
    splitEachRowBox = new QCheckBox(widget);
    splitEachRowBox->setObjectName(QStringLiteral("splitEachRowBox"));
    gridLayout->addWidget(splitEachRowBox, 0, 1, 1, 1);
    outputBox = new QTextBrowser(widget);
    outputBox->setObjectName(QStringLiteral("outputBox"));
    outputBox->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    outputBox->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    outputBox->setLineWrapMode(QTextBrowser::NoWrap);
    outputBox->setTextInteractionFlags(Qt::TextSelectableByMouse);
    outputBox->setAcceptDrops(true);
    // outputBox->setFrameShadow(QFrame::Sunken);
    // outputBox->setReadOnly(true);                // Q: 如何让输出窗口变得暗淡？
    // outputBox->setForegroundRole(QPalette::NoRole);
    outputBox->setEnabled(false);                   // 让输出窗口变灰
    gridLayout->addWidget(outputBox, 1, 0, 1, 1);
    copyButton = new QPushButton(widget);
    copyButton->setObjectName(QStringLiteral("copyButton"));
    gridLayout->addWidget(copyButton, 1, 1, 1, 1);

    widget_2 = new QWidget(this);
    widget_2->setObjectName(QStringLiteral("widget_2"));
    horizontalLayout = new QHBoxLayout(widget_2);
    horizontalLayout->setSpacing(6);
    horizontalLayout->setContentsMargins(11, 11, 11, 11);
    horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
    horizontalSpacer = new QSpacerItem(156, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    horizontalLayout->addItem(horizontalSpacer);
    closeButton = new QPushButton(widget_2);
    closeButton->setObjectName(QStringLiteral("closeButton"));
    horizontalLayout->addWidget(closeButton);
    horizontalSpacer_2 = new QSpacerItem(156, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    horizontalLayout->addItem(horizontalSpacer_2);

    mainLayout->addWidget(widget);
    mainLayout->addWidget(widget_2);
    this->setLayout(mainLayout);

    // 去除最大最小化按钮
    setWindowFlags(windowFlags() & ~Qt::WindowMinMaxButtonsHint);
    this->setWindowTitle(tr("生成MD5值"));
    copyButton->setText(tr("复制到剪贴板"));
    closeButton->setText(tr("关闭"));
    splitEachRowBox->setText(tr("按行分别生成"));


    connect(closeButton, &QPushButton::clicked, this, &MD5Widget::close);
    connect(inputBox, &QPlainTextEdit::textChanged, this, &MD5Widget::generateMD5);
    connect(splitEachRowBox, &QCheckBox::stateChanged, this, &MD5Widget::generateMD5);
    connect(copyButton, &QPushButton::clicked, this, &MD5Widget::copyOutput);
}

void MD5Widget::generateMD5()
{
    QString md5;
    QString pwd;
    QByteArray ba, bb;
    QCryptographicHash md(QCryptographicHash::Md5);


    if(!splitEachRowBox->isChecked())
    {
        pwd = inputBox->toPlainText();
        ba.append(pwd);
        md.addData(ba);
        bb = md.result();
        md5.append(bb.toHex());
        outputBox->setPlainText(md5);
    }
    else
    {
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
