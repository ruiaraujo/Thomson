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
    BruteForce(uint32_t) ;
    ~BruteForce(){}
    void generate(uint32_t);
    QVector<QString> getResults();
  signals:
    void updateBar();
  protected:
    void run();
  private:
    uint32_t essid;
    QVector<QString> results;
} ;

#endif /* BRUTEFORCE_H_ */
