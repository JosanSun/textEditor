#include <QMimeData>
#include <QClipboard>
#include <QAction>
#include <QLabel>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QFileDialog>
#include <QTextEdit>
#include <QMessageBox>
#include <QFileDialog>
#include <QCloseEvent>
#include <QSettings>
#include <QStringList>
#include <QDesktopWidget>
#include <QKeyEvent>
#include <QPrinter>
#include <QTabWidget>
// 增加打印功能支持

#include <QtPrintSupport/qtprintsupportglobal.h>

#include <QPrintDialog>
#include <QPrintPreviewDialog>

#ifdef _MSC_VER
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
#endif

#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    m_pTabWidget = new QTabWidget(this);
    m_pTextEdit = new TextEditor(this);

    this->setCentralWidget(m_pTabWidget);

    m_pTabWidget->setTabsClosable(true);
    m_pTabWidget->addTab(m_pTextEdit, tr("First"));


    m_pTextEdit->setFocus();
    m_pTextEdit->setAcceptDrops(false);
    setAcceptDrops(true);

    m_pTextEdit->setMarginType(0, QsciScintilla::NumberMargin);//设置编号为0的页边显示行号。
    m_pTextEdit->setMarginLineNumbers(0, true);//对该页边启用行号
    m_pTextEdit->setMarginWidth(0, 35);//设置页边宽度

    // 创建主界面
    createActions();
    createMenus();
    createContextMenu();
    createToolBars();
    createStatusBar();

    setWindowIcon(QIcon(":/images/fileIcon.png"));
    this->setCurrentFile("");
    //this->resize(500, 300);
    setUnifiedTitleAndToolBarOnMac(true);

    // 读取配置
    readSettings();

    // 更新actions()
    updateActions();

    connect(m_pTextEdit, SIGNAL(textChanged()) , this, SLOT(textEditorModified()));
    connect(m_pTextEdit, SIGNAL(cursorPositionChanged(int, int)), this, SLOT(showCursorPosition(int, int)));
    connect(m_pTextEdit, &TextEditor::overwriteModeChanged, [=](){
           if(m_pTextEdit->overwriteMode())
           {
               insertModeLabel->setText("OVR");
           }
           else
           {
               insertModeLabel->setText("INS");
           }
        });

    // connect(textEdit, &TextEditor::textChanged,
    //         this, &MainWindow::showSizeLines);
}

MainWindow::~MainWindow()
{

}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(okToContinue())
    {
        writeSettings();
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasUrls())
    {
        event->acceptProposedAction();
    }
}

void MainWindow::dropEvent(QDropEvent *event)
{
    QList<QUrl> urls = event->mimeData()->urls();
    if(urls.isEmpty())
    {
        return ;
    }

    QString filePath = urls.first().toLocalFile();
    if(filePath.isEmpty())
    {
        return ;
    }

    if(okToContinue())
    {
        loadFile(filePath);
    }
}

void MainWindow::newFile()
{
    if(okToContinue())
    {
        m_pTextEdit->clear();
        setCurrentFile("");
    }
}

void MainWindow::open()
{
    if(okToContinue())
    {
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open MainWindow"), ".",
                                                        tr("All types (*.*);;"
                                                           "Normal text file (*.txt);;"
                                                           "C++ source file (*.h;*.hpp;*.hxx;*.cpp;*.cxx;*.cc)"));
        if(!fileName.isEmpty())
        {
            loadFile(fileName);
        }
    }
    updateEndOfLineModeLabel();
}

bool MainWindow::save()
{
    if(curFile.isEmpty())
    {
        return saveAs();
    }
    else
    {
        return saveFile(curFile);
    }
}

bool MainWindow::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save TextEditor"), ".",
                                                    tr("Normal text file (*.txt);;"
                                                        "C++ source file (*.cpp)"));
    if(fileName.isEmpty())
    {
        return false;
    }
    return saveFile(fileName);
}

void MainWindow::printFile()
{

//    QPrinter printer(QPrinter::HighResolution);
//    QPrintDialog *dlg = new QPrintDialog(&printer, this);
//    if (textEdit->hasSelectedText())
//        dlg->addEnabledOption(QAbstractPrintDialog::PrintSelection);
//    dlg->setWindowTitle(tr("打印文档"));
//    if (dlg->exec() == QDialog::Accepted)
//        textEdit->print(&printer);
//    delete dlg;

//    // 部分系统可以简写为
//    QPrinter printer(QPrinter::HighResolution);
//    textEdit->print(&printer);

}

