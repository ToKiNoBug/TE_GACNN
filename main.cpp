#include "MainWindow.h"

#include <QApplication>

#include "Sequence.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    //test_Sequence();

    return a.exec();
}
