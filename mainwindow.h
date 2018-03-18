#ifndef MainWindow_H
#define MainWindow_H

#include <QMainWindow>
#include <texteditor.h>

class QAction;
class QLabel;
class QMenu;
class QToolBar;
class QTextEdit;
class QCloseEvent;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent* event);

private slots:
    void newFile();
    void open();
    bool save();
    bool saveAs();
    void cut();
    void copy();
    void paste();
    void deleteText();
    void selectAll();
    void find();
    void about();

    void textModified();
    //void textUnmodified();

private:
    void createActions();
    void createMenus();
    void createContextMenu();
    void createToolBars();
    void createStatusBar();
    bool okToContinue();
    void setCurrentFile(const QString& fileName);
    bool loadFile(const QString& fileName);
    bool saveFile(const QString& fileName);
    QString strippedName(const QString& fileName);

    // 当前文件名
    QString curFile;
    TextEditor* textEdit;
    QLabel* showLabel;
    bool textIsModified;

    // 菜单项
    QMenu* fileMenu;
    QMenu* editMenu;
    QMenu* searchMenu;
    QMenu* viewMenu;
    QMenu* settingsMenu;
    QMenu* toolsMenu;
    QMenu* helpMenu;

    // 工具栏
    QToolBar* fileToolBar;
    QToolBar* editToolBar;

    // 各类菜单项
    QAction* newAction;
    QAction* openAction;
    QAction* saveAction;
    QAction* saveAsAction;
    QAction* exitAction;

    QAction* cutAction;
    QAction* copyAction;
    QAction* pasteAction;
    QAction* deleteAction;
    QAction* selectAllAction;

    QAction* findAction;

    QAction* fullScreenAction;

    QAction* optionAction;

    QAction* md5Action;

    QAction* updateAction;
    QAction* aboutAction;

    void textUnmodified();
};

#endif // MainWindow_H
