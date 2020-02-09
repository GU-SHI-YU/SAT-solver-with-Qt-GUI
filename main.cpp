#include "satsolverqt.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SATSolverQt w;
    w.show();
    return a.exec();
}
