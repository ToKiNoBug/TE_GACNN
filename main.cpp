#include "MainWindow.h"

#include <QApplication>

#include "Sequence.h"
#include "Gene.h"
#include "defines.h"

#include <ctime>
#include <cmath>

int main(int argc, char *argv[])
{
    std::srand(std::time(nullptr));

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    //test_Sequence();
    test_Gene();

    return a.exec();
}
