#include "satsolverqt.h"
#include "ui_satsolverqt.h"
#include "define.h"
#include "function.h"
#include "binary_puzzle.cpp"

SATSolverQt::SATSolverQt(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SATSolverQt)
{
    ui->setupUi(this);
    ui->boardTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->boardTable->horizontalHeader()->setHidden(true);
    ui->boardTable->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->boardTable->verticalHeader()->setHidden(true);
}

SATSolverQt::~SATSolverQt()
{
    delete ui;
}


void SATSolverQt::on_puzzleGen_clicked()
{
    int size = ui->chooseSize->currentText().toInt();
    puzzleGen(1,size);
    QStandardItemModel *itemModel;
    ui->boardTable->clearSpans();
    itemModel = new QStandardItemModel(size,size);
    ui->boardTable->setModel(itemModel);
    connect(ui->boardTable->model(),SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)),this,SLOT(on_data_changed(QModelIndex,QModelIndex,QVector<int>)));
    FILE* fp = fopen(".\\ans.sud","r");
    fscanf(fp,"%*d%*d");
    for(int i = 0;i < size;i++)
    {
        for(int j = 0;j < size;j++)
        {
            char c;
            fscanf(fp,"%c",&c);
            if(c == '\n')
            {
                j--;
                continue;
            }
            ans[i][j] = c - 48;
        }
    }
    fclose(fp);
    fp = fopen(".\\question.sud","r");
    fscanf(fp,"%*d%*d");
    for(int i = 0;i < size;i++)
    {
        for(int j = 0;j < size;j++)
        {
            char c;
            fscanf(fp,"%c",&c);
            if(c == '\n')
            {
                j--;
                continue;
            }
            if(c == '*')
            {
                QStandardItem *item = new QStandardItem(QString(" "));
                item->setTextAlignment(Qt::AlignCenter);
                itemModel->setItem(i,j,item);
                editable[i][j] = 1;
            }
            else
            {
                QStandardItem *item = new QStandardItem(QString(c));
                item->setTextAlignment(Qt::AlignCenter);
                itemModel->setItem(i,j,item);
                item->setForeground(QColor(150,150,150));
                item->setEnabled(false);
            }
        }
    }
    fclose(fp);
}

void SATSolverQt::on_assign0_clicked()
{
    QModelIndex index = ui->boardTable->selectionModel()->currentIndex();
    ui->boardTable->model()->setData(index,QString("0"));
}

void SATSolverQt::on_assign1_clicked()
{
    QModelIndex index = ui->boardTable->selectionModel()->currentIndex();
    ui->boardTable->model()->setData(index,QString("1"));
}

void SATSolverQt::on_assignCancel_clicked()
{
    QModelIndex index = ui->boardTable->selectionModel()->currentIndex();
    ui->boardTable->model()->setData(index,QString(" "));
}

void SATSolverQt::on_data_changed(QModelIndex, QModelIndex, QVector<int>)
{
    int m = puzzle_size;
    int count = 0;
    for(int i = 0;i < m;i++)
    {
        for(int j = 0;j < m;j++)
        {
            QModelIndex index = ui->boardTable->model()->index(i,j,QModelIndex());
            if(index.data() == " ")
                continue;
            if(ans[i][j] == index.data().toInt())
                count++;
        }
    }
    if(count == m * m)
        QMessageBox::information(this,QString("恭喜！"),QString("你赢了！"));
}

void SATSolverQt::on_getAns_clicked()
{
    branching_mode = 2;
    int m = puzzle_size;
    QStandardItemModel *itemModel;
    ui->boardTable->clearSpans();
    itemModel = new QStandardItemModel(m,m);
    ui->boardTable->setModel(itemModel);
    FILE* fp = fopen(".\\ans.sud","r");
    fscanf(fp,"%*d%*d");
    for(int i = 0;i < m;i++)
    {
        for(int j = 0;j < m;j++)
        {
            char c;
            fscanf(fp,"%c",&c);
            if(c == '\n')
            {
                j--;
                continue;
            }
            QStandardItem *item = new QStandardItem(QString(c));
            item->setTextAlignment(Qt::AlignCenter);
            item->setForeground(QColor(150,150,150));
            item->setSelectable(false);
            itemModel->setItem(i,j,item);
            ans[i][j] = 0;
        }
    }
}

