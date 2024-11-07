#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QFontDatabase>

#include "mainwindow.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFontDatabase::addApplicationFont(":/font/res/rainyhearts.ttf");
    QFontDatabase::addApplicationFont(":/font/res/Vermin Vibes 1989.ttf");

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "Project-eDot_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    MainWindow m;
    m.show();
    return a.exec();
}
