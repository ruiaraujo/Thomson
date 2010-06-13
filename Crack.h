#ifndef CRACK_H
#define CRACK_H

#include <QtGui/QWidget>
#include <QFutureWatcher>
#include <stdint.h>
#include "ui_crack.h"
#include <QTextStream>


class Crack : public QWidget, public Ui::CrackClass
{
    Q_OBJECT

public:
    Crack(QWidget *parent = 0);
    ~Crack();

private slots:
        void process();
        void finished();

private:
        QFutureWatcher<void> watcher;
        bool running;

};

extern void crack( uint32_t essid );


#endif // CRACK_H