void MainWindow::printFilePreview()
{

    QPrinter printer(QPrinter::HighResolution);
    QPrintPreviewDialog preview(&printer, this);
    connect(&preview, &QPrintPreviewDialog::paintRequested, this, &MainWindow::printPreview);
    preview.exec();

}

void MainWindow::printPreview(QPrinter *printer)
{
//#ifdef QT_NO_PRINTER
//    Q_UNUSED(printer);
//#else
//    textEdit->print(printer);
//#endif
}

void MainWindow::printFilePDF()
{
//#ifndef QT_NO_PRINTER
//    QFileDialog fileDialog(this, tr("导出PDF文件..."));
//    fileDialog.setAcceptMode(QFileDialog::AcceptSave);
//    fileDialog.setMimeTypeFilters(QStringList("application/pdf"));
//    fileDialog.setDefaultSuffix("pdf");
//    if (fileDialog.exec() != QDialog::Accepted)
//        return;
//    QString fileName = fileDialog.selectedFiles().first();
//    QPrinter printer(QPrinter::HighResolution);
//    printer.setOutputFormat(QPrinter::PdfFormat);
//    printer.setOutputFileName(fileName);
//    textEdit->document()->print(&printer);
//    statusBar()->showMessage(tr("Exported \"%1\"")
//                             .arg(QDir::toNativeSeparators(fileName)));
//#endif
}

void MainWindow::setFullScreen()
{
    if(isFullScreen())
    {
        // menuBar()->setVisible(true);
        mainToolBar->show();
        statusBar()->show();
        this->setWindowState(Qt::WindowMaximized);
    }
    else
    {
        // menuBar()->setVisible(false);   // ??? menuBar()->hide可以隐藏，但是想显示却无法显示出来。不知为什么，跟statusBar()还有区别.
        mainToolBar->hide();
        statusBar()->hide();
        this->setWindowState(Qt::WindowFullScreen);
    }
}

void MainWindow::changeAutoLine()
{
    isAutoLine = autoLineAction->isChecked();
//    textEdit->setLineWrapMode(isAutoLine ? QTextEdit::WidgetWidth : QTextEdit::NoWrap);

    QSettings settings(QCoreApplication::organizationName(),
                       QCoreApplication::applicationName());
    settings.setValue("autoLineSwitch", QVariant(isAutoLine ? "1" : "0"));
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("关于 TextEditor"),
            tr("<h2>TextEditor v%1</h2>"
               "<p>Copyright &copy; 2018 SouthEast University."
               "<p>"
               "<p>项目原地址： https://github.com/JosanSun/textEditor"
               "<p>"
               "<p><b>TextEditor</b>是主要用来展示QAction, QMainWindow, QMenuBar, "
               "QStatusBar, QTableWidget, QToolBar以及其他Qt类的常用用法."
               "<p>本软件仅用来交流讨论，有任何好的建议欢迎联系QQ:1030460698.")
                       .arg(QCoreApplication::applicationVersion()));
}

void MainWindow::textEditorModified()
{
    saveAction->setEnabled(true);
    setWindowModified(m_pTextEdit->isModified());
}

void MainWindow::clipboardDataChanged()
{
    if (const QMimeData *md = QApplication::clipboard()->mimeData())
    {
        pasteAction->setEnabled(md->hasText());
    }
}

void MainWindow::openRecentFile()
{
    if(okToContinue())
    {
        // QObject *QObject::sender() const
        // Returns a pointer to the object that sent the signal
        QAction* action = qobject_cast<QAction *>(sender());
        if(action)
        {
            loadFile(action->data().toString());
        }
    }
    updateEndOfLineModeLabel();
}

