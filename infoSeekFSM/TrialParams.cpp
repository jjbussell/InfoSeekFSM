/*

--in blockSetup, create block of choiceTypes(1,2,3) and blocks of outcomes to shuffle for info and rand sides
--in newBlock, shuffle choice types
--in newTypeBlock, shuffle trialType block of outcomes-->feed in/out the blocks
--in startTrial, switch to countup choiceType and call newBlock
--in pickTrialParams, switch to set outcome, countup trialType, and call newTypeBlock

need choiceInfoTrialNum, choiceRandTrialNum, forcedInfoTrialNum, forcedRandTrialNum
need block, blockShuffle

where to put forcedInfoBlock? and block[]? define here and declare in infoseek?!?

*/

#include "TrialParams.h"
#include "Arduino.h"

const int blockSize = 12;
int blockShuffle[blockSize];

const int typeBlockSize = 8;
int infoBlockShuffle[typeBlockSize];
int randBlockShuffle[typeBlockSize];
int infoChoiceBlockShuffle[typeBlockSize];
int randChoiceBlockShuffle[typeBlockSize];
int infoForcedBlockShuffle[typeBlockSize];
int randForcedBlockShuffle[typeBlockSize];

int choiceInfoTrialNum = typeBlockSize;
int choiceRandTrialNum = typeBlockSize;
int forcedInfoTrialNum = typeBlockSize;
int forcedRandTrialNum = typeBlockSize;

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
  float infoBigCount;
  float randBigCount;


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
  for (int i = choiceBlockSize; i<choiceBlockSize + infoBlockSize; i++){
    blockShuffle[i] = 2;
  }

  // Place rand trials
  for (int r = choiceBlockSize + infoBlockSize; r<choiceBlockSize + infoBlockSize + randBlockSize; r++){
    blockShuffle[r] = 3;
  }

  Serial.print("BLOCK TO SHUFFLE = ");
  for (int r=0; r<blockSize; r=r+1){
    Serial.print(blockShuffle[r]);
    Serial.print(" ");    
  }
  Serial.println(" ");  

  /////////////////  END OF CHOICE TYPE BLOCK TO SHUFFLE

  // make a mini-block of choice trials for both info and rand choices = choiceInfoBlock, choiceRandBlock
  // with the correct number of big vs small rewards for each type
  // cycle through based on choices

  // count of trials of each choice type in each mini block
  infoBigCount = (float)infoRewardProb/100 * (float)typeBlockSize;
  randBigCount = (float)randRewardProb/100 * (float)typeBlockSize;

  // assign the rewards to the info and rand choice blocks
  for (int n=0;n<typeBlockSize;n++){
    if (n<(int)infoBigCount){
      infoBlockShuffle[n] = 1;
    }
    else {
      infoBlockShuffle[n] = 0;
    }
    if (n<(int)randBigCount){
      randBlockShuffle[n] = 1;
    }
    else {
      randBlockShuffle[n] = 0;
    }
  }

  for (int i = 0; i<typeBlockSize; i++){
    infoChoiceBlockShuffle[i] = infoBlockShuffle[i];
    infoForcedBlockShuffle[i] = infoBlockShuffle[i];
    randChoiceBlockShuffle[i] = randBlockShuffle[i];
    randForcedBlockShuffle[i] = randBlockShuffle[i];
  }

  Serial.print("INFO BLOCK TO SHUFFLE = ");
  for (int r=0; r<typeBlockSize; r++){
    Serial.print(infoBlockShuffle[r]);
    Serial.print(" ");    
  }
  Serial.println(" ");
  Serial.print("RAND BLOCK TO SHUFFLE = ");    
  for (int r=0; r<typeBlockSize; r++){
    Serial.print(randBlockShuffle[r]);
    Serial.print(" ");    
  }
  Serial.println(" ");    
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void newBlock(){

  int temp;
  int i;

//  randomSeed(analogRead(15));

  // SHUFFLE BLOCK  

  for (int w=0; w<blockSize; w++){
    block[w] = blockShuffle[w];
  }

  for (int m=blockSize-1; m>0; m--){
    i = random(0,m);
    temp = block[m];
    block[m] = block[i];
    block[i] = temp;
  }

  Serial.print("NEW BLOCK = ");
  for (int j=0; j<blockSize; j++){
    Serial.print(block[j]);
    Serial.print(" ");
  }
  Serial.println(" ");
}

