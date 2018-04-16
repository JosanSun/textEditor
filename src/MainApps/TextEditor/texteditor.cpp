#include <QMessageBox>
#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QKeyEvent>
#include <QTextCodec>
#include <QTime>
#include <QMimeData>

#include "my_plug-in/myheaders.h"
#include "texteditor.h"

TextEditor::TextEditor(QWidget* parent):QTextEdit(parent)
{
    clear();


    // 打开一个一行约有500KB字节的文件，所需的时间对比  测试文件在：testFile/longline.txt
    // QTextOption::WrapAtWordBoundaryOrAnywhere 是默认格式        // 40s
    // setWordWrapMode(QTextOption::NoWrap);                      // 177ms
    // setWordWrapMode(QTextOption::ManualWrap);                  // 159ms   QTextOption::ManualWrap  Same as QTextOption::NoWrap
    setWordWrapMode(QTextOption::WrapAnywhere);                // 1189ms
    // setWordWrapMode(QTextOption::WordWrap);                    // 180ms


    connect(this->document(), &QTextDocument::modificationChanged,
            this, &TextEditor::signalTextModification);
}

bool TextEditor::readFile(const QString &fileName)
{
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, tr("TextEditor"),
                             tr("Cannot read file %1.\n%2.")
                             .arg(file.fileName())
                             .arg(file.errorString()));
        return false;
    }

    clear();

    //QTextStream inFile(&file);
#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    QTime t1, t2, t3, t4;
    t1 = QTime::currentTime();


// 这种方法，可以按行读取内容。对于比较大的文件，eg:1MB文件，读取显示的时候，显得比较慢。
//    while(!file.atEnd())
//    {
//        const QByteArray data = file.readLine();
//        QTextCodec::ConverterState state;
//        QTextCodec *codec = QTextCodec::codecForName("UTF-8");
//        const QString text = codec->toUnicode(data.constData(), data.size(), &state);
//        if (state.invalidChars > 0)
//        {
//            // Not a UTF-8 text - using system default locale
//            QTextCodec * codec1 = QTextCodec::codecForLocale();
//            // qcout << codec1->name();
//            if (!codec1)
//                return false;
//            // qcout << "invalidChars > 0";
//            append(codec1->toUnicode(data));
//        }
//        else
//        {
//            //qcout << "invalidChars = 0";
//            append(text);
//        }
//    }
//    t2 = QTime::currentTime();
//    qcout << t1.msecsTo(t2);




    // 测试结果：
    // 对于8MB的文件，需要6759ms左右的时间打开
    // 对于13.5MB的文件，需要11624ms左右的时间
    // 打开20MB的文件失败

    const QByteArray data = file.readAll();
    t2 = QTime::currentTime();
    qcout << t1.msecsTo(t2);
    QTextCodec::ConverterState state;
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    const QString text = codec->toUnicode(data.constData(), data.size(), &state);
    //qcout << text;
    t3 = QTime::currentTime();
    qcout << t2.msecsTo(t3);
    if (state.invalidChars > 0)
    {
        // Not a UTF-8 text - using system default locale
        QTextCodec * codec1 = QTextCodec::codecForLocale();
        qcout << codec1->name();
        if (!codec1)
            return false;
        qcout << "invalidChars > 0";
        setPlainText(codec1->toUnicode(data));
    }
    else
    {
        qcout << "invalidChars = 0";
        setPlainText(text);
    }
    t4 = QTime::currentTime();
    qcout << t3.msecsTo(t4);
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif
    return true;
}

bool TextEditor::writeFile(const QString &fileName)
{
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, tr("TextEditor"),
                             tr("Cannot write file %1.\n%2")
                             .arg(file.fileName())
                             .arg(file.errorString()));
        return false;
    }

    QTextStream out(&file);
    out.setCodec("UTF-8");
#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif

    out << this -> toPlainText();

#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif
    return true;
}

void TextEditor::keyPressEvent(QKeyEvent *ev)
{
    if( Qt::Key_Insert == ev->key())
    {
        // 置反当前写入模式
        setOverwriteMode(!overwriteMode());
        emit overwriteModeChanged();
        return ;
    }
    QTextEdit::keyPressEvent(ev);
}

void TextEditor::findNext(const QString &str, Qt::CaseSensitivity cs)
{
    QTextDocument::FindFlags flag;
    flag = ((cs == Qt::CaseSensitive)? QTextDocument::FindCaseSensitively : flag);

    if(!find(str, flag))
    {
        QMessageBox::warning(this, tr("查找失败"),
                             tr("没有查找到: %1 .").arg(str), QMessageBox::Ok,
                             QMessageBox::NoButton);
    }
}

void TextEditor::findPrevious(const QString &str, Qt::CaseSensitivity cs)
{
    QTextDocument::FindFlags flag = QTextDocument::FindBackward;

    if(cs == Qt::CaseSensitive)
    {
        flag = flag | QTextDocument::FindCaseSensitively;
    }

    if(!find(str, flag))
    {
        QMessageBox::warning(this, tr("查找失败"),
                             tr("没有查找到%1.").arg(str), QMessageBox::Ok,
                             QMessageBox::NoButton);
    }
}

void TextEditor::signalTextModification(bool changed)
{
    emit modificationChanged(changed);
}
