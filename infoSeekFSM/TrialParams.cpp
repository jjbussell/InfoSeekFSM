/*

--in blockSetup, create block of choiceTypes(1,2,3) and blocks of outcomes to shuffle for info and rand sides
--in newBlock, shuffle choice types
--in newTypeBlock, shuffle trialType block of outcomes-->feed in/out the blocks
--in startTrial, switch to countup choiceType and call newBlock
--in pickTrialParams, switch to set outcome, countup trialType, and call newTypeBlock

need trialNum, choiceInfoTrialNum, choiceRandTrialNum, forcedInfoTrialNum, forcedRandTrialNum
need block, blockShuffle

*/




#include "TrialParams.h"
#include "Arduino.h"

const int blockSize = globalBlockSize;
int blockShuffle[blockSize];

const int typeBlockSize = globalTypeBlockSize;
int infoBlockShuffle[typeBlockSize];
int randBlockShuffle[typeBlockSize]

/////// REMEMBER TO CHANGE BLOCK COUNT IN SWITCHING TO NEW BLOCK AT TRIAL START!!!
////// ALSO REMEMBER TO COUNT UP CHOICE TRIALS!!!!

// BLOCK SETUP RUNS ONCE AT SESSION START (SET CHOICE TRIAL COUNT)
// SHUFFLE BLOCKS AT EACH NEW BLOCK