void newChoiceInfoBlock(){
  int i;
  int temp;

  randomSeed(analogRead(15));

  for (int i = 0; i<typeBlockSize; i++){
    choiceInfoBlock[i] = infoChoiceBlockShuffle[i];
  }

  for (int m = typeBlockSize-1; m>0; m--){
    i = random(0,m);
    temp = choiceInfoBlock[m];
    choiceInfoBlock[m] = choiceInfoBlock[i];
    choiceInfoBlock[i] = temp;
  }

  Serial.print("NEW INFO CHOICE BLOCK = ");
  for (int j=0; j<typeBlockSize; j++){
    Serial.print(choiceInfoBlock[j]);
    Serial.print(" ");    
  }
  Serial.println(" ");
}

void newChoiceRandBlock(){
  int i;
  int temp;

  randomSeed(analogRead(15));

  for (int i = 0; i<typeBlockSize; i++){
    choiceRandBlock[i] = randChoiceBlockShuffle[i];
  }

  for (int m = typeBlockSize-1; m>0; m--){
    i = random(0,m);
    temp = choiceRandBlock[m];
    choiceRandBlock[m] = choiceRandBlock[i];
    choiceRandBlock[i] = temp;
  }

  Serial.print("NEW RAND CHOICE BLOCK = ");
  for (int j=0; j<typeBlockSize; j++){
    Serial.print(choiceRandBlock[j]);
    Serial.print(" ");    
  }
  Serial.println(" ");
}

void newForcedInfoBlock(){
  int i;
  int temp;

//  randomSeed(analogRead(15));

  for (int i = 0; i<typeBlockSize; i++){
    forcedInfoBlock[i] = infoForcedBlockShuffle[i];
  }

  for (int m = typeBlockSize-1; m>0; m--){
    i = random(0,m);
    temp = forcedInfoBlock[m];
    forcedInfoBlock[m] = forcedInfoBlock[i];
    forcedInfoBlock[i] = temp;
  }

  Serial.print("NEW INFO FORCED BLOCK = ");
  for (int j=0; j<typeBlockSize; j++){
    Serial.print(forcedInfoBlock[j]);
    Serial.print(" ");     
  }
  Serial.println(" ");
}

void newForcedRandBlock(){
  int i;
  int temp;

//  randomSeed(analogRead(15));

  for (int i = 0; i<typeBlockSize; i++){
    forcedRandBlock[i] = randForcedBlockShuffle[i];
  }

  for (int m = typeBlockSize-1; m>0; m--){
    i = random(0,m);
    temp = forcedRandBlock[m];
    forcedRandBlock[m] = forcedRandBlock[i];
    forcedRandBlock[i] = temp;
  }

  Serial.print("NEW RAND FORCED BLOCK = ");
  for (int j=0; j<typeBlockSize; j++){
    Serial.print(forcedRandBlock[j]);
    Serial.print(" ");     
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
        // choice info trial
        if (choiceInfoTrialNum == typeBlockSize){
          Serial.println("NEW CHOICE INFO BLOCK");
          newChoiceInfoBlock();
          choiceInfoTrialNum = 0;
        }
        reward = choiceInfoBlock[choiceInfoTrialNum];
        choiceInfoTrialNum++;
      }
      else if (choice == 0){
        // choice rand trial
        if (choiceRandTrialNum == typeBlockSize){
          Serial.println("NEW CHOICE RAND BLOCK");
          newChoiceRandBlock();
          choiceRandTrialNum = 0;
        }
        reward = choiceRandBlock[choiceRandTrialNum];
        choiceRandTrialNum++;        
      }
      break;
    case 2:
      // forced info trial
      if (forcedInfoTrialNum == typeBlockSize){
        Serial.println("NEW FORCED INFO BLOCK");
        newForcedInfoBlock();
        forcedInfoTrialNum = 0;
      }
      reward = forcedInfoBlock[forcedInfoTrialNum];
      forcedInfoTrialNum++;
      break;
    case 3:
      // forced random trial
      if (forcedRandTrialNum == typeBlockSize){
        Serial.println("NEW FORCED RAND BLOCK");
        newForcedRandBlock();
        forcedRandTrialNum = 0;
      }
      reward = forcedRandBlock[forcedRandTrialNum];
      forcedRandTrialNum++;
      break;
    }

    Serial.print("REWARD = ");
    Serial.println(reward);
  
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
