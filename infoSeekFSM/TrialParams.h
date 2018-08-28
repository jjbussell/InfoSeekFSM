#ifndef __TRIALPARAMS_H_INCLUDED__
#define __TRIALPARAMS_H_INCLUDED__

extern int infoRewardProb;
extern int randRewardProb;
extern int odor;
extern unsigned long currentRewardTime;
extern int globalBlockSize;
extern int globalTypeBlockSize;
extern int block[24];
extern int choiceInfoBlock[8];
extern int choiceRandBlock[8];
extern int forcedInfoBlock[8];
extern int forcedRandBlock[8];
extern int trialChoiceType;
extern int choice;
extern int trialTypes;
extern int reward;
extern unsigned long  infoBigRewardTime;
extern unsigned long infoSmallRewardTime;
extern unsigned long  randBigRewardTime;
extern unsigned long randSmallRewardTime;
extern int odorA;
extern int odorB;
extern int odorC;
extern int odorD;
extern int water;
extern int infoWater;
extern int randWater;



void blockSetup(void);

void newBlock(void);

int [8] newTypeBlock(int [8]);

void pickTrialParams();

int randomize(int);

#endif