void MainWindow::createActions()
{
    newAction = new QAction(tr("新建(&N)"), this);
    newAction->setIcon(QIcon(":/images/new.png"));
    newAction->setShortcut(QKeySequence::New);
    newAction->setToolTip(tr("新建一个文件"));
    newAction->setStatusTip(tr("新建一个文件"));
    connect(newAction, &QAction::triggered,
            this, &MainWindow::newFile);

    openAction = new QAction(tr("打开(&O)"), this);
    openAction->setIcon(QIcon(":/images/open.png"));
    openAction->setShortcut(QKeySequence::Open);
    openAction->setToolTip(tr("打开一个文件"));
    openAction->setStatusTip(tr("打开一个文件"));
    connect(openAction, &QAction::triggered,
            this, &MainWindow::open);

    saveAction = new QAction(tr("保存(&S)"), this);
    saveAction->setIcon(QIcon(":/images/save.png"));
    saveAction->setShortcut(QKeySequence::Save);
    saveAction->setToolTip("保存文件");
    saveAction->setStatusTip("保存文件");
    connect(saveAction, &QAction::triggered,
            this, &MainWindow::save);
    //对于textEdit来说，文本内容都是修改了的，会返回true
//    connect(textEdit, &TextEditor::modificationChanged,
//            saveAction, &QAction::setEnabled);
    saveAction->setEnabled(false);

    saveAsAction = new QAction(tr("另存为(&A)..."), this);
    saveAsAction->setShortcut(tr("Ctrl+Alt+S"));
    // saveAsAction->setShortcut(QKeySequence::SaveAs);  // other method
    saveAsAction->setToolTip(tr("将文件另存为..."));
    saveAsAction->setStatusTip(tr("将文件另存为..."));
    connect(saveAsAction, &QAction::triggered,
            this, &MainWindow::saveAs);

    printAction = new QAction(tr("打印..."), this);
    printAction->setIcon(QIcon(":/images/print.png"));
    printAction->setShortcut(QKeySequence::Print);
    printAction->setToolTip(tr("打印文档"));
    printAction->setStatusTip(tr("打印文档"));
    connect(printAction, &QAction::triggered,
            this, &MainWindow::printFile);

    printPreviewAction = new QAction(tr("打印预览..."), this);
    printPreviewAction->setToolTip(tr("打印预览"));
    printPreviewAction->setStatusTip(tr("打印预览"));
    connect(printPreviewAction, &QAction::triggered,
            this, &MainWindow::printFilePreview);

    exportToPDFAction = new QAction(tr("导出至PDF..."), this);
    exportToPDFAction->setIcon(QIcon(":/images/exportpdf.png"));
    exportToPDFAction->setToolTip(tr("导出至PDF"));
    exportToPDFAction->setStatusTip(tr("导出至PDF"));
    connect(exportToPDFAction, &QAction::triggered,
            this, &MainWindow::printFilePDF);

    // 文件菜单 --> 最近打开文件
    for(int i = 0; i < MaxRecentFiles; ++i)
    {
        recentFileActions[i] = new QAction(this);
        recentFileActions[i]->setVisible(false);
        connect(recentFileActions[i], &QAction::triggered,
                this, &MainWindow::openRecentFile);
    }

    exitAction = new QAction(tr("关闭"), this);
    exitAction->setShortcut(tr("Ctrl+Q"));
    exitAction->setToolTip(tr("退出程序"));
    exitAction->setStatusTip(tr("退出程序"));
    connect(exitAction, &QAction::triggered,
            this, &MainWindow::close);

    undoAction = new QAction(tr("撤销(&U)"), this);
    undoAction->setIcon(QIcon(":/images/undo.png"));
    undoAction->setShortcut(QKeySequence::Undo);
    undoAction->setToolTip(tr("撤销"));
    undoAction->setStatusTip(tr("撤销"));
    connect(undoAction, &QAction::triggered,
            m_pTextEdit, &TextEditor::undo);

    // connect(textEdit, &TextEditor::undoAvailable,
    //         this, [=](bool available)
    // {
    //     undoAction->setEnabled(available);
    //     if(false == available)
    //     {
    //         saveAction->setEnabled(false);
    //         setCurrentFile(curFile);
    //     }
    // });

//    undoAction->setEnabled(textEdit->document()->isUndoAvailable());
    // 设定初始状态，注意与【复制】，【剪切】等动作的区别
    // undoAction->setEnabled(false);

    redoAction = new QAction(tr("恢复(&R)"), this);
    redoAction->setIcon(QIcon(":/images/redo.png"));
    redoAction->setShortcut(QKeySequence::Redo);
    redoAction->setToolTip(tr("恢复"));
    redoAction->setStatusTip(tr("恢复"));
    connect(redoAction, &QAction::triggered,
            m_pTextEdit, &TextEditor::redo);
//    connect(textEdit, &TextEditor::redoAvailable,
//            redoAction, &QAction::setEnabled);
//    redoAction->setEnabled(textEdit->document()->isRedoAvailable());
    // redoAction->setEnabled(false);

    cutAction = new QAction(tr("剪切(&T)"), this);
    cutAction->setIcon(QIcon(":/images/cut.png"));
    cutAction->setShortcut(QKeySequence::Cut);
    cutAction->setToolTip(tr("剪切文本"));
    cutAction->setStatusTip(tr("剪切文本"));
    cutAction->setEnabled(false);
    connect(cutAction, &QAction::triggered,
            m_pTextEdit, &TextEditor::cut);

    copyAction = new QAction(tr("复制(&C)"), this);
    copyAction->setIcon(QIcon(":/images/copy.png"));
    copyAction->setShortcut(QKeySequence::Copy);
    copyAction->setToolTip(tr("复制文本"));
    copyAction->setStatusTip(tr("复制文本"));
    copyAction->setEnabled(false);
    connect(copyAction, &QAction::triggered,
            m_pTextEdit, &TextEditor::copy);
    connect(m_pTextEdit, SIGNAL(copyAvailable(bool)), this, SLOT(slotCopyAvailable(bool)));

    pasteAction = new QAction(tr("粘贴(&P)"), this);
    pasteAction->setIcon(QIcon(":/images/paste.png"));
    pasteAction->setShortcut(QKeySequence::Paste);
    pasteAction->setToolTip(tr("粘贴文本"));
    pasteAction->setStatusTip(tr("粘贴文本"));
    connect(pasteAction, &QAction::triggered,
            m_pTextEdit, &TextEditor::paste);
    // 绑定
    connect(QApplication::clipboard(), &QClipboard::dataChanged,
            this, &MainWindow::clipboardDataChanged);
    // 初始化
    if(const QMimeData* md = QApplication::clipboard()->mimeData())
    {
        pasteAction->setEnabled(md->hasText());
    }

    deleteAction = new QAction(tr("删除(&D)"), this);
    deleteAction->setShortcut(QKeySequence::Delete);
    deleteAction->setToolTip(tr("删除所选文本"));
    deleteAction->setStatusTip(tr("删除所选文本"));
    connect(deleteAction, &QAction::triggered,
            [=]()
            {
                QKeyEvent* event = new QKeyEvent(QEvent::KeyPress,
                                                 Qt::Key_Delete, Qt::NoModifier);
                QCoreApplication::postEvent(m_pTextEdit, event);
            });

    selectAllAction = new QAction(tr("全选(&L)"), this);
    selectAllAction->setShortcut(QKeySequence::SelectAll);
    selectAllAction->setToolTip(tr("全选文本"));
    selectAllAction->setStatusTip(tr("全选文本"));
    connect(selectAllAction, &QAction::triggered,
            m_pTextEdit, &TextEditor::selectAll);

    findAction = new QAction(tr("查找(&F)..."), this);
    findAction->setIcon(QIcon(":/images/find.png"));
    findAction->setShortcut(QKeySequence::Find);
    findAction->setToolTip(tr("查找文本"));
    findAction->setStatusTip(tr("查找文本"));
    connect(findAction, &QAction::triggered,
            this, &MainWindow::find);

    fullScreenAction = new QAction(tr("切换全屏模式"), this);
    fullScreenAction->setShortcut(QKeySequence::FullScreen);
    fullScreenAction->setToolTip(tr("全屏显示"));
    fullScreenAction->setStatusTip(tr("全屏显示"));
    connect(fullScreenAction, &QAction::triggered,
            this, &MainWindow::setFullScreen);

    autoLineAction = new QAction(tr("自动换行"), this);
    // 打开可选开关
    autoLineAction->setCheckable(true);
    autoLineAction->setChecked(isAutoLine);
    connect(autoLineAction, &QAction::triggered, this, &MainWindow::changeAutoLine);


    optionAction = new QAction(tr("首选项..."), this);
    optionAction->setToolTip(tr("配置程序"));
    optionAction->setStatusTip(tr("配置程序"));

    generateMD5Action = new QAction(tr("生成..."), this);
    generateMD5Action->setToolTip(tr("MD5校验生成工具"));
    generateMD5Action->setStatusTip(tr("MD5校验生成工具"));
    connect(generateMD5Action, &QAction::triggered, this, &MainWindow::MD5WidgetShow);

    generateMD5FileAction = new QAction(tr("从文件生成..."), this);
    generateMD5FileAction->setToolTip(tr("从文件中生成MD5值"));
    generateMD5FileAction->setStatusTip(tr("从文件中生成MD5值"));
    connect(generateMD5FileAction, &QAction::triggered, this, &MainWindow::MD5FileWidgetShow);

    updateAction = new QAction(tr("升级 TextEditor"), this);
    updateAction->setToolTip(tr("升级应用程序"));
    updateAction->setStatusTip(tr("升级应用程序"));
    connect(updateAction, &QAction::triggered,
            this, &MainWindow::updateApp);

    aboutAction = new QAction(tr("关于 TextEditor..."), this);
    aboutAction->setToolTip(tr("显示应用的相关信息"));
    aboutAction->setStatusTip(tr("显示应用的相关信息"));
    connect(aboutAction, &QAction::triggered,
            this, &MainWindow::about);
}

