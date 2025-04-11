#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QCursor>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCursor cursor(Qt::BlankCursor);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "Neo_Air_Mat_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    MainWindow w;

    w.setCursor(cursor);
    w.showFullScreen();
    w.setWindowState(Qt::WindowFullScreen);
    w.show();

    return a.exec();
}
