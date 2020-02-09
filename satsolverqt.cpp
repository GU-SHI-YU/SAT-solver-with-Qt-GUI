#include "satsolverqt.h"
#include "ui_satsolverqt.h"

SATSolverQt::SATSolverQt(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SATSolverQt)
{
    ui->setupUi(this);
}

SATSolverQt::~SATSolverQt()
{
    delete ui;
}

