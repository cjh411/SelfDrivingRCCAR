
//motor A connected between A01 and A02
//motor B connected between B01 and B02

#include <Wire.h>
#include <Adafruit_ADS1015.h>
#include <Regression.h>

Adafruit_ADS1115 ads1115;
int STBY = 7; //standby

//Motor A
int AIN1 = 9; //Direction
int AIN2 = 10; //Direction

//Motor B

int BIN1 = 12; //Direction
int BIN2 = 13; //Direction

float _resp = 0.000000;
float adcArr[4] = {0.0000,0.0000,0.0000,0.0000};
int movehist[4][4] = {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};
float mdpDiscreteThresh[2] = {.8,999};
int currDiscreteGroupFront = 0;
int currDiscreteGroupRear = 0;
int actionsToExplore[4] = {-1,-1,-1,-1};
int actExplrInd = 0;
//regression reg0;
Regression reg1(.1,50.0000,0.0000);
Regression reg2(.1,100.0000,0.0000);
Regression reg3(.1,75.0000,0.0000);
Regression reg4(.1,75.0000,0.0000);
Regression regArr[4] = {reg1,reg2,reg3,reg4};
int curract = 0;
float adc0tmp, adc1tmp,adc2tmp, adc3tmp;
int tmp;
int retval;
float tmpexp[4];
float currMax;
float randseed;
boolean inPin1 = LOW;
boolean inPin2 = LOW;
boolean trnPin1 = LOW;
boolean trnPin2 = LOW;
int direction;
int msmove;
//ACCELEROMETER INIT

void setup(){
  Serial.begin(9600);
  pinMode(AIN1,OUTPUT);
  pinMode(AIN2,OUTPUT);
  pinMode(BIN1,OUTPUT);
  pinMode(BIN2,OUTPUT);  
  //Specsheets say PCF8574 is officially rated only for 100KHz I2C-bus
  //PCF8575 is rated for 400KHz
  Wire.begin();
  ads1115.begin();
//
//regArr[0].rewarr[0] = 0.000;
//regArr[0].rewarr[1] = -1.000;
//
//regArr[1].rewarr[0] = 0.000;
//regArr[1].rewarr[1] = -2.000;
//
//regArr[2].rewarr[0] = 1.000;
//regArr[2].rewarr[1] = -1.000;
//  
}

void loop(){
  

    adcArr[0]=((float)ads1115.readADC_SingleEnded(0))/10000.00;
    adcArr[1]=((float)ads1115.readADC_SingleEnded(1))/10000.00;
    adcArr[2]=((float)ads1115.readADC_SingleEnded(2))/10000.00;
    adcArr[3]=((float)ads1115.readADC_SingleEnded(3))/10000.00;
    Serial.print("Starting X Values = ");Serial.print(adcArr[0]);Serial.print(",  ");Serial.print(adcArr[1]);Serial.print(",  ");Serial.print(adcArr[2]);Serial.print(",  ");Serial.print(adcArr[3]);Serial.println("");
    for(int i = 0; i < 4; i++){
      curract = i;
      setxvals();
      regArr[i].pred();
    }

    for(int i = 0; i < 2; i++){
      if(adcArr[0]<mdpDiscreteThresh[0]){
        if(adcArr[1] < mdpDiscreteThresh[0]){
          currDiscreteGroupFront = 0;
        }else{
          currDiscreteGroupFront = 1;
        }
      }else{
        if(adcArr[1] < mdpDiscreteThresh[0]){
          currDiscreteGroupFront = 2;
        }else{
          currDiscreteGroupFront = 3;
        }
      }

      if(adcArr[2]<mdpDiscreteThresh[0]){
        if(adcArr[3] < mdpDiscreteThresh[0]){
          currDiscreteGroupRear = 0;
        }else{
          currDiscreteGroupRear = 1;
        }
      }else{
        if(adcArr[3] < mdpDiscreteThresh[0]){
          currDiscreteGroupRear = 2;
        }else{
          currDiscreteGroupRear = 3;
        }
      }
    }



  tmp = evalAction();

  if(tmp == 0){
    movehist[tmp][currDiscreteGroupFront]=movehist[tmp][currDiscreteGroupFront]+1;
  }else{
    movehist[tmp][currDiscreteGroupRear]=movehist[tmp][currDiscreteGroupRear]+1;
  }
    
    if (tmp==0){
        Serial.println("backward");Serial.println("");
       inPin1 = HIGH;
       inPin2 = LOW;
        digitalWrite(AIN1, inPin1);
        digitalWrite(AIN2, inPin2);

        delay(250);
        
        inPin1 = LOW;
       inPin2 = LOW;
        digitalWrite(AIN1, inPin1);
        digitalWrite(AIN2, inPin2);

        delay(1000);
        curract = 0; 
    }
    
    if (tmp==1){
        Serial.println("Forward");Serial.println("");
       inPin1 = LOW;
       inPin2 = HIGH;
        digitalWrite(AIN1, inPin1);
        digitalWrite(AIN2, inPin2);

        delay(250);
        
        inPin1 = LOW;
       inPin2 = LOW;
        digitalWrite(AIN1, inPin1);
        digitalWrite(AIN2, inPin2);

        delay(500);
        curract = 1; 
    }
    if (tmp==2){
        Serial.println("Turn 1");Serial.println("");
       inPin1 = LOW;
       inPin2 = HIGH;
       trnPin1 = LOW;
       trnPin2 = HIGH;
        digitalWrite(BIN1, trnPin1);
        digitalWrite(BIN2, trnPin2);
        digitalWrite(AIN1, inPin1);
        digitalWrite(AIN2, inPin2);

        delay(250);
        
        inPin1 = LOW;
        inPin2 = LOW;
        digitalWrite(AIN1, inPin1);
        digitalWrite(AIN2, inPin2);

        delay(500);
        trnPin1 = LOW;
        trnPin2 = LOW;
        digitalWrite(BIN1, trnPin1);
        digitalWrite(BIN2, trnPin2);
        curract = 2; 
    }

    if (tmp==3){
        Serial.println("Turn 2");Serial.println("");
       inPin1 = LOW;
       inPin2 = HIGH;
       trnPin1 = HIGH;
       trnPin2 = LOW;
        digitalWrite(BIN1, trnPin1);
        digitalWrite(BIN2, trnPin2);
        digitalWrite(AIN1, inPin1);
        digitalWrite(AIN2, inPin2);

        delay(250);
        
       inPin1 = LOW;
       inPin2 = LOW;
        digitalWrite(AIN1, inPin1);
        digitalWrite(AIN2, inPin2);


        delay(500);
       trnPin1 = LOW;
       trnPin2 = LOW;
        digitalWrite(BIN1, trnPin1);
        digitalWrite(BIN2, trnPin2);
        curract = 3; 
    }    

    
    

    adc0tmp = ((float)ads1115.readADC_SingleEnded(0))/10000.00;
    adc1tmp = ((float)ads1115.readADC_SingleEnded(1))/10000.00;
    adc2tmp = ((float)ads1115.readADC_SingleEnded(2))/10000.00;
    adc3tmp = ((float)ads1115.readADC_SingleEnded(3))/10000.00;
    Serial.print("Ending X Values = ");Serial.print(adc0tmp);Serial.print(",  ");Serial.print(adc1tmp);Serial.print(",  ");Serial.print(adc3tmp);Serial.print(",  ");Serial.print(adc2tmp);Serial.println("");
    
    if ((((adc0tmp>1 || adc1tmp>1) && curract  == 1) || ((adc0tmp>1.5 || adc1tmp>1.5) && curract >1) || ((adc3tmp>1.4 || adc2tmp>1.4) && curract==0)) ){
        Serial.println("Hit Something");
        _resp = 1;
          refit();

    }else{
        _resp = 0;
          refit();
    }

    Serial.println("");Serial.println("");Serial.println("");
  
}




