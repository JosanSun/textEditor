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

#ifdef _MSC_VER
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
#endif

#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    textEdit = new TextEditor(this);
    this->setCentralWidget(textEdit);

    // 创建主界面
    createActions();
    createMenus();
    createContextMenu();
    createToolBars();
    createStatusBar();

    // 读取配置
    readSettings();

    setWindowIcon(QIcon(":/images/fileIcon.png"));
    this->setCurrentFile("");
    //this->resize(500, 300);
    setUnifiedTitleAndToolBarOnMac(true);

    connect(textEdit, &TextEditor::textChanged,
            this, &MainWindow::textEditorModified);

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

void MainWindow::newFile()
{
    if(okToContinue())
    {
        textEdit->clear();
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

void MainWindow::about()
{
    QMessageBox::about(this, tr("关于 <b>TextEditor</b>"),
            tr("<h2>TextEditor 2.0</h2>"
               "<p>Copyright &copy; 2018 SouthEast University."
               "<p><b>TextEditor</b>是一个用来展示QAction, QMainWindow, QMenuBar, "
               "QStatusBar, QTableWidget, QToolBar以及其他Qt类用法."
               "<p>本软件仅用来交流讨论，有任何好的建议欢迎联系QQ:1030460698."));
}

void MainWindow::textEditorModified()
{
    setWindowModified(true);
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
}

void MainWindow::createActions()
{
    newAction = new QAction(tr("新建(&N)"), this);
    newAction->setIcon(QIcon(":/images/new.png"));
    newAction->setShortcut(QKeySequence::New);
    newAction->setStatusTip(tr("新建一个文件"));
    connect(newAction, &QAction::triggered,
            this, &MainWindow::newFile);

    openAction = new QAction(tr("打开(&O)"), this);
    openAction->setIcon(QIcon(":/images/open.png"));
    openAction->setShortcut(QKeySequence::Open);
    openAction->setStatusTip(tr("打开一个文件"));
    connect(openAction, &QAction::triggered,
            this, &MainWindow::open);

    saveAction = new QAction(tr("保存(&S)"), this);
    saveAction->setIcon(QIcon(":/images/save.png"));
    saveAction->setShortcut(QKeySequence::Save);
    saveAction->setStatusTip("保存文件");
    connect(saveAction, &QAction::triggered,
            this, &MainWindow::save);
    connect(textEdit, &TextEditor::textChanged,
            [this]()
    {
        this->saveAction->setEnabled(true);
    });
    saveAction->setEnabled(false);

    saveAsAction = new QAction(tr("另存为(&A)..."), this);
    saveAsAction->setShortcut(tr("Ctrl+Alt+S"));
    //saveAsAction->setShortcut(QKeySequence::SaveAs);  // other method
    saveAsAction->setStatusTip(tr("将文件另存为..."));
    connect(saveAsAction, &QAction::triggered,
            this, &MainWindow::saveAs);

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
    exitAction->setStatusTip(tr("退出程序"));
    connect(exitAction, &QAction::triggered,
            this, &MainWindow::close);

    undoAction = new QAction(tr("撤销(&U)"), this);
    undoAction->setIcon(QIcon(":/images/undo.png"));
    undoAction->setShortcut(QKeySequence::Undo);
    undoAction->setStatusTip(tr("撤销"));
    connect(undoAction, &QAction::triggered,
            textEdit, &TextEditor::undo);
    connect(textEdit, &TextEditor::undoAvailable,
            undoAction, &QAction::setEnabled);
    // 设定初始状态，注意与【复制】，【剪切】等动作的区别
    undoAction->setEnabled(false);

    redoAction = new QAction(tr("恢复(&R)"), this);
    redoAction->setIcon(QIcon(":/images/redo.png"));
    redoAction->setShortcut(QKeySequence::Redo);
    redoAction->setStatusTip(tr("恢复"));
    connect(redoAction, &QAction::triggered,
            textEdit, &TextEditor::redo);
    connect(textEdit, &TextEditor::redoAvailable,
            redoAction, &QAction::setEnabled);
    redoAction->setEnabled(false);

    cutAction = new QAction(tr("剪切(&T)"), this);
    cutAction->setIcon(QIcon(":/images/cut.png"));
    cutAction->setShortcut(QKeySequence::Cut);
    cutAction->setStatusTip(tr("剪切文本"));
    cutAction->setEnabled(false);
    connect(cutAction, &QAction::triggered,
            textEdit, &TextEditor::cut);
    connect(textEdit, &TextEditor::copyAvailable, cutAction, &QAction::setEnabled);

    copyAction = new QAction(tr("复制(&C)"), this);
    copyAction->setIcon(QIcon(":/images/copy.png"));
    copyAction->setShortcut(QKeySequence::Copy);
    copyAction->setStatusTip(tr("复制文本"));
    copyAction->setEnabled(false);
    connect(copyAction, &QAction::triggered,
            textEdit, &TextEditor::copy);
    connect(textEdit, &TextEditor::copyAvailable, copyAction, &QAction::setEnabled);

    pasteAction = new QAction(tr("粘贴(&P)"), this);
    pasteAction->setIcon(QIcon(":/images/paste.png"));
    pasteAction->setShortcut(QKeySequence::Paste);
    pasteAction->setStatusTip(tr("粘贴文本"));
    connect(pasteAction, &QAction::triggered,
            textEdit, &TextEditor::paste);

    deleteAction = new QAction(tr("删除(&D)"), this);
    deleteAction->setShortcut(QKeySequence::Delete);
    deleteAction->setStatusTip(tr("删除所选文本"));
    connect(deleteAction, &QAction::triggered,
            [=]()
            {
                QKeyEvent* event = new QKeyEvent(QEvent::KeyPress,
                                                 Qt::Key_Delete, Qt::NoModifier);
                QCoreApplication::postEvent(textEdit, event);
            });
    connect(textEdit, &TextEditor::copyAvailable, deleteAction, &QAction::setEnabled);

    selectAllAction = new QAction(tr("全选(&L)"), this);
    selectAllAction->setShortcut(QKeySequence::SelectAll);
    selectAllAction->setStatusTip(tr("全选文本"));
    connect(selectAllAction, &QAction::triggered,
            textEdit, &TextEditor::selectAll);

    findAction = new QAction(tr("查找(&F)..."), this);
    findAction->setStatusTip(tr("查找文本"));
    connect(findAction, &QAction::triggered,
            textEdit, &TextEditor::find);

    fullScreenAction = new QAction(tr("切换全屏模式"), this);
    fullScreenAction->setShortcut(QKeySequence::FullScreen);
    fullScreenAction->setStatusTip(tr("全屏显示"));

    optionAction = new QAction(tr("首选项..."), this);
    optionAction->setStatusTip(tr("配置程序"));

    generateMD5Action = new QAction(tr("生成..."), this);
    generateMD5Action->setStatusTip(tr("MD5校验生成工具"));
    connect(generateMD5Action, &QAction::triggered, this, &MainWindow::MD5WidgetShow);

    updateAction = new QAction(tr("升级 TextEditor"), this);
    updateAction->setStatusTip(tr("升级应用程序"));
    connect(updateAction, &QAction::triggered,
            this, &MainWindow::updateApp);

    aboutAction = new QAction(tr("关于 TextEditor..."), this);
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

    // 设置菜单
    settingsMenu = menuBar()->addMenu(tr("设置(&T)"));
    settingsMenu->addAction(optionAction);

    // 工具菜单
    toolsMenu = menuBar()->addMenu(tr("工具(&O)"));
    md5Menu = new QMenu(tr("MD5"));
    toolsMenu->addMenu(md5Menu);
    md5Menu->addAction(generateMD5Action);

    // 帮助菜单
    helpMenu = menuBar()->addMenu(tr("帮助(&H)"));
    helpMenu->addAction(updateAction);
    helpMenu->addAction(aboutAction);
}

void MainWindow::createContextMenu()
{
    textEdit->addAction(copyAction);
    textEdit->addAction(cutAction);
    textEdit->addAction(pasteAction);
    textEdit->addAction(deleteAction);
    textEdit->addAction(selectAllAction);
    textEdit->setContextMenuPolicy(Qt::ActionsContextMenu);
}

void MainWindow::createToolBars()
{
    fileToolBar = addToolBar(tr("&File"));
    fileToolBar->addAction(newAction);
    fileToolBar->addAction(openAction);
    fileToolBar->addAction(saveAction);

    editToolBar = addToolBar(tr("&Edit"));
    editToolBar->addAction(undoAction);
    editToolBar->addAction(redoAction);
    editToolBar->addSeparator();
    editToolBar->addAction(cutAction);
    editToolBar->addAction(copyAction);
    editToolBar->addAction(pasteAction);

}

void MainWindow::createStatusBar()
{
    showLabel = new QLabel(tr("Normal text file"));
    showLabel->setAlignment(Qt::AlignLeft);
    showLabel->setMinimumSize(showLabel->sizeHint());

    statusBar()->addWidget(showLabel);
}

void MainWindow::setCurrentFile(const QString &fileName)
{
    curFile = fileName;
    setWindowModified(false);

    QString shownName = tr("new");

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
    updateRecentFileActions();
}

void MainWindow::writeSettings()
{
    QSettings settings(QCoreApplication::organizationName(),
                       QCoreApplication::applicationName());

    settings.setValue("geometry", saveGeometry());

    settings.setValue("recentFiles", recentFiles);
}

bool MainWindow::okToContinue()
{
    if(isWindowModified())
    {
        int res = QMessageBox::warning(this, tr("TextEditor"),
                                       tr("The text has been modified.\n"
                                          "Do you want to save your changes?"),
                                       QMessageBox::Yes | QMessageBox::No
                                       | QMessageBox::Cancel);
        if(res == QMessageBox::Yes)
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
    if(!textEdit->readFile(fileName))
    {
        statusBar()->showMessage(tr("Loading canceled"), 2000);
        return false;
    }

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File loaded"), 2000);
    return true;
}

bool MainWindow::saveFile(const QString &fileName)
{
    if(!textEdit->writeFile(fileName))
    {
        statusBar()->showMessage(tr("Saving canceled"), 2000);
        return false;
    }

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File saved"), 2000);
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
    MD5Widget* md5widget = new MD5Widget;
    md5widget->show();
    md5widget->setAttribute(Qt::WA_DeleteOnClose);   // 防止内存泄漏
}

void MainWindow::updateApp()
{
    QNetworkAccessManager networkManager;

    QUrl url("https://api.github.com/repos/JosanSun/textEditor/releases/latest");
    // The current all existing releases are all pre-release
    // So the json file is unavailable
    QNetworkRequest request;
    request.setUrl(url);

    currentReply = networkManager.get(request);  // GET
    connect(&networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onResultUpdate(QNetworkReply*)));
    QEventLoop eventLoop;
    QObject::connect(&networkManager, &QNetworkAccessManager::finished, &eventLoop, &QEventLoop::quit);
    eventLoop.exec();
}


void MainWindow::onResultUpdate(QNetworkReply* reply)
{
    if (currentReply->error() != QNetworkReply::NoError)
    {
        //qDebug()<<"ERROR!";
        return;  // ...only in a blog post
    }

    QString data = (QString) reply->readAll();
    //qDebug()<<data;
    QJsonDocument d = QJsonDocument::fromJson(data.toUtf8());
    QJsonObject sett2 = d.object();
    QJsonValue value = sett2.value(QString("tag_name"));
    //qDebug() << value;

    if(value.toDouble() > VERSION)
    {
        QMessageBox::StandardButton button;
        button = QMessageBox::question(this, tr("有新的版本"),
                QString(tr("是否下载新的版本？")),
                QMessageBox::Yes | QMessageBox::No);

        if (button == QMessageBox::Yes)
        {
             downloadNewApp();
        }
    }
    else
    {
        QMessageBox::information(0, "更新检查","此版本已经是最新发布版本", QMessageBox::Yes);
    }
}

void MainWindow::downloadNewApp()
{

}