void MainWindow::createMenus()
{
    // 文件菜单
    fileMenu = menuBar()->addMenu(tr("文件(&F)"));
    fileMenu->addAction(newAction);
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);
    fileMenu->addAction(saveAsAction);
    fileMenu->addSeparator();
    fileMenu->addAction(printAction);
    fileMenu->addAction(printPreviewAction);
    fileMenu->addAction(exportToPDFAction);
    separatorAction = fileMenu->addSeparator();
    for(int i = 0; i < MaxRecentFiles; ++i)
    {
        fileMenu->addAction(recentFileActions[i]);
    }
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    // 编辑菜单
    editMenu = menuBar()->addMenu(tr("编辑(&E)"));
    editMenu->addAction(undoAction);
    editMenu->addAction(redoAction);
    editMenu->addSeparator();
    editMenu->addAction(cutAction);
    editMenu->addAction(copyAction);
    editMenu->addAction(pasteAction);
    editMenu->addAction(deleteAction);
    editMenu->addAction(selectAllAction);

    // 搜索菜单
    searchMenu = menuBar()->addMenu(tr("搜索(&S)"));
    searchMenu->addAction(findAction);

    // 视图菜单
    viewMenu = menuBar()->addMenu(tr("视图(&V)"));
    viewMenu->addAction(fullScreenAction);
    viewMenu->addSeparator();
    viewMenu->addAction(autoLineAction);

    // 设置菜单
    settingsMenu = menuBar()->addMenu(tr("设置(&T)"));
    settingsMenu->addAction(optionAction);

    // 工具菜单
    toolsMenu = menuBar()->addMenu(tr("工具(&O)"));
    md5Menu = new QMenu(tr("MD5"));
    toolsMenu->addMenu(md5Menu);
    md5Menu->addAction(generateMD5Action);
    md5Menu->addAction(generateMD5FileAction);

    // 帮助菜单
    helpMenu = menuBar()->addMenu(tr("帮助(&H)"));
    helpMenu->addAction(updateAction);
    helpMenu->addAction(aboutAction);
}

