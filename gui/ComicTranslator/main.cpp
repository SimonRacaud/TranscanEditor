#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

#include <QFile>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Setup app stylesheet
    /*QFile stylesheetFile("/media/work/personnal-projects/scanTranslator/gui/ComicTranslator/Devsion.qss");
    stylesheetFile.open(QFile::ReadOnly);
    QString stylesheet = QLatin1String(stylesheetFile.readAll());
    app.setStyleSheet(stylesheet);*/

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "ComicTranslator_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            app.installTranslator(&translator);
            break;
        }
    }
    MainWindow w;
    w.show();
    return app.exec();
}
