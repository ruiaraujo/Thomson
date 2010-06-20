/*
 * TextMode.h
 *
 *  Created on: 2010/06/19
 *      Author: Rui Araújo
 */

#ifndef TEXTMODE_H_
#define TEXTMODE_H_

class TextMode
{
  public:
    TextMode() ;
    int exec(int argc, char *argv[]);
    virtual ~TextMode(){}
  private:
    bool validInput( char * input );
} ;

#endif /* TEXTMODE_H_ */
