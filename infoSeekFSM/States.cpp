#include "States.h"
#include "Arduino.h"


//// INTER_TRIAL_INTERVAL 15
void StateInterTrialInterval::s_setup()
{
//  Serial.println("ITI");
//  Serial.println("setting duration");
  set_duration(interval + random(1,1000));
  Serial.print("Completed Trials: ");
  Serial.println(cTCount);
  Serial.print("Forced Info Trials: ");
  Serial.print(infoFCt);
  Serial.print("   Forced Rand Trials: ");
  Serial.println(randFCt);
  Serial.print("Choice Info Trials: ");
  Serial.print(infoCCt);
  Serial.print("   Choice Rand Trials: ");
  Serial.println(randCCt);
  Serial.print("Reward Amount: ");
  Serial.println(rewardAmt);
}

void StateInterTrialInterval::s_finish()
{
//  Serial.println("ending ITI, move to WAIT_FOR_TRIAL");
  next_state = WAIT_FOR_TRIAL;   
}

//// WAIT_FOR_TRIAL 0
void StateWaitForTrial::s_setup()
{
//  Serial.println("WAIT_FOR_TRIAL");
  trialStart = 0;
  rxn = 0;
  choiceStart = 0;
  choice = 2;
  trialCt++;
}

void StateWaitForTrial::s_finish()
{
//  Serial.println("end wait for trial, move to START_TRIAL");
  next_state = START_TRIAL;
}


//// START_TRIAL_DELAY 2
void StateStartTrialDelay::s_setup()
{
//  Serial.println("DELAY for beep");
}

void StateStartTrialDelay::s_finish()
{
//  Serial.println("end start trial delay, move to WAIT_FOR_CENTER");
//  Serial.println("WAIT_FOR_CENTER");
  digitalWrite(LED,LOW);
  digitalWrite(startGPIO,LOW);
  next_state = WAIT_FOR_CENTER;
}


//// CENTER_DELAY 4
void StateCenterDelay::s_setup()
{
//  Serial.println("CENTER_DELAY");
  printer(13,0,0);
}

void StateCenterDelay::loop()
{
  if (centerFlag == 0){
    Serial.println("WAIT_FOR_CENTER");
    timer = 0;
    next_state = WAIT_FOR_CENTER;
  }
}

void StateCenterDelay::s_finish()
{
//  Serial.println("ending center odor delay, move to CENTER_ODOR");
  next_state = CENTER_ODOR;
}

//// CENTER_ODOR 5
void StateCenterOdor::s_setup()
{
  centerOdorOn(centerOdor);
}

void StateCenterOdor::loop()
{
  if (centerFlag == 0){
    Serial.println("WAIT_FOR_CENTER");
    if (centerOdorValveOpen == 1){   
      centerOdorOff(centerOdor);
    }
    timer = 0;
    next_state = WAIT_FOR_CENTER;
  }
}

void StateCenterOdor::s_finish()
{
  if (centerOdorValveOpen == 1){
    centerOdorOff(centerOdor);
  }
  next_state = CENTER_POSTODOR_DELAY;
}

//// CENTER_POSTODOR_DELAY 6
void StateCenterPostOdorDelay::s_setup()
{
//  Serial.println("CENTER_POSTODOR_DELAY");
}
void StateCenterPostOdorDelay::loop()

{
  if (centerFlag == 0){
    Serial.println("WAIT_FOR_CENTER");
    timer = 0;
    next_state = WAIT_FOR_CENTER;
  }
}

void StateCenterPostOdorDelay::s_finish()
{
//  Serial.println("end start delay, move to GO_CUE");
  next_state = GO_CUE;
}


//// GO_CUE_DELAY 8
void StateGoCueDelay::s_setup()
{
//  Serial.println("DELAY for beep");
}

void StateGoCueDelay::s_finish()
{
//  Serial.println("end go cue delay, move to RESPONSE");
  next_state = RESPONSE;
}


//// RESPONSE 9
void StateResponse::s_setup()
{
//  digitalWrite(LED,LOW);
  choiceStart = millis()-startTime;
//  Serial.println("RESPONSE");
  
//  Serial.print("choiceStart = ");
//  Serial.println(choiceStart);
    
  printer(1,trialChoiceType,infoSide);
}

void StateResponse::loop()
{
  if (infoFlag == 1){
    if(trialChoiceType == 1 || trialChoiceType == 2){
      Serial.println("CHOICE INFO");
      choice = 1;
    }
    else {
      choice = 3;
      Serial.println("INCORRECT");
    }
    rxn = millis() - startTime;
//    Serial.print("rxn = ");
//    Serial.println(rxn);
     // REPORT THE RESPONSE AFTER ENTRY 0/1 = correct port, 2 = no choice, 3 = incorrect  
    printer(11,choice,0);
    if (choice < 2) {   
      newTrial = 1;
    }
    flag_stop = 1;
//    next_state = WAIT_FOR_ODOR;
  }
  else if (randFlag == 1){
    if(trialChoiceType == 1 || trialChoiceType == 3){
      Serial.println("CHOICE RAND");
      choice = 0;
 
    }
    else {
      choice = 3;
      Serial.println("INCORRECT");
    }
    rxn = millis() - startTime;
//    Serial.print("rxn = ");
//    Serial.println(rxn);
     // REPORT THE RESPONSE AFTER ENTRY 0/1 = correct port, 2 = no choice, 3 = incorrect  
    printer(11,choice,0);       
    if (choice < 2) {   
      newTrial = 1;
    }
    flag_stop = 1;
//    next_state = WAIT_FOR_ODOR;   
  }
}

void StateResponse::s_finish()
{
  if (choice == 2){
    next_state = GRACE_PERIOD;
  }
  else {
    next_state = WAIT_FOR_ODOR;
  }
}


//// GRACE_PERIOD 10
void StateGracePeriod::s_setup()
{
//  Serial.println("GRACE_PERIOD ");
}

void StateGracePeriod::loop()
{
  if (choice ==2){
    if (infoFlag == 1){
      if(trialChoiceType == 1 || trialChoiceType == 2){
        Serial.println("CHOICE INFO");
        choice = 1;
      }
      else {
        choice = 3;
        Serial.println("INCORRECT");
      }
      rxn = millis() - startTime;   
  //    Serial.print("rxn = ");
  //    Serial.println(rxn);
       // REPORT THE RESPONSE AFTER ENTRY 0/1 = correct port, 2 = no choice, 3 = incorrect  
      printer(11,choice,0);          
      if (choice < 2) {   
        newTrial = 1;
      }
      flag_stop = 1;
    }
    else if (randFlag == 1){
      if(trialChoiceType == 1 || trialChoiceType == 3){
        Serial.println("CHOICE RAND");
        choice = 0;   
      }
      else {
        choice = 3;
        Serial.println("INCORRECT");
      }
      rxn = millis() - startTime;
  //    Serial.print("rxn = ");
  //    Serial.println(rxn);
       // REPORT THE RESPONSE AFTER ENTRY 0/1 = correct port, 2 = no choice, 3 = incorrect  
      printer(11,choice,0);           
      if (choice < 2) {   
        newTrial = 1;
      }
      flag_stop = 1;
    }
  }  
}

void StateGracePeriod::s_finish()
{
  if (choice == 2){
    Serial.println("no choice, TIMEOUT");
    next_state = TIMEOUT;
  }
  else {
//    Serial.println("end GRACE, move to SIDE_ODOR");
    next_state = SIDE_ODOR;
  }
}


//// WAIT FOR ODOR 11
void StateWaitForOdor::s_setup()
{
  Serial.println("WAIT FOR ODOR");
//  Serial.print("Time is ");
//  Serial.println(currentTime);
  
//  unsigned long test = rxn - choiceStart;
//  Serial.print("rxn - choiceStart ");
//  Serial.println(test);
//  Serial.print("odorDelay + gracePeriod ");
//  Serial.println(odorDelay + gracePeriod);
//  Serial.print("time should be ");
//  Serial.println((odorDelay + gracePeriod) - (test));
  if ((rxn - choiceStart) > odorDelay) {
    set_duration(1);
  }
  else{
    set_duration(odorDelay - (rxn - choiceStart));      
  }
  
}

void StateWaitForOdor::s_finish()
{
//  Serial.println("finish odor delay, move to SIDE_ODOR");
  next_state = SIDE_ODOR;
}


