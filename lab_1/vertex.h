#ifndef VERTEX_H
#define VERTEX_H
#include <QString>

class edge
{
public:
    edge::edge(QString start, QString fin, int _val)
    {
        begin = start;
        end = fin;
        val = _val;
    }
    QString begin;
    QString end;
    int val;
};

#endif // VERTEX_H
