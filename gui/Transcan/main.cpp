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
    /*QFile stylesheetFile("/media/work/personnal-projects/scanTranslator/gui/ComicTranslator/Devsion.qss");
    stylesheetFile.open(QFile::ReadOnly);
    QString stylesheet = QLatin1String(stylesheetFile.readAll());
    app.setStyleSheet(stylesheet);*/

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
