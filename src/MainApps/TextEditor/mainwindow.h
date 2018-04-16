#ifndef MainWindow_H
#define MainWindow_H

#include <QMainWindow>
#include <QtNetwork>
#include "texteditor.h"
#include "my_plug-in/finddialog.h"
#include "my_plug-in/md5widget.h"
#include "my_plug-in/md5filewidget.h"

QT_BEGIN_NAMESPACE
class QAction;
class QLabel;
class QMenu;
class QToolBar;
class QTextEdit;
class QCloseEvent;
class FindDialog;
class QPrinter;
QT_END_NAMESPACE

enum EndOfLine
{
    Default, Windows, Unix, Mac
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    // 为了在main()中调用，改为public权限。
    bool loadFile(const QString& fileName);

protected:
    void closeEvent(QCloseEvent* event);
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent* event);

private:
    void createActions();
    void createMenus();
    void createContextMenu();
    void createToolBars();
    void createStatusBar();
    void readSettings();
    void writeSettings();
    bool okToContinue();
    void setCurrentFile(const QString& fileName);
    bool saveFile(const QString& fileName);
    void updateRecentFileActions();
    void updateEndOfLineModeLabel();
    // 更新自动换行菜单项
    void updateActions();


private slots:
    void newFile();
    void open();
    bool save();
    bool saveAs();
    void printFile();
    void printFilePreview();
    void printPreview(QPrinter *printer);
    void printFilePDF();
    void about();
    void textEditorModified();
    void clipboardDataChanged();
    void openRecentFile();
    void MD5WidgetShow();
    void MD5FileWidgetShow();
    void find();
    void setFullScreen();
    void changeAutoLine();
    void updateApp();
    void onResultUpdate(QNetworkReply*);

    void showCursorPosition(int line, int index);
    void slotCopyAvailable(bool enabled);
    // 显示大小和总行数的状态栏
    void showSizeLines();
    // 更新当前光标所在的行列号
    void showCursorPosition();

private:
    // 当前文件名
    QString curFile;            // 带有路径的文件名 eg：C:\test\test.txt
    QString shownName;           // 作为curFile的辅助，主要在curFile为空时，辅助shownName为new
    QStringList recentFiles;    // recentFiles文件名存储格式与curFile一致
    // 主编辑界面
    TextEditor* textEdit      = nullptr;
    // 查找对话框
    FindDialog* findDialog       = nullptr;
    MD5Widget* md5Widget         = nullptr;
    MD5FileWidget* md5FileWidget = nullptr;

    EndOfLine lineFormat = EndOfLine::Default;

    // 状态栏的5个显示标签
    QLabel* showLabel            = nullptr;
    QLabel* sizeLinesLabel       = nullptr;
    QLabel* rowColumnLabel       = nullptr;
    QLabel* endOfLineModeLabel   = nullptr;
    QLabel* insertModeLabel      = nullptr;

    bool isAutoLine = true;

    // 菜单项
    QMenu* fileMenu     = nullptr;
    QMenu* editMenu     = nullptr;
    QMenu* searchMenu   = nullptr;
    QMenu* viewMenu     = nullptr;
    QMenu* settingsMenu = nullptr;
    QMenu* toolsMenu    = nullptr;
    QMenu* md5Menu      = nullptr;
    QMenu* helpMenu     = nullptr;

    // 工具栏
    QToolBar* mainToolBar = nullptr;
    //QToolBar* editToolBar;

    // 各类菜单项
    QAction* newAction          = nullptr;
    QAction* openAction         = nullptr;
    QAction* saveAction         = nullptr;
    QAction* saveAsAction       = nullptr;
    QAction* printAction        = nullptr;
    QAction* printPreviewAction = nullptr;
    QAction* exportToPDFAction  = nullptr;
    enum{ MaxRecentFiles = 5 };
    QAction* recentFileActions[MaxRecentFiles];
    QAction* separatorAction   = nullptr;        // 用于最近打开文件菜单项的分隔
    QAction* exitAction        = nullptr;
    QAction* undoAction        = nullptr;
    QAction* redoAction        = nullptr;
    QAction* cutAction         = nullptr;
    QAction* copyAction        = nullptr;
    QAction* pasteAction       = nullptr;
    QAction* deleteAction      = nullptr;
    QAction* selectAllAction   = nullptr;
    QAction* findAction        = nullptr;
    QAction* fullScreenAction  = nullptr;
    QAction* autoLineAction    = nullptr;
    QAction* optionAction      = nullptr;
    QAction* generateMD5Action = nullptr;
    QAction* generateMD5FileAction = nullptr;
    QAction* updateAction      = nullptr;
    QAction* aboutAction       = nullptr;

    void downloadNewApp();
    QNetworkReply* currentReply = nullptr;
};

#endif // MainWindow_H
