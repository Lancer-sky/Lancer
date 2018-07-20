#include "visualsort.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    visualsort w;
    w.show();

    return a.exec();
}
