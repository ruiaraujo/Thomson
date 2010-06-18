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
#include <QVector>

class BruteForce : public QThread
{
    Q_OBJECT
  public:
    BruteForce( uint32_t essid) ;
    BruteForce( uint32_t essid , int year ) ;
    virtual ~BruteForce(){}
    void generate(uint32_t essid);
    void generate(uint32_t essid , int year);
    QVector<QString> getResults();
  signals:
    void updateBar();
  protected:
    void run();
  private:
    uint32_t essid;
    QVector<QString> results;
    int year_begin;
    int year_end;
} ;

#endif /* BRUTEFORCE_H_ */
