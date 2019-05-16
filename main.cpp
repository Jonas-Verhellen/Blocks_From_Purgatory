#include "tetriswindow.h"
#include <QApplication>
#include <QSplashScreen>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QPixmap pixmap("/Users/Jonas/Music/splash.png");
    QSplashScreen splash(pixmap);
    splash.setWindowFlags(Qt::WindowStaysOnTopHint);
    splash.showFullScreen();
    a.processEvents();
    TetrisWindow w;
    w.show();
    return a.exec();
}
