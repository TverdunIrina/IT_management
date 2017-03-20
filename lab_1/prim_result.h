#ifndef PRIM_RESULT_H
#define PRIM_RESULT_H

#include <QWidget>
#include <vector>
#include "vertex.h"
#include "vertex.cpp"
#include <QTableWidget>

using namespace std;

class prim_result:public QWidget
{
    Q_OBJECT
public:
    prim_result(QTableWidget *v);
};

#endif // PRIM_RESULT_H
