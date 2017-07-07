/**************************************************************************/
/*!
    @file     Regression.h
    @author   c hedenberg
    @license  BSD (see license.txt)


*/
/**************************************************************************/

#ifndef _Regression_
#define _Regression_

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

class Regression {

  public: 
  float params[3] = {0.000000,0.000000,0.000000};
  float x[3] = {0.000000,0.000000,0.000000};
  float y = 0.00000000;
  float lr;
  float _yhat=0.0000000;
  float rewarr[2] = {0.000000,0.000000};
  float exprew = 0.0000000;
  
  Regression(float lrin, float rew, float pen){
    lr = lrin; 
    rewarr[0] = rew;
    rewarr[1] = pen;
  }


  void fit();
  
  float expRewFunc();

  float pred();
};
#endif