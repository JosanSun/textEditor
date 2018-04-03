#ifndef MainWindow_H
#define MainWindow_H

#include <QMainWindow>
#include <QtNetwork>
#include "texteditor.h"
#include "md5widget.h"

QT_BEGIN_NAMESPACE
class QAction;
class QLabel;
class QMenu;
class QToolBar;
class QTextEdit;
class QCloseEvent;
QT_END_NAMESPACE

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
    // bool loadFile(const QString& fileName);
    bool saveFile(const QString& fileName);
    void updateRecentFileActions();
    QString strippedName(const QString& fileName);

private slots:
    void newFile();
    void open();
    bool save();
    bool saveAs();
    void printFile();
    void about();
    void textEditorModified();
    void openRecentFile();
    void MD5WidgetShow();

    void updateApp();
    void onResultUpdate(QNetworkReply*);

    void showCursorPosition();



private:
    // 当前文件名
    QString curFile;            // 带有路径的文件名 eg：C:\test\test.txt
    QStringList recentFiles;    // recentFiles文件名存储格式与curFile一致
    TextEditor* textEdit;
    QLabel* showLabel;
    QLabel* insertModeLabel;

    // 菜单项
    QMenu* fileMenu;
    QMenu* editMenu;
    QMenu* searchMenu;
    QMenu* viewMenu;
    QMenu* settingsMenu;
    QMenu* toolsMenu;
    QMenu* md5Menu;
    QMenu* helpMenu;

    // 工具栏
    QToolBar* fileToolBar;
    QToolBar* editToolBar;

    // 各类菜单项
    QAction* newAction;
    QAction* openAction;
    QAction* saveAction;
    QAction* saveAsAction;
    QAction* printAction;
    enum{ MaxRecentFiles = 5 };
    QAction* recentFileActions[MaxRecentFiles];
    QAction* separatorAction;        // 用于最近打开文件菜单项的分隔
    QAction* exitAction;
    QAction* undoAction;
    QAction* redoAction;
    QAction* cutAction;
    QAction* copyAction;
    QAction* pasteAction;
    QAction* deleteAction;
    QAction* selectAllAction;
    QAction* findAction;
    QAction* fullScreenAction;
    QAction* optionAction;
    QAction* generateMD5Action;
    QAction* updateAction;
    QAction* aboutAction;

    void downloadNewApp();
    QNetworkReply* currentReply;
};

#endif // MainWindow_H
