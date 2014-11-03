#include "controllerpercorso.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ControllerPercorso w;
    w.show();

    return a.exec();
}
