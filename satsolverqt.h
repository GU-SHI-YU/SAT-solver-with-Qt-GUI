#ifndef SATSOLVERQT_H
#define SATSOLVERQT_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class SATSolverQt; }
QT_END_NAMESPACE

class SATSolverQt : public QMainWindow
{
    Q_OBJECT

public:
    SATSolverQt(QWidget *parent = nullptr);
    ~SATSolverQt();

private:
    Ui::SATSolverQt *ui;
};
#endif // SATSOLVERQT_H
