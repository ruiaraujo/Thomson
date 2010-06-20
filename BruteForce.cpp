/*
 * BruteForce.cpp
 *
 *  Created on: 2010/06/13
 *      Author: Rui Araújo
 */

#include "BruteForce.h"
#include "unknown.h"
#include <cstdlib>
#include <cstdio>
#include <openssl/sha.h>


BruteForce::BruteForce( uint32_t essid) : QThread() , results() , year_begin(4),
                                          year_end(10) , running(false)
{
  this->essid = essid;
}

BruteForce::BruteForce( uint32_t essid , int year ) : QThread() , results(), running(false)
{
  this->essid = essid;
  if ( year > 2000 )
    this->year_begin = this->year_end = year - 2000;
  else
    this->year_begin = this->year_end = year;
}


void BruteForce::stop()
{
  this->running = false;
}

void BruteForce::generate( uint32_t essid , int year )
{
  if ( year != 0 )
  {
    if ( year > 2000 )
    this->year_begin = this->year_end = year - 2000;
    else
      this->year_begin = this->year_end = year;
  }
  this->essid = essid;
  this->results.clear();
  if (!isRunning())
  {
    this->running = true;
    start(TimeCriticalPriority);
  }
}

void BruteForce::run()
{
  int n = sizeof(dic)/sizeof("AAA");
  {
      uint8_t message_digest[SHA_DIGEST_LENGTH];
      char unknown[7]= " ";
      char result[11];
      char input[13];
      SHA_CTX sha1;
      input[0] = 'C';
      input[1] = 'P';
      uint32_t * ptr = (uint32_t *)malloc(sizeof(uint32_t));
      for( int i = 0 ; i < n; ++i  )
      {
        if ( !running )
          return;
          sprintf( unknown , "%02X%02X%02X" , (int)dic[i][0]
                                  , (int)dic[i][1], (int)dic[i][2] );
          if ( i % ( n / 100 ) == 0 )
            emit updateBar();
          for ( int year = this->year_begin ; year <= this->year_end ; ++year )
          {
              for ( int week = 1 ; week <= 52 ; ++week )
              {
                  input[2] = '0' + year / 10;
                  input[3] = '0' + year % 10;
                  input[4] = '0' + week / 10;
                  input[5] = '0' + week % 10;
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

                  if ( (*ptr) == this->essid )
                  {
                    sprintf( result ,"%02X%02X%02X%02X%02X" , message_digest[0], message_digest[1] ,
                                      message_digest[2] , message_digest[3], message_digest[4] );
                     QString string(result);
                     results.append(string);
                  }


              }
          }
      }
  }
  this->running = false;
  return;
}

QVector<QString> BruteForce::getResults()
{
  return results;
}