void MainWindow::createContextMenu()
{
    m_pTextEdit->addAction(copyAction);
    m_pTextEdit->addAction(cutAction);
    m_pTextEdit->addAction(pasteAction);
    m_pTextEdit->addAction(deleteAction);
    m_pTextEdit->addAction(selectAllAction);
    m_pTextEdit->setContextMenuPolicy(Qt::ActionsContextMenu);
}

void MainWindow::createToolBars()
{
    mainToolBar = addToolBar(tr("main"));
    mainToolBar->addAction(newAction);
    mainToolBar->addAction(openAction);
    mainToolBar->addAction(saveAction);
    mainToolBar->addAction(printAction);
    mainToolBar->addSeparator();
    mainToolBar->addAction(cutAction);
    mainToolBar->addAction(copyAction);
    mainToolBar->addAction(pasteAction);
    mainToolBar->addSeparator();
    mainToolBar->addAction(undoAction);
    mainToolBar->addAction(redoAction);
    mainToolBar->addSeparator();
    mainToolBar->addAction(findAction);
}

void MainWindow::createStatusBar()
{
    showLabel = new QLabel(tr("Normal text file"));
    showLabel->setAlignment(Qt::AlignRight);
    showLabel->setMinimumSize(showLabel->sizeHint());
    // showLabel->setMargin(2); // 设定边距
    showLabel->setIndent(50);   // 设定间距

    sizeLinesLabel = new QLabel(tr("length : %1  lines : %2"));
//    sizeLinesLabel = new QLabel(tr("length : %1  lines : %2")
//                                  .arg(textEdit->document()->characterCount() - 1)
//                                  .arg(textEdit->document()->lineCount()));
    sizeLinesLabel->setAlignment(Qt::AlignLeft);
    sizeLinesLabel->setMinimumSize(sizeLinesLabel->sizeHint());

    rowColumnLabel = new QLabel(tr("Ln : 1   Col : 1"));
    rowColumnLabel->setAlignment(Qt::AlignLeft);
    rowColumnLabel->setMinimumSize(rowColumnLabel->sizeHint());

    EndOfLineText endLineFormat = m_pTextEdit->getLineFormat();
    if(endLineFormat == EndOfLineText::Windows1)
    {
        lineFormat = EndOfLine::Windows;
        endOfLineModeLabel = new QLabel(tr("Windows (CR LF)"));
    }
    else if(endLineFormat == EndOfLineText::Unix1)
    {
        lineFormat = EndOfLine::Unix;
        endOfLineModeLabel = new QLabel(tr("Unix (LF)"));
    }
    else if(endLineFormat == EndOfLineText::Mac1)
    {
        lineFormat = EndOfLine::Mac;
        endOfLineModeLabel = new QLabel(tr("Macintosh (CR)"));
    }
    else
    {
        lineFormat = EndOfLine::Default;
        endOfLineModeLabel = new QLabel(tr("Unix (LF)"));
    }
    endOfLineModeLabel->setAlignment(Qt::AlignLeft);
    endOfLineModeLabel->setMinimumSize(endOfLineModeLabel->sizeHint());

    insertModeLabel = new QLabel(tr("INS"));
//    if(textEdit->overwriteMode())
//    {
//        insertModeLabel->setText("OVR");
//    }
//    else
//    {
//        insertModeLabel->setText("INS");
//    }
    insertModeLabel->setAlignment(Qt::AlignLeft);
    insertModeLabel->setMinimumSize(insertModeLabel->sizeHint());

    statusBar()->addWidget(showLabel);
    // rowcol | insertMode

    statusBar()->addPermanentWidget(sizeLinesLabel);
    statusBar()->addPermanentWidget(rowColumnLabel);
    statusBar()->addPermanentWidget(endOfLineModeLabel);
    statusBar()->addPermanentWidget(insertModeLabel);
}

