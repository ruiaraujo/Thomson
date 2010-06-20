 #include "GraphicMode.h"
#include <QVector>
#include <QString>
#include <cstdio>

GraphicMode::GraphicMode(QWidget *parent)
    : QWidget(parent) , running(false)  , finder(NULL)
{
    setWindowIcon(QIcon(":/icon.ico"));
    setupUi(this);
    QRegExp rx("[a-fA-F0-9]{6}");
    QValidator *validator = new QRegExpValidator(rx, this);
    lineEdit->setValidator(validator);
    connect( calc , SIGNAL( clicked() ), this , SLOT(processEssid()) );
    connect( lineEdit , SIGNAL( returnPressed() ), this , SLOT(processEssid()) );
    connect( singleYear , SIGNAL( clicked() ), this , SLOT( setYear() ) );
}

void GraphicMode::processEssid()
{
  int pos;
  QString input = lineEdit->text();
  if ( this->running )
  {
    finder->stop();
    output->setText("Cancelled!");
    getResults();
    calc->setText("Calculate");
    this->running = false;
    return;
  }

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
  {
      output->setText("Calculating...");
      if (  this->singleYear->isChecked()  )
      {
        char year[5];
        sprintf( year , "%d" , this->spinboxYear->value() );
        output->append("Warning: Calculating only for routers from ");
        output->append(QString(year));
      }
  }
  this->running = true;
  calc->setText("Cancel");
  uint32_t essid = input.toInt(NULL , 16 );
  if (  finder == NULL )
  {
    finder = new BruteForce( essid );
    connect( finder, SIGNAL(finished()),  this , SLOT(finished()));
    connect( finder, SIGNAL(updateBar()),  this , SLOT(updateProgress()));
  }
  this->progressBar->setValue(0);
  if ( this->singleYear->isChecked() )
    finder->generate(essid , this->spinboxYear->value() - 2000 );
  else
    finder->generate(essid );
}

void GraphicMode::finished()
{
  running = false;
  output->setText("Calculation finished!");
  calc->setText("Calculate");
  getResults();

}

void GraphicMode::getResults()
{
  if ( finder == NULL )
    return;
  QVector<QString> result = finder->getResults();
    if ( result.size() == 0 )
      output->append("No Keys were found...");
    else
    {
      output->append("Found Keys!");
      for ( int i = 0; i < result.size() ; ++i )
        output->append(result.at(i));
    }
}
void GraphicMode::updateProgress()
{
  this->progressBar->setValue(this->progressBar->value()+1);
}

void GraphicMode::setYear()
{
  if ( this->singleYear->isChecked() )
    this->spinboxYear->setEnabled(true);
  else
    this->spinboxYear->setEnabled(false);
}

GraphicMode::~GraphicMode()
{

}
