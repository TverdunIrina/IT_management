#include "prim_result.h"
#include <QTableWidget>
#include <QVBoxLayout>
#include <QPushButton>

prim_result::prim_result(QTableWidget *v)
{
    QVBoxLayout *vb = new QVBoxLayout();
    QTableWidget *table = new QTableWidget();
    table = v;
    QPushButton *ok = new QPushButton("Ok");

    vb->addWidget(table);
    vb->addWidget(ok);
    setLayout(vb);

    connect(ok, SIGNAL(clicked(bool)), this, SLOT(close()));
}