void MainWindow::setCurrentFile(const QString &fileName)
{
    curFile = fileName;

    if(fileName.isEmpty())
    {
        curFile.append("new");
    }
    m_pTextEdit->setModified(false);

    setWindowModified(false);

    shownName = tr("new");

    if(!curFile.isEmpty())
    {
        curFile.replace('/', '\\');
        shownName = curFile;
        recentFiles.removeAll(curFile);
        recentFiles.prepend(curFile);   // 头插
        updateRecentFileActions();
    }

    // 更简单的方法改变默认星号的位置 [*]可以解析 [#或者其他字符]无法解析
    setWindowTitle(tr("[*]%1 - %2")
                   .arg(shownName)
                   .arg(tr("TextEditor")));
}

void MainWindow::readSettings()
{
    QSettings settings(QCoreApplication::organizationName(),
                       QCoreApplication::applicationName());

    const QByteArray geometry = settings.value("geometry", QByteArray()).toByteArray();
    if (geometry.isEmpty())
    {
        const QRect availableGeometry = QApplication::desktop()->availableGeometry(this);
        resize(availableGeometry.width() / 3, availableGeometry.height() / 2);
        move((availableGeometry.width() - width()) / 2,
             (availableGeometry.height() - height()) / 2);
    }
    else
    {
        restoreGeometry(geometry);
    }

    recentFiles = settings.value("recentFiles").toStringList();

    isAutoLine = settings.value("autoLineSwitch").toInt() == 1 ? true : false;

    updateRecentFileActions();
}

