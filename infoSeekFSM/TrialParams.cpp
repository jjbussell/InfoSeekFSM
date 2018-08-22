#include "TrialParams.h"
#include "Arduino.h"

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
  int blockTypeCounts[4];
  int blockTypes = [2,3,4,5];
  float choiceInfoBigSize;
  float choiceInfoSmallSize;
  float choiceRandBigSize;
  float choiceRandSmallSize;
  int choiceInfoBigCount;
  int choiceInfoSmallCount;
  int choiceRandBigCount;
  int choiceRandSmallCount;
  float blockTypeSize[4];
  int blockTypeCounts[4];
  int blockTypeCount;
  int blockType;
  int startType;
  int typeStop = choiceBlockSize;
  int blockTypes[] = {2,3,4,5};
  int n;
  int temp;


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

  // make a mini-block of choice trials for both info and rand choices = choiceInfoBlock, choiceRandBlock
  // with the correct number of big vs small rewards for each type
  // cycle through based on choices

  // count of trials of each choice type in each mini block
  choiceInfoBigSize = choiceBlockCount - (float)infoRewardProb/100*choiceBlockCount;
  choiceInfoSmallSize = choiceBlockCount - choiceInfoBigSize;
  choiceRandBigSize = choiceBlockCount - (float)randrewardProb/100*choiceBlockCount;
  choiceRandSmallSize = choiceBlockCount - choiceRandBigSize;

  choiceInfoBigCount = (int)choiceInfoBigSize;
  choiceInfoSmallCount =(int)choiceInfoSmallSize;
  choiceRandBigCount = (int)choiceRandBigSize;
  choiceRandSmallCount = (int)choiceRandSmallSize;

  // assign the rewards to the info and rand choice blocks
  for (int n=0;n<choiceBlockSize;n++){
    if (n<choiceInfoBigCount){
      choiceInfoBlock[n] = 1;
    }
    else {
      choiceInfoBlock[n] = 0;
    }
    if (n<choiceRandBigCount){
      choiceRandBlock[n] = 1;
    }
    else {
      choiceRandBlock[n] = 0;
    }
  }

  // assign the count of each type of forced trial
  blockTypeSize[0] = infoBlockCount - (float)infoRewardProb/100*infoBlockCount; // info big
  blockTypeSize[1] = infoBlockCount - blockTypeSize[0]; // info small
  blockTypeSize[2] = randBlockCount-(float)randRewardProb/100*randBlockCount; // rand big
  blockTypeSize[3] = randBlockCount-blockTypeSize[2]; // rand small  

  for (int i = 0; i<4; i++){
    blockTypeCounts[i]=(int)blockTypeSize[i];
  }

  /// MAKE FULL BLOCK TO SHUFFLE=blockShuffle
  for (int i=0; i<blockSize; i++){
    blockShuffle[i]=0;
  }

  // Place choice trials
  for (int i = 0; i<choiceBlockSize; i++){
    blockShuffle[i] = 1;
  }

  // Place info and random trials
  for (int i = 0; i<4; i++){
    blockTypeCount = blockTypeCounts[i];
    blockType = blockTypes[i];
    startType = typeStop; 
    for (int j = startType; j<blockTypeCount+startType; j++){
      blockShuffle[j] = blockType;
    }
    typeStop = startType+blockTypeCount;
  }

  Serial.print("BLOCK TO SHUFFLE = ");
  for (int 1=0; i<blockSize; i++){
    Serial.print(blockShuffle[i]);
  }
  Serial.println(" ");
}

void newBlock(){

  // SHUFFLE BLOCK  

  for ( int p=0; p<blockSize; p++){
    block[p] = blockShuffle[p];
  }
  for (int i=0; i<blockSize; i++){
    n = random(0,blockSize);
    temp = block[n];
    block[n] = block[i];
    block[i] = temp;
  }

  Serial.print("NEW BLOCK = ");
  for (int 1=0; i<blockSize; i++){
    Serial.print(block[i]);
  }
  Serial.println(" ");
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



// To SET THE NUMBER OF DROPS AND WATER VALVE AND RANDOM ODOR AND REWARD IF CHOICE
void pickTrialParams(int choice){

  // REWARD SIZE IS ALREADY SET FOR FORCED TRIALS BUT NOT CHOICE!!!
  
  unsigned long bigRewardTime;
  unsigned long smallRewardTime;

  // SET REWARD SIZE FOR CHOICE TRIAL
  if (trialChoiceType == 1){
    if (choice == 1)
    {
      reward = choiceInfoBlock[choiceTrialNum];
    }
    elseif(choice == 0){
      reward = choiceRandBlock[choiceTrialNum];
    }
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