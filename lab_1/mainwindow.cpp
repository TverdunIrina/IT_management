#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>

#include <set>
#include <vector>

#include <QStandardItemModel>
#include <QStandardItem>
#include <QTableWidget>

#include "prim_result.h"
#include "prim_result.cpp"
#include "vertex.h"
#include "vertex.cpp"

using namespace std;

inline void add_edge(QString b, QString e, int val, vector<edge> &edges)      //додає ребро у остове дерево
{
    edges.push_back(edge(b,e,val));
}

bool check_vertex(QString b, set<QString> &ver)                                 //повертає true коли немає у множині
{
    set<QString>::iterator it = ver.begin();
    for(;it != ver.end();it++)
        if(*it == b)
            return false;
    return true;
}

inline void add_vertex(QString b, set<QString> &ver)                            //додає вершину до списку використаних
{
    ver.insert(b);
}

const int max_for_vertex(QString ver, QTableWidget *tab, int &col)       //повертає номер вершини кінця(номер стовпця)
{
    int max = 0;
    int result = 0;
    size_t row = 0;
    size_t number_of_rows = tab->rowCount()-1;
    size_t number_of_columns = tab->columnCount();
    for(size_t j=1;j<number_of_rows;j++)                //шукаємо рядок що позначає вершину
    {
        if(tab->item(j,0)->text() == ver)   //якщо це потрібна вершина
        {
            row = j;
            break;
        }
    }
//    cout<<"max_for_vertex  row = "<<row<<endl;
    for(size_t i=1;i<number_of_columns;i++)             //шукаємо стовпець
    {
        QString val = tab->item(row,i)->text();
        if(!val.isEmpty())                                 //якщо це початок або кінець дуги то щось робимо
        {
            int temp = tab->item(number_of_rows,i)->text().toInt();     //вага дуги
            if(temp > max)
            {
                max = temp;
                result = i;
            }
        }
    }
//    cout<<"max_for_vertex  column = "<<result<<"\t value = "<<max<<endl;
    col = result;              //стовпець шуканого ребра
//    cout<<col<<endl;
    return max;
}

inline const QString end_of_edge(QString start, size_t col, QTableWidget *t)                                           //повертає позначення кінця ребра
{
    size_t res=0;
    for(size_t i=1;i<t->rowCount();i++)
    {
        QString val = t->item(i,col)->text();
        if(!val.isEmpty())
        {
            QString end = t->item(i,0)->text();
            if(end != start)
            {
//                cout<<"end_of_edge\t"<<end.toStdString()<<endl;
                return end;
            }
        }

    }
    return "";
}

const QString new_vertex(QTableWidget *table, set<QString> &ver, vector<edge> &edges)        //повертає нову вершину
{
    QString newV;
    bool flag = true;
    while(flag)
    {
        int col;      //стовпець тимчасового максимуму
        int column =0;          //стовпець максимуму
        QString start;          //початок шуканої дуги
        int max = 0;
        set<QString>::iterator b = ver.begin();
        for(;b !=ver.end();b++)
        {
            //шукаємо для усіх точок максимальне значення ваги дуги
            int temp = max_for_vertex(*b,table, col);
            if(temp > max)
            {
                start = *b;
                max = temp;
                column = col;
            }
        }
        //шукаємо кінцеву точку отриманої дуги
        QString end = end_of_edge(start,column,table);
        //якщо точка не використовувалась то заносимо її в множину виходимо з циклу
        if(check_vertex(end,ver))
        {
            newV = end;
            edges.push_back(edge(start,end,max));
            ver.insert(end);
            flag = false;
        }
        else    //якщо точка вже була видаляємо ребро і шукаємо наступну дугу
        {
            table->removeColumn(column);
        }
    }
    return "ok";
}

void Prime_algorithm(QTableWidget *table, set<QString> &ver, vector<edge> &edges)
{
    ver.insert(table->item(1,0)->text());           //початкова точка -перша вершина
    while(ver.size() != table->rowCount()-2)        //поки кількість використаних вершин не дорівнює їх кількості
    {
        QString n = new_vertex(table,ver,edges);
    }
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//    p = 0;
    connect(ui->setNumber,SIGNAL(clicked(bool)),this, SLOT(setMatrix()));
    connect(ui->prim, SIGNAL(clicked(bool)), this, SLOT(primalg()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setMatrix()
{
    QString number = ui->numberPoint->text();
    QString col = ui->numberReb->text();
    if(number.isEmpty() || col.isEmpty())
    {
        d = new dialogs("Введіть кількість вершин і ребер");
        connect(d, SIGNAL(i_closed()), this, SLOT(close_dialog()));
        d->show();
        ui->numberPoint->clear();
        ui->numberReb->clear();
        return;
    }
    if(!number.toInt())
    {
        d = new dialogs("Потрібно ввести число");
        d->show();
        ui->numberPoint->clear();
        ui->numberReb->clear();
        return;
    }
    else
    {
        size_t rows = number.toInt()+1;
        size_t columns = col.toInt()+1;
        ui->table->setColumnCount(columns);
        ui->table->setRowCount(rows+1);
//        виділення памяті під комірки таблиці
        for(size_t i=0;i<ui->table->rowCount();i++)
            for(size_t j = 0;j<ui->table->columnCount();j++)
                ui->table->setItem(i, j, new QTableWidgetItem());

//        записуємо назви вершин
        for(size_t i=1;i<rows;i++)
        {
            ui->table->item(i,0)->setText(QString((int)(i+64)));
        }
        ui->table->item(rows,0)->setText(QString("Val"));
//        записуємо номери ребер
        for(size_t i=1;i<columns;i++)
            ui->table->item(0,i)->setText(QString::number(i));
    }
}

void MainWindow::close_dialog()
{
    delete d;
}

void MainWindow::primalg()
{
    set<QString> v;
    vector<edge> ed;
    QTableWidget *nT = new QTableWidget();
    nT = ui->table;
    Prime_algorithm(nT,v,ed);

    QTableWidget *result_prime = new QTableWidget();
    size_t rows = nT->rowCount()-1;
    size_t columns = ed.size()+1;
    result_prime->setColumnCount(columns);
    result_prime->setRowCount(rows+1);
//        виділення памяті під комірки таблиці
    for(size_t i=0;i<result_prime->rowCount();i++)
        for(size_t j = 0;j<result_prime->columnCount();j++)
            result_prime->setItem(i, j, new QTableWidgetItem());

//        записуємо назви вершин
    for(size_t i=1;i<rows;i++)
    {
        result_prime->item(i,0)->setText(QString((int)(i+64)));
    }
    result_prime->item(rows,0)->setText(QString("Val"));
//        записуємо номери ребер
    for(size_t i=1;i<columns;i++)
        result_prime->item(0,i)->setText(QString::number(i));

    //заповнюємо результуючу таблицю
    for(size_t i=0;i<ed.size();i++)
    {
        for(size_t j=1;j<result_prime->rowCount();j++)
        {
            QString t = result_prime->item(j,0)->text();
            if(t == ed[i].begin || t == ed[i].end)
                result_prime->item(j,i+1)->setText("1");
        }
        result_prime->item(rows,i+1)->setText(QString::number(ed[i].val));
    }

    prim_result *res = new prim_result(result_prime);
    res->show();



    cout<<endl<<endl;
    for(size_t i=0;i<ed.size();i++)
        cout<<(ed[i].begin).toStdString()<<"\t"<<(ed[i].end).toStdString()<<"\t"<<ed[i].val<<endl;

}
