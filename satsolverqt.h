#ifndef SATSOLVERQT_H
#define SATSOLVERQT_H

#include <QMainWindow>
#include <QTableView>
#include <QStandardItem>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextCodec>
#include <time.h>

QT_BEGIN_NAMESPACE
namespace Ui { class SATSolverQt; }
QT_END_NAMESPACE

class SATSolverQt : public QMainWindow
{
    Q_OBJECT

public:
    SATSolverQt(QWidget *parent = nullptr);
    ~SATSolverQt();

private slots:
    void on_puzzleGen_clicked();

    void on_assign0_clicked();

    void on_assign1_clicked();

    void on_assignCancel_clicked();

    void on_data_changed(QModelIndex,QModelIndex,QVector<int>);

    void on_getAns_clicked();

    void on_getPuzzle_clicked();

    void on_getSAT_clicked();

    void on_solveSAT_clicked();

private:
    Ui::SATSolverQt *ui;
    char cnfPath[80];
};
#endif // SATSOLVERQT_H