void MainWindow::writeSettings()
{
    QSettings settings(QCoreApplication::organizationName(),
                       QCoreApplication::applicationName());

    settings.setValue("geometry", saveGeometry());

    settings.setValue("recentFiles", recentFiles);

    settings.setValue("autoLineSwitch", autoLineAction->isChecked() ? 1 : 0);
}

bool MainWindow::okToContinue()
{
    if(isWindowModified())
    {
        QMessageBox box(QMessageBox::Question, tr("保存"),
                        tr("是否保存文件\"%1\"?").arg(curFile.isEmpty() ? shownName : curFile));

        // 去掉问号！  区分Icon与WindowIcon
        //box.setIcon(QMessageBox::NoIcon);
        box.setWindowIcon(QIcon(":/images/fileIcon.png"));
        box.setStandardButtons(QMessageBox::Ok | QMessageBox::No | QMessageBox::Cancel);
        box.setButtonText(QMessageBox::Ok, tr("是(&Y)"));
        box.setButtonText(QMessageBox::No, tr("否(&N)"));
        box.setButtonText(QMessageBox::Cancel, tr("取消"));
//        int res = QMessageBox::warning(this, tr("TextEditor"),
//                                       tr("The text has been modified.\n"
//                                          "Do you want to save your changes?"),
//                                       QMessageBox::Yes | QMessageBox::No
//                                       | QMessageBox::Cancel);
        int res = box.exec();
        if(res == QMessageBox::Ok)
        {
            return save();
        }
        else if(res == QMessageBox::Cancel)
        {
            return false;
        }
    }
    return true;
}

