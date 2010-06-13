#include "Crack.h"

#include <QtGui>
#include <QApplication>

void textMode(int argc, char *argv[]);

int main(int argc, char *argv[])
{
    if ( argc > 1 )
    {
      textMode( argc , argv[] );
    }
    QApplication a(argc, argv);
    Crack w;
    w.show();
    return a.exec();
}

void textMode(int argc, char *argv[])
{

}