void blockSetup(void){
  float choicePercent;
  float infoPercent;
  float randPercent;
  float infoBlockCount;
  float randBlockCount;
  float choiceBlockCount;
  int choiceBlockSize;
  int infoBlockSize;
  int randBlockSize;
  int infoBigCount;
  int randBigCount;


  randomSeed(analogRead(15));


  // determine fraction of each trial type (info, random, choice) based on trialTypes input
  switch (trialTypes) {
    case 1: // choice
      choicePercent = 1; infoPercent = 0; randPercent = 0;
      break;
    case 2: // forced info
      choicePercent = 0; infoPercent = 1; randPercent = 0;
      break;
    case 3: // forced rand
      choicePercent = 0; infoPercent = 0; randPercent = 1;
      break;
    case 4: // forced info and forced rand alternating
      choicePercent = 0; infoPercent = 0.5; randPercent = 0.5;
      break;
    case 5: // all three alternating
      choicePercent = 0.334; infoPercent = 0.334; randPercent = 0.334;
      break;
    case 6: // biased, hardcode which
      choicePercent = 0; infoPercent = 0.85; randPercent = 0.15;
      break;
    case 7: // choice training info
      choicePercent = 0.5; infoPercent = 0.5; randPercent = 0;
      break;
    case 8: // choice training rand
      choicePercent = 0.5; infoPercent = 0; randPercent = 0.5;
      break;
  }


  // count of trials of each choice type in block
  randBlockCount = randPercent * (float)blockSize;
  infoBlockCount = infoPercent * (float)blockSize;
  choiceBlockCount = choicePercent * (float)blockSize;
  choiceBlockSize = (int)choiceBlockCount;
  infoBlockSize = (int)infoBlockCount;
  randBlockSize = (int)randBlockCount;

    // Place choice trials
  for (int  k= 0; k<choiceBlockSize; k++){
    blockShuffle[k] = 1;
  }

  // Place info trials
  for (int i = choiceBlockSize; i<infoBlockSize; i++){
    blockShuffle[i] = 2;
  }

  // Place rand trials
  for (int r = choiceBlockSize + infoBlockSize; r<randBlockSize; r++){
    blockShuffle[r] = 3;
  }

  /////////////////  END OF CHOICE TYPE BLOCK TO SHUFFLE

  // make a mini-block of choice trials for both info and rand choices = choiceInfoBlock, choiceRandBlock
  // with the correct number of big vs small rewards for each type
  // cycle through based on choices

  // count of trials of each choice type in each mini block
  infoBigCount = infoRewardProb * typeBlockSize;
  randBigCount = randRewardProb * typeBlockSize;

  // assign the rewards to the info and rand choice blocks
  for (int n=0;n<typeBlockSize;n++){
    if (n<infoBigCount){
      infoBlockShuffle[n] = 1;
    }
    else {
      infoBlockShuffle[n] = 0;
    }
    if (n<randBigCount){
      randBlockShuffle[n] = 1;
    }
    else {
      randBlockShuffle[n] = 0;
    }
  }

  Serial.print("INFO BLOCK TO SHUFFLE = ");
  for (int r=0; r<blockSize; r=r+1){
    Serial.print(infoBlockShuffle[r]);
  }
  Serial.println(" ");
  Serial.print("RAND BLOCK TO SHUFFLE = ");    
  for (int r=0; r<blockSize; r=r+1){
    Serial.print(randBlockShuffle[r]);
  }
  Serial.println(" ");    

  Serial.print("BLOCK TO SHUFFLE = ");
  for (int r=0; r<blockSize; r=r+1){
    Serial.print(blockShuffle[r]);
  }
  Serial.println(" ");
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void newBlock(){

  int n;
  int temp;

  randomSeed(analogRead(15));

  // SHUFFLE BLOCK  

  for (int w=0; w<blockSize; w++){
    block[w] = blockShuffle[w];
  }

  for (int i=0; i<blockSize; i++){
    n = random(0,blockSize);
    temp = block[n];
    block[n] = block[i];
    block[i] = temp;
  }

  Serial.print("NEW BLOCK = ");
  for (int j=0; j<blockSize; j++){
    Serial.print(block[j]);
  }
  Serial.println(" ");
}

int newTypeBlock(int typeBlockShuffle){
  int n;
  int temp;

  randomSeed(analogRead(15));

  for (int i = 0; i<typeBlockSize; i++){
    typeBlock[i] = typeBlockShuffle[i];
  }

  for (int i = 0; i<typeBlockSize; i++){
    n = random(0,typeBlockSize);
    temp = typeBlock[n];
    typeBlock[n] = typeBlock[i];
    typeBlock[i] = temp;
  }

  Serial.print("NEW TYPE BLOCK = ");
  for (int j=0; j<blockSize; j++){
    Serial.print(typeBlock[j]);
  }
  Serial.println(" ");

}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



// To SET THE NUMBER OF DROPS AND WATER VALVE AND RANDOM ODOR AND REWARD IF CHOICE
void pickTrialParams(){


  unsigned long bigRewardTime;
  unsigned long smallRewardTime;

  // SET TRIAL TYPE FROM TRIAL CHOICE TYPE BLOCKS

  switch(trialChoiceType){
    case 1:
      if (choice == 1){
        trialType = 1; // choice info trial
        if (choiceInfoTrialNum == typeBlockSize){
          choiceInfoBlock = newTypeBlock(infoBlockShuffle);
        }
        reward = choiceInfoBlock[choiceInfoTrialNum];
        choiceInfoTrialNum++;
      }
      else if (choice == 0){
        trialType = 0; // choice info trial
        if (choiceRandTrialNum == typeBlockSize){
          choiceRandBlock = newTypeBlock(randBlockShuffle);
        }
        reward = choiceRandBlock[choiceRandTrialNum];
        choiceRandTrialNum++;        
      }
      break;
    case 2:
      trialType = 2; // forced info trial
      if (forcedInfoTrialNum == typeBlockSize){
        forcedInfoBlock = newTypeBlock(infoBlockShuffle);
      }
      reward = forcedInfoBlock[forcedInfoTrialNum];
      forcedInfoTrialNum++;
      break;
    case 3:
      trialType = 3; // forced random trial
      if (forcedRandTrialNum == typeBlockSize){
        forcedRandBlock = newTypeBlock(randBlockShuffle);
      }
      reward = forcedRandBlock[forcedRandTrialNum];
      forcedRandTrialNum++;
      break;
    }
  
  // vals based on choice
  if (choice == 1){
    water = infoWater; // which valve
    bigRewardTime = infoBigRewardTime;
    smallRewardTime = infoSmallRewardTime;
  }
  else if (choice == 0){
    water = randWater;
    bigRewardTime = randBigRewardTime;
    smallRewardTime = randSmallRewardTime;
  }
  else {
    water = infoWater;
    smallRewardTime = infoSmallRewardTime;
    bigRewardTime = infoBigRewardTime;
  }

  if (reward == 1) {
    currentRewardTime = bigRewardTime;
  }
  else {
    currentRewardTime = smallRewardTime;
  }

  int odorPick;
  odorPick = randomize(randRewardProb); // 1 = OdorC, 0 = OdorD
  if ((choice == 1) && (reward == 1)) {
    odor = odorA;
  }
  else if ((choice == 1) && (reward == 0)) {
    odor = odorB;
  }
  else if ((choice == 0) && (odorPick == 1)) {
    odor = odorC;
  }
  else if ((choice == 0) && (odorPick == 0)) {
    odor = odorD;
  }
  else { odor = 7;}
}

// To randomly determine big or small reward (1, 0 respectively)
// based on reward probability
int randomize(int prob) {
  int randVal = 0;
  long randNum = random(100);
  if (randNum < prob) {
    randVal = 1;
  }
  return randVal;
} 
