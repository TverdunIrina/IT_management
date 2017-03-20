#ifndef DIALOGS_H
#define DIALOGS_H

#include <QWidget>
#include <QTextBrowser>
#include <QPushButton>
#include <QVBoxLayout>

class dialogs: public QWidget
{
    Q_OBJECT
public:
    dialogs(QString a);
private:
    QTextBrowser *text;
    QPushButton *ok;

signals:
    void i_closed();
public slots:
    void end();
};

#endif // DIALOGS_H
