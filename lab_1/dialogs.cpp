#include "dialogs.h"

dialogs::dialogs(QString a)
{
    text = new QTextBrowser();
    text->setText(a);
    ok = new QPushButton("Ok");

    QVBoxLayout *v = new QVBoxLayout();
    v->addWidget(text);
    v->addWidget(ok);

    setLayout(v);
    setGeometry(650,300,200,100);
    setVisible(true);

    connect(ok, SIGNAL(clicked(bool)), this, SLOT(end()));
}

void dialogs::end()
{
    emit i_closed();
    close();
}