bool MainWindow::loadFile(const QString &fileName)
{
    QFileInfo fileInfo(fileName);
    // 如果文件的大小超过200MB，则显示无法打开。
    if(fileInfo.size() > 200 * 1024 * 1024)
    {
        QMessageBox::warning(this, tr("文件大小问题"),
                             tr("<p>文件太大, Text Editor打不开。"
                                "<p>程序员们正在加班加点解决问题^_^"));
        return false;
    }

//    if(!textEdit->readFile(fileName))
//    {
//        statusBar()->showMessage(tr("Loading canceled"), 2000);
//        return false;
//    }

    QFile file(fileName);
    if (!file.open(QFile::ReadOnly)) {
        QMessageBox::warning(this, tr("Application"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return false;
    }

    QTextStream in(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    m_pTextEdit->setText(in.readAll());
    QApplication::restoreOverrideCursor();

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File loaded"), 2000);
    saveAction->setEnabled(false);
    return true;
}

bool MainWindow::saveFile(const QString &fileName)
{
//    if(!textEdit->writeFile(fileName))
//    {
//        statusBar()->showMessage(tr("Saving canceled"), 2000);
//        return false;
//    }

    QFile file(fileName);
    if (!file.open(QFile::WriteOnly)) {
        QMessageBox::warning(this, tr("Application"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return false;
    }

    QTextStream out(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    out << m_pTextEdit->text();
    QApplication::restoreOverrideCursor();

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File saved"), 2000);
    saveAction->setEnabled(false);
    return true;
}

void MainWindow::updateRecentFileActions()
{
    QMutableStringListIterator i(recentFiles);
    while(i.hasNext())
    {
        if(!QFile::exists(i.next()))
        {
            i.remove();
        }
    }

    for(int j = 0; j < MaxRecentFiles; ++j)
    {
        if(j < recentFiles.count())
        {
            QString text = tr("%1: %2")
                    .arg(j + 1)
                    .arg(recentFiles[j]);
            recentFileActions[j]->setText(text);
            recentFileActions[j]->setData(recentFiles[j]);
            recentFileActions[j]->setVisible(true);
        }
        else
        {
            recentFileActions[j]->setVisible(false);
        }
    }
    separatorAction->setVisible(!recentFiles.isEmpty());

}

void MainWindow::MD5WidgetShow()
{
    md5Widget = new MD5Widget;
    md5Widget->setAttribute(Qt::WA_DeleteOnClose);
    md5Widget->exec();
//    md5Widget->show();
//    md5Widget->raise();
//    md5Widget->activateWindow();
}

void MainWindow::MD5FileWidgetShow()
{
    md5FileWidget = new MD5FileWidget;
    md5FileWidget->setAttribute(Qt::WA_DeleteOnClose);
    md5FileWidget->exec();
}

void MainWindow::find()
{
    if(!findDialog)
    {
        findDialog = new FindDialog(this);
        connect(findDialog, &FindDialog::findNext,
                m_pTextEdit, &TextEditor::findNext);
        connect(findDialog, &FindDialog::findPrevious,
                m_pTextEdit, &TextEditor::findPrevious);
    }

    findDialog->show();
    findDialog->raise();
    findDialog->activateWindow();
}

// TESTING
void MainWindow::updateApp()
{
//    QNetworkAccessManager networkManager;

//    QUrl url("https://api.github.com/repos/JosanSun/textEditor/releases/latest");
//    // The current all existing releases are all pre-release
//    // So the json file is unavailable
//    QNetworkRequest request;
//    request.setUrl(url);

//    currentReply = networkManager.get(request);  // GET
//    connect(&networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onResultUpdate(QNetworkReply*)));
//    QEventLoop eventLoop;
//    QObject::connect(&networkManager, &QNetworkAccessManager::finished, &eventLoop, &QEventLoop::quit);
//    eventLoop.exec();
}


void MainWindow::onResultUpdate(QNetworkReply* /* reply */)
{
//    if (currentReply->error() != QNetworkReply::NoError)
//    {
//        //qDebug()<<"ERROR!";
//        return;  // ...only in a blog post
//    }

//    QString data = (QString) reply->readAll();
//    //qDebug()<<data;
//    QJsonDocument d = QJsonDocument::fromJson(data.toUtf8());
//    QJsonObject sett2 = d.object();
//    QJsonValue value = sett2.value(QString("tag_name"));
//    //qDebug() << value;

//    if(value.toDouble() > VERSION)
//    {
//        QMessageBox::StandardButton button;
//        button = QMessageBox::question(this, tr("有新的版本"),
//                QString(tr("是否下载新的版本？")),
//                QMessageBox::Yes | QMessageBox::No);

//        if (button == QMessageBox::Yes)
//        {
//             downloadNewApp();
//        }
//    }
//    else
//    {
//        QMessageBox::information(0, "更新检查","此版本已经是最新发布版本", QMessageBox::Yes);
//    }
}

// 显示当前文档的字符总数以及总行数  NOTE: 注意这里的中文字符按一个字符来算
void MainWindow::showSizeLines()
{
//    sizeLinesLabel->setText(tr("length : %1  lines : %2")
//                            .arg(textEdit->document()->characterCount() - 1)
//                            .arg(textEdit->document()->lineCount()));
}

// 显示当前光标所在的行列号
void MainWindow::showCursorPosition(int line, int index)
{
    rowColumnLabel->setText(tr("Ln : %1   Col : %2").arg(line + 1).arg(index));
}

void MainWindow::slotCopyAvailable(bool enabled)
{
    cutAction->setEnabled(enabled);
    copyAction->setEnabled(enabled);
    deleteAction->setEnabled(enabled);
}


void MainWindow::downloadNewApp()
{

}

void MainWindow::updateEndOfLineModeLabel()
{
    EndOfLineText endLineFormat = m_pTextEdit->getLineFormat();
    if(endLineFormat == EndOfLineText::Windows1)
    {
        lineFormat = EndOfLine::Windows;
        endOfLineModeLabel->setText(tr("Windows (CR LF)"));
    }
    else if(endLineFormat == EndOfLineText::Unix1)
    {
        lineFormat = EndOfLine::Unix;
        endOfLineModeLabel->setText(tr("Unix (LF)"));
    }
    else if(endLineFormat == EndOfLineText::Mac1)
    {
        lineFormat = EndOfLine::Mac;
        endOfLineModeLabel->setText(tr("Macintosh (CR)"));
    }
    else
    {
        lineFormat = EndOfLine::Default;
        endOfLineModeLabel->setText(tr("Unix (LF)"));
    }
}

void MainWindow::updateActions()
{
    autoLineAction->setChecked(isAutoLine);
//    textEdit->setLineWrapMode(isAutoLine ? QTextEdit::WidgetWidth : QTextEdit::NoWrap);
}







