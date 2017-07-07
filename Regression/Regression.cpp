/**************************************************************************/

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include <Regression.h>

/**************************************************************************/


  
  void Regression::fit(){

    for(int  i = 0;i < 3; i++){
      if( i == 0){
        params[i] = params[i] + lr*(y - _yhat);
      }else{
        params[i] = params[i] + lr*(y - _yhat)*x[i];
      }
      
    }
    
  }

  float Regression::pred(){
    float _pred = 0.00;
    for(int i = 0; i < 3; i++){
      _pred = _pred + params[i]*x[i];
    }
    
    _yhat = 1/(1+exp(-_pred));
  }
  
  float Regression::expRewFunc(){
  	exprew = ((1-_yhat)*rewarr[0])+(_yhat*rewarr[1]);
  
  }