//// SIDE_ODOR 12
void StateSideOdor::s_setup()
{
//  Serial.println("SIDE_ODOR");
//  Serial.print("Time is ");
//  Serial.println(currentTime);
  if (choice < 2){
//    Serial.println("choose trial params");
    pickTrialParams();
    printer(17,reward,odor);
//    Serial.println("Side odor on");
    if(choice == 1){
        controlOff(infoControl);
      }
      else {
        controlOff(randControl);
    }
    odorOn(odor);
  }
}

void StateSideOdor::s_finish()
{
//  Serial.println("side odor off");
  if (choice <2){
//    Serial.println("Side odor off");
    if (odorValveOpen == 1){
      odorOff(odor);
      if(choice == 1){
          controlOn(infoControl);
        }
      else {
          controlOn(randControl);
      }      
    }
  }
  
  next_state = REWARD_DELAY;
}

//// REWARD_DELAY 13
void StateRewardDelay::s_setup()
{
//  Serial.println("REWARD_DELAY");
  buzzInterval = rewardDelay/10;
  change = 0;
  buzzCt = 0;
  lastBuzzCt = 0;
}

void StateRewardDelay::loop(){

  if (choice < 2){

    //check if time to turn buzzer on
    if (currentTime >= lastBuzzerOff + buzzInterval) {
      tone(buzzer,8000);
//      Serial.print("t ");
//      Serial.print(currentTime);
//      Serial.println(" buzz");
      buzzCt++;
      lastBuzzerOn = currentTime;
      lastBuzzerOff = 1000000000000000000000;
    }

    // check if time to turn buzzer off
    if (currentTime >= lastBuzzerOn + 20) {
      noTone(buzzer);
      lastBuzzerOff = currentTime;
//      Serial.print("off ");
//      Serial.println(currentTime);
      lastBuzzerOn = 1000000000000000000000;
    }

    // check if time to decrease interval
    if (currentTime % (1000) == 0 & change == 0 ){
      buzzInterval = buzzInterval - 100;
//      Serial.print("I ");
//      Serial.print(currentTime);
//      Serial.print("interval ");
//      Serial.println(buzzInterval);
      change = 1;
      lastBuzzCt = buzzCt;
    }

    if (currentTime % (1000) != 0 & change == 1){
      change = 0;
    }
  }
}

void StateRewardDelay::s_finish()
{
//  Serial.println("end Reward delay, move to REWARD");

  noTone(buzzer);
  int port;
  port = 5;
  
//  Serial.print("reward delay end ");
//  Serial.println(currentTime);
//
// Serial.print("current reward time (drops) ");
// Serial.println(currentRewardTime);

 rewardDropCount = maxRewardDrops;
  
  if (choice < 2){
    if (infoFlag == 1){
//      Serial.println("in info port");
      port = 1;
    }
    else if (randFlag == 1){
//      Serial.println("in rand port");
      port = 0;
    }
    else {
      port = 5;
    }

//    Serial.print("port = ");
//    Serial.println(port);
//    Serial.print("choice = ");
//    Serial.println(choice);

    // Serial.print("lickRate = ");
    // Serial.println(lickRate);

//    if (port == choice && lickRate > 0){
    if (port == choice){
      rewardCount++;
      rewardDrops = currentRewardTime;
//      Serial.print("reward drops = ");
//      Serial.println(rewardDrops);

      if (reward == 1) {
        rewardBigCount++;
        Serial.println("Big reward");
        printer(15, trialChoiceType, choice);
        
      }
      else {
        rewardSmallCount++;
        Serial.println("Small reward");
        printer(16, trialChoiceType, choice);
      }
    }
  }

  next_state = DELIVER_REWARD;
}


//// TIMEOUT 16
void StateTimeout::s_setup(){
  printer(11,choice,0);
  Serial.println("TIMEOUT");
}

void StateTimeout::s_finish()
{
  next_state = INTER_TRIAL_INTERVAL;
}

//// REWARD_PAUSE 17
void StateRewardPause::s_setup(){
  Serial.println("REWARD PAUSE");
}

void StateRewardPause::s_finish(){
  next_state = DELIVER_REWARD;
}


