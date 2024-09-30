#include "widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.setWindowFlags(Qt::FramelessWindowHint);
    w.show();

    return a.exec();
}