void setxvals(){
  regArr[curract].x[0] = 1.00;

  if(curract == 0){
    for (int i = 1; i < 3; i++){
      regArr[curract].x[i] = adcArr[i+1]*adcArr[i+1]*adcArr[i+1];
    }
  }else{
    for (int i = 1; i < 3; i++){
      regArr[curract].x[i] = adcArr[i-1]*adcArr[i-1]*adcArr[i-1];
    }    
  }

  
}

int refit(){
  regArr[curract].y = (float) _resp;
//  regArr[curract].pred();

  Serial.print("Response value = ");Serial.print(regArr[curract].y);Serial.print(" is for action ");Serial.println(curract);
  
  Serial.print("Parameter Values Going IN Fitting = ");
  for(int i = 0; i < 3; i++){
    Serial.print(regArr[curract].params[i]);Serial.print(",  ");
  }
  Serial.println("");
  regArr[curract].fit();

  Serial.print("Parameter Values Coming OUT Fitting = ");
  for(int i = 0; i < 3; i++){
    Serial.print(regArr[curract].params[i]);Serial.print(",  ");
  }
  Serial.println("");
  
}


int evalAction(){


    for(int i = 0; i < 4; i++){
      regArr[i].expRewFunc();
      tmpexp[i] = regArr[i].exprew;
    }

    Serial.print("Expected Rewards are "); Serial.print(tmpexp[0]);Serial.print(", ");Serial.print(tmpexp[1]);Serial.print(", ");Serial.print(tmpexp[2]);Serial.print(", ");Serial.print(tmpexp[3]);Serial.print(", ");Serial.print(tmpexp[4]);Serial.print(", ");Serial.println("");
    Serial.print("Predictions are "); Serial.print(regArr[0]._yhat);Serial.print(", ");Serial.print(regArr[1]._yhat);Serial.print(", ");Serial.print(regArr[2]._yhat);Serial.print(", ");Serial.print(regArr[3]._yhat);Serial.print(", ");Serial.print(regArr[4]._yhat);Serial.print(", ");
    Serial.println("");  

  retval = -1;
  actExplrInd = 0;
  for(int i = 0; i < 4; i++){
    if(i == 0){
      if(movehist[i][currDiscreteGroupRear] < 10){
        actionsToExplore[actExplrInd] = i;
        actExplrInd++;
      }
    }else{
      if(movehist[i][currDiscreteGroupFront] < 10){
        actionsToExplore[actExplrInd] = i;
        actExplrInd++;
      }
    }
  }

  randseed = ((float)random(0,100))/100.00;
  Serial.println(randseed);
  Serial.println(actExplrInd);
  
  if (actExplrInd > 0){
    for(int i = 0; i < actExplrInd; i++){
        Serial.println((i+1)/actExplrInd);
      if(randseed <= ((float)(i+1))/((float)actExplrInd)){
        retval = i;
        return retval;
      }
    }
  }

    
    currMax = tmpexp[0];
    retval = 0;
  
  
    for (int i =1; i < 4; i++){
   if (tmpexp[i] > currMax){
        retval = i;
        currMax = tmpexp[i];
      }
    }

  Serial.print("Returned Value: ");Serial.println(retval);
  return retval;
}


