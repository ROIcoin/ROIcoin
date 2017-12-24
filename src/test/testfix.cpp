#include "math.h"
#include <string.h>
#include "test/bignum.h"
#define __STDC_FORMAT_MACROS
#include <inttypes.h>

// TO COMPILE DO: g++ -o testfix -I .. testfix.cpp  -lcrypto -std=c++11

typedef int64_t CAmount;

static int FORK1HEIGHT=20000;

static int THIRTYDAYS=720*30;
static int ONEYEAR=720*365;
static int ONEYEARPLUS1=ONEYEAR+1;
static int TWOYEARS=ONEYEAR*2;

static const CAmount COIN = 100000000;
static const CAmount CENT = 1000000;
static const CAmount MAX_MONEY = 6000000000 * COIN;
inline bool MoneyRange(const CAmount& nValue) { return (nValue >= 0 && nValue <= MAX_MONEY); }

static uint64_t rateTable[720*365+1];
static uint64_t bonusTable[720*365+1];

int chainHeight;

/*
 * This function validates the coins never to be negative or exceed MAX_MONEY
 */
CAmount validateCoins(CAmount theAmount){
    // Never return a negative amount
    if(theAmount < 0) { return CAmount(0); }

    // Never return a value larger than MAX_MONEY
    if(!MoneyRange(theAmount)) { return MAX_MONEY-1; }

    return theAmount;
}

CAmount getBonusForAmount(int periods, CAmount theAmount){

    CBigNum amount256(theAmount);
    CBigNum rate256(bonusTable[periods]);
    CBigNum rate0256(bonusTable[0]);
    CBigNum result=(amount256*rate256)/rate0256;
    printf(" getBonusForAmount(): %" PRId64 " \n", result.getuint64());
    return validateCoins(result.getuint64()-theAmount);
}

CAmount getRateForAmount(int periods, CAmount theAmount){

    CBigNum amount256(theAmount);
    CBigNum rate256(rateTable[periods]);
    CBigNum rate0256(rateTable[0]);
    CBigNum result=(amount256*rate256)/rate0256;
    printf(" getRateForAmount(): %" PRId64 " \n", result.getuint64());
    return  validateCoins(result.getuint64()-theAmount);
}

CAmount getPostRateForAmount2(int periods, CAmount theAmount){

    CBigNum  result(0);
    CBigNum  amount256(theAmount);
    double   multiplier = 0.0000005975;

    for ( int i = 1; i < periods; i++)
    {
        
        //printf(" pr2:pow: %.12f \n", pow( 1.0 + multiplier, i)); 
        result = amount256 * pow( 1.0 + multiplier, i);
    }
    printf(" getPostRateForAmount2(): %" PRId64 " \n", result.getuint64());
    return result.getuint64()-theAmount;
}

CAmount getPostRateForAmount(int periods, CAmount theAmount){

    double result;
    double multiplier = 0.0000005975;
    for ( int i = 1; i < periods; i++)
    {
	result = theAmount * pow(1.0 + multiplier, i);
    }
    printf(" getPostRateForAmount(): %.12f\n", result);
    return validateCoins(CAmount(result)-theAmount);
}


void initRateTable(){

    rateTable[0]=1;
    rateTable[0]=rateTable[0]<<60;
    bonusTable[0]=1;
    bonusTable[0]=bonusTable[0]<<52;

    for(int i=1;i<ONEYEARPLUS1;i++)
    {
        rateTable[i]=rateTable[i-1]+(rateTable[i-1]>>18);  
        bonusTable[i]=bonusTable[i-1]+(bonusTable[i-1]>>16);
    }
}

