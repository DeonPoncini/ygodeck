#include "Lobby.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Lobby l;
    l.show();

    return a.exec();
}