void SATSolverQt::on_getPuzzle_clicked()
{
    branching_mode = 2;
    QString str = QFileDialog::getOpenFileName(this,tr("打开文件"),tr("D:/"),tr("数独文件 (*.sud)"));
    QTextCodec *code = QTextCodec::codecForName("GB2312");
    std::string name = code->fromUnicode(str).data();
    char path[80];
    strcpy(path,name.c_str());
    if(!readPuzzle(path))
        QMessageBox::information(this,QString("错误！"),QString("无解！"));
    int m = puzzle_size;
    if(dpll())
    {
        FILE* fans = fopen(".\\ans.sud","w");
        fprintf(fans,"%d %d\n",m,m*m);
        for(int i = 1;i <= m*m;i++)
        {
            if(assign[i].type == TRUE)
            {
                fprintf(fans,"%c",'1');
                ans[(i - 1) / m][(i - 1) % m] = 1;
            }
            else
            {
                fprintf(fans,"%c",'0');
                ans[(i - 1) / m][(i - 1) % m] = 0;
            }
            if(i % m == 0)
                fprintf(fans,"\n");
        }
        fclose(fans);
    }
    else
        QMessageBox::information(this,QString("错误！"),QString("无解！"));
    QStandardItemModel *itemModel;
    ui->boardTable->clearSpans();
    itemModel = new QStandardItemModel(m,m);
    ui->boardTable->setModel(itemModel);
    connect(ui->boardTable->model(),SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)),this,SLOT(on_data_changed(QModelIndex,QModelIndex,QVector<int>)));
    FILE* fp = fopen(path,"r");
    fscanf(fp,"%*d%*d");
    for(int i = 0;i < m;i++)
    {
        for(int j = 0;j < m;j++)
        {
            char c;
            fscanf(fp,"%c",&c);
            if(c == '\n')
            {
                j--;
                continue;
            }
            if(c == '*')
            {
                QStandardItem *item = new QStandardItem(QString(" "));
                item->setTextAlignment(Qt::AlignCenter);
                itemModel->setItem(i,j,item);
                editable[i][j] = 1;
            }
            else
            {
                QStandardItem *item = new QStandardItem(QString(c));
                item->setTextAlignment(Qt::AlignCenter);
                itemModel->setItem(i,j,item);
                item->setForeground(QColor(150,150,150));
                item->setEnabled(false);
            }
        }
    }
    fclose(fp);
    ui->chooseSize->setCurrentText(QString::number(m));
}

void SATSolverQt::on_getSAT_clicked()
{
    ui->showAns->clear();
    ui->showSAT->clear();
    QString str = QFileDialog::getOpenFileName(this,tr("打开文件"),tr("D:/Study/数据结构/程序设计综合课程设计任务及指导学生包/SAT测试备选算例"),tr("CNF文件(*.cnf)"));
    QTextCodec *code = QTextCodec::codecForName("GB2312");
    std::string name = code->fromUnicode(str).data();
    strcpy(cnfPath,name.c_str());
    QFile file(str);
    file.open(QIODevice::ReadOnly);
    ui->showSAT->setPlainText(file.readAll());
    file.close();
}

void SATSolverQt::on_solveSAT_clicked()
{
    ui->showAns->clear();
    branching_mode = ui->chooseBranching->currentIndex();
    begin = clock();
    if(!reader(cnfPath))
    {
        end = clock();
        ui->showAns->appendPlainText("所用时间："+QString::number(end-begin)+"ms");
        ui->showAns->appendPlainText("无法满足！");
        return;
    }
    if(dpll())
    {
        end = clock();
        ui->showAns->appendPlainText("所用时间："+QString::number(end-begin)+"ms");
        for(int i = 1;i <= n_vars;i++)
        {
            if(assign[i].type == TRUE)
                ui->showAns->appendPlainText(QString::number(i));
            else
                ui->showAns->appendPlainText(QString::number(-i));
        }
    }
    else
    {
        end = clock();
        ui->showAns->appendPlainText("所用时间："+QString::number(end-begin)+"ms");
        ui->showAns->appendPlainText("无法满足！");
    }
    ui->showAns->moveCursor(QTextCursor::Start);
}