CAmount GetInterest(CAmount nValue, int outputBlockHeight, int valuationHeight, int maturationBlock){

    //These conditions generally should not occur
    if(maturationBlock >= 500000000 || outputBlockHeight<0 || valuationHeight<0 || valuationHeight<outputBlockHeight){
        return nValue;
    }

    int blocks=std::min(THIRTYDAYS,valuationHeight-outputBlockHeight);
    if(maturationBlock>0)
    {
        blocks=std::min(ONEYEAR,valuationHeight-outputBlockHeight);
    }

    CAmount standardInterest;

    if (chainHeight < FORK1HEIGHT || outputBlockHeight < FORK1HEIGHT) 
    {
        standardInterest=getRateForAmount(blocks, nValue);
    }
    else if ( chainHeight == 2000 )
    {
        standardInterest=getPostRateForAmount2(blocks, nValue);
    }
    else{
        standardInterest=getPostRateForAmount(blocks, nValue);
    }

    CAmount bonusAmount=0;

    if(outputBlockHeight<TWOYEARS && chainHeight < FORK1HEIGHT || outputBlockHeight < FORK1HEIGHT)
    {
        //Calculate bonus rate based on outputBlockHeight
        bonusAmount=getBonusForAmount(blocks, nValue);
        CBigNum am(bonusAmount);
        CBigNum fac(TWOYEARS-outputBlockHeight);
        CBigNum div(TWOYEARS);
        CBigNum result= am - ((am*fac*fac*fac*fac)/(div*div*div*div));
        bonusAmount=result.getuint64();
	printf("Pre_Fork: Bonus For Block Height: %d Principal: %" PRId64 " BonusAmount: %" PRId64 "\n", outputBlockHeight , nValue ,bonusAmount);
    }
    else if(outputBlockHeight<TWOYEARS && chainHeight >= FORK1HEIGHT)
    {
	//LogPrintf("Fork: Principle:%li outputBlockHeight:%d valuationHeight:%d maturationBlock:%d", nValue, outputBlockHeight, valuationHeight, maturationBlock);
        //Calculate bonus rate based on outputBlockHeight
        bonusAmount=getBonusForAmount(blocks, nValue);
        CBigNum am(bonusAmount);
        CBigNum fac(TWOYEARS);
        CBigNum div(TWOYEARS);
        CBigNum result= ((am*fac*fac*fac*fac)/(div*div*div*div))/10; //605% One year Term Deposit Rate
        bonusAmount=result.getuint64();
	printf("Post_Fork: Bonus For Block Height: %d Principal: %" PRId64 "  BonusAmount: %" PRId64 "\n", outputBlockHeight, nValue, bonusAmount);
    }
    
    CAmount interestAmount=standardInterest+bonusAmount;
    CAmount termDepositAmount=0;

    //Reward term deposits more
    if(maturationBlock>0)
    {
        int term=std::min(ONEYEAR,maturationBlock-outputBlockHeight);

        //No advantage to term deposits of less than 2 days
        if(term>720*2)
	{
          // Already locked coins benefit from the old pre-fork rate.
          if(chainHeight < FORK1HEIGHT || outputBlockHeight < FORK1HEIGHT)
          {
            CBigNum am(interestAmount);
            CBigNum fac(TWOYEARS-term);
            CBigNum div(TWOYEARS);
            CBigNum result= am - ((am*fac*fac*fac*fac*fac*fac)/(div*div*div*div*div*div));
            termDepositAmount=result.getuint64();
	    printf("Pre_Fork: principal: %" PRId64 " termDepositAmount: %" PRId64 "\n", nValue, termDepositAmount);
	  }    
          else
	  {
            CBigNum am(interestAmount);
            CBigNum fac(TWOYEARS-term);
            CBigNum div(TWOYEARS);
            CBigNum result= (((am*fac*fac*fac*fac)/(div*div*div*div)));
            termDepositAmount=result.getuint64();
	    printf("Post_Fork: principal: %" PRId64 " termDepositAmount: %" PRId64 "\n", nValue, termDepositAmount);
          }
       }
    }
    return validateCoins(nValue+interestAmount+termDepositAmount);
}

int main(int argc, char* argv[])
{
  if ( argc < 3 )
  {
     printf("SYNTAX: test PRINCIPAL VALUATIONHEIGHT BLOCKHEIGHT \n");
     exit(1);
  }

  CAmount principal= std::stoull(argv[1]);
  CAmount outputblock = atoi(argv[2]);
  CAmount blockheight = atoi(argv[3]);
  CAmount termblocks = blockheight-outputblock;

  chainHeight = blockheight;
  initRateTable();
  printf("calculating rate for PRINCIPAL: %" PRId64 " TERM_BLOCKS: %li VALUATIONHEIGHT: %li BLOCKHEIGHT: %li \n",principal,termblocks,outputblock,blockheight);
 
  CAmount bonus = getBonusForAmount(termblocks, principal);
  CAmount rate  = getRateForAmount(termblocks , principal);
  CAmount postrate = getPostRateForAmount(termblocks, principal);
  //CAmount postrate2 = getPostRateForAmount2(termblocks, principal);
  //CAmount interestdep = GetInterest(principal, blockheight , (blockheight+termblocks), (blockheight+termblocks));
  CAmount interest = GetInterest(principal, outputblock , blockheight , -1 );
  printf("result for %" PRId64 " RIO at %li blocks BONUS:%" PRId64 " RATE:%" PRId64 " POSTRATE:%" PRId64 " INTEREST:%" PRId64 " \n",principal,termblocks,bonus,rate,postrate,interest);
}

