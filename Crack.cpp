#include "Crack.h"
#include <QVector>
#include <QString>
#include <cstdio>

Crack::Crack(QWidget *parent)
    : QWidget(parent) , running(false)  , finder(NULL)
{
    setupUi(this);
    QRegExp rx("[a-fA-F0-9]{6}");
    QValidator *validator = new QRegExpValidator(rx, this);
    lineEdit->setValidator(validator);
    connect( calc , SIGNAL( clicked() ), this , SLOT(process()) );
    connect( lineEdit , SIGNAL( returnPressed() ), this , SLOT(process()) );
}

void Crack::process()
{
  int pos;
  QString input = lineEdit->text();
  if ( input.isEmpty() )
  {
      output->setText("ESSID code empty!");
      return;
    }
  if ( lineEdit->validator()->validate( input , pos ) != QValidator::Acceptable )
  {
    output->setText("ESSID code not valid!");
    return;
  }
  else
    if ( !running )
      output->setText("Calculating...");
    else
      output->setText("Already running!");
  this->running = true;
  uint32_t essid = input.toInt(NULL , 16 );
  if (  finder == NULL )
  {
    finder = new BruteForce( essid );
    connect( finder, SIGNAL(finished()),  this , SLOT(finished()));
    connect( finder, SIGNAL(updateBar()),  this , SLOT(updateProgress()));
  }
  this->progressBar->setValue(0);
  finder->generate(essid);
}

void Crack::finished()
{
  running = false;
  output->setText("Calculation finished!");

  QVector<QString> result = finder->getResults();
  if ( result.size() == 0 )
    output->append("No Keys found...");
  else
  {
    output->append("Keys found!");
    for ( int i = 0; i < result.size() ; ++i )
      output->append(result.at(i));
  }
  delete finder;
  finder = NULL;
}

void Crack::updateProgress()
{
  this->progressBar->setValue(this->progressBar->value()+1);
}


Crack::~Crack()
{

}
