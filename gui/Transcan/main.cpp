#include "window/MainWindow.h"
#include "window/ViewerWindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

#include <QFile>

using namespace std;

MainWindow *mainWindow = nullptr;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    int status;

    // Setup app stylesheet
    QFontDatabase::addApplicationFont(":/font/Inter.ttf");
    QFile stylesheetFile(":/stylesheet.qss");
    if (!stylesheetFile.open(QFile::ReadOnly)) {
        std::cerr << "ERROR: fail to open stylesheet file." << std::endl;
    }
    QString stylesheet = QLatin1String(stylesheetFile.readAll());
    app.setStyleSheet(stylesheet);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages)
    {
        const QString baseName = "Transcan_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName))
        {
            app.installTranslator(&translator);
            break;
        }
    }
    mainWindow = new MainWindow();
    mainWindow->show();
    status = app.exec();
    delete mainWindow;
    return status;
}
