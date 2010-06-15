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
#include <omp.h>
#include <openssl/sha.h>
#define BEGIN_YEAR 4
#define FINAL_YEAR 10


BruteForce::BruteForce( uint32_t essid) : QThread() , results()
{
  this->essid = essid;
}

void BruteForce::generate( uint32_t essid )
{
  this->essid = essid;
  this->results.clear();
  if (!isRunning())
           start(TimeCriticalPriority);
}

void BruteForce::run()
{
  int n = sizeof(dic)/sizeof("AAA");
  //printf("essid:%X" , essid);
  //#pragma omp parallel
  {
      uint8_t message_digest[SHA_DIGEST_LENGTH];
      char unknown[7]= " ";
      char result[11];
      char input[13];
      SHA_CTX sha1;
      input[0] = 'C';
      input[1] = 'P';
      uint32_t * ptr = (uint32_t *)malloc(sizeof(uint32_t));
    //  #pragma omp for
      for( int i = 0 ; i < n; ++i  )
      {
          sprintf( unknown , "%02X%02X%02X" , (int)dic[i][0]
                                  , (int)dic[i][1], (int)dic[i][2] );
          for ( int year = BEGIN_YEAR ; year <= FINAL_YEAR ; ++year )
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
              //       printf( "Possibility: Year - %d\tWeek: %d\n", 2000+year,  week );
                //     printf("ESSID: Thomson%02X%02X%02X\t" , message_digest[17] ,
                  //                     message_digest[18] , message_digest[19]);

                    sprintf( result ,"%02X%02X%02X%02X%02X" , message_digest[0], message_digest[1] ,
                                      message_digest[2] , message_digest[3], message_digest[4] );
                   // #pragma omp critical
                    //{
                     QString string(result);
                     results.append(string);
                    //}
                  }


              }
          }
      }
  }
  return;
}

QVector<QString> BruteForce::getResults()
{
  return results;
}
