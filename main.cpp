#include "GraphicMode.h"
#include "TextMode.h"
#include <QtGui>
#include <QApplication>


int main(int argc, char *argv[])
{
    int result = 1;
    if ( argc > 1 )
    {
      TextMode prog;
      result = prog.exec(argc , argv);
    }
    if ( result != 0 )
    {
      QApplication a(argc, argv);
      GraphicMode win;
      win.show();
      return a.exec();
    }
    return 0;
}
