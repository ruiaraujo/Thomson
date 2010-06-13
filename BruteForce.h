/*
 * BruteForce.h
 *
 *  Created on: 2010/06/13
 *      Author: Rui Araújo
 */

#ifndef BRUTEFORCE_H_
#define BRUTEFORCE_H_

#include <stdint.h>
#include <QThread>

class BruteForce : public QThread
{
  public:
    BruteForce(uint32_t) ;
    virtual ~BruteForce(){}
  private:
    uint32_t essid;

  protected:
    void run();
} ;

#endif /* BRUTEFORCE_H_ */
