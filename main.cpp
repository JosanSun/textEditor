#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(texteditor);

    QApplication app(argc, argv);
    QCoreApplication::setOrganizationName("Southeast University");
    QCoreApplication::setApplicationName("TextEditor");
    QCoreApplication::setApplicationVersion(QT_VERSION_STR);
    QCoreApplication::setApplicationVersion("2.0");

    // 增加命令行的支持
    QCommandLineParser parser;
    parser.setApplicationDescription(QCoreApplication::applicationName());
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("file", "The file to open.");
    parser.process(app);

    MainWindow mainWindow;
    if(!parser.positionalArguments().isEmpty())
    {
        mainWindow.loadFile(parser.positionalArguments().first());
    }
    mainWindow.show();

    return app.exec();
}
