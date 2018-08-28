#ifndef __TRIALPARAMS_H_INCLUDED__
#define __TRIALPARAMS_H_INCLUDED__

extern int infoRewardProb;
extern int randRewardProb;
extern int odor;
extern unsigned long currentRewardTime;
extern int globalBlockSize;
extern int globalTypeBlockSize;
extern int block[];
extern int choiceInfoBlock[];
extern int choiceRandBlock[];
extern int forcedInfoBlock[];
extern int forcedRandBlock[];
extern int trialType;
extern int trialChoiceType;
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

int newTypeBlock(int);

void pickTrialParams();

int randomize(int);

#endif
