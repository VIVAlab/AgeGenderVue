
#include <QApplication>
#include <QtGui>

#include "facetrackingwindow.h"
#include "demomanagerwindow.h"



int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    DemoManagerWindow demoManagerWindow;
    demoManagerWindow.show();

    return app.exec();
}




