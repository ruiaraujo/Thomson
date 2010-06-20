#ifndef CRACK_H
#define CRACK_H

#include <QtGui/QWidget>
#include <stdint.h>
#include "ui_crack.h"
#include "BruteForce.h"

class GraphicMode : public QWidget, public Ui::CrackClass
{
    Q_OBJECT

public:
    GraphicMode(QWidget *parent = 0);
    ~GraphicMode();

private slots:
        void processEssid();
        void finished();
        void updateProgress();
        void setYear();

private:
        void getResults();
        bool running;
        BruteForce * finder;
};



#endif // CRACK_H
