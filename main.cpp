#include "welcomepage.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    welcomepage w;
    w.show();
    return a.exec();
}
