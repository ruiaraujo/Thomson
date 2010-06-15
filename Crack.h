#ifndef CRACK_H
#define CRACK_H

#include <QtGui/QWidget>
#include <stdint.h>
#include "ui_crack.h"
#include "BruteForce.h"

class Crack : public QWidget, public Ui::CrackClass
{
    Q_OBJECT

public:
    Crack(QWidget *parent = 0);
    ~Crack();

private slots:
        void process();
        void finished();
        void updateProgress();

private:
        bool running;
        BruteForce * finder;
};



#endif // CRACK_H
