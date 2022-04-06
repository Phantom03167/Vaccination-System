#include "mainwidget.h"
#include "login.h"
#include "appinit.h"
#include "quihelper.h"

#include <QApplication>

int main(int argc, char *argv[])
{
#if (QT_VERSION >= QT_VERSION_CHECK(5,14,0))
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::Floor);
#endif
    QApplication a(argc, argv);
    AppInit::Instance()->start();

    mainWidget w;

    return a.exec();
}
