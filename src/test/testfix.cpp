#include "math.h"
#include "test/bignum.h"

// TO COMPILE DO: g++ -o test -I .. test.cpp -lssl -lcrypto

typedef int64_t CAmount;

static int FORK1HEIGHT=20000;

static int THIRTYDAYS=720*30;
static int ONEYEAR=720*365;
static int ONEYEARPLUS1=ONEYEAR+1;
static int TWOYEARS=ONEYEAR*2;

static uint64_t postRateTable[720*365+1];
static uint64_t rateTable[720*365+1];
static uint64_t bonusTable[720*365+1];

int chainHeight;

CAmount getBonusForAmount(int periods, CAmount theAmount){

    CBigNum amount256(theAmount);
    CBigNum rate256(bonusTable[periods]);
    CBigNum rate0256(bonusTable[0]);
    CBigNum result=(amount256*rate256)/rate0256;
    return result.getuint64()-theAmount;

}

CAmount getRateForAmount(int periods, CAmount theAmount){

    CBigNum amount256(theAmount);
    CBigNum rate256(rateTable[periods]);
    CBigNum rate0256(rateTable[0]);
    CBigNum result=(amount256*rate256)/rate0256;
    return  result.getuint64()-theAmount;

}

CAmount getPostRateForAmount(int periods, CAmount theAmount){

    CBigNum amount256(theAmount);
    CBigNum rate256(postRateTable[periods]);
    CBigNum rate0256(postRateTable[0]);
    CBigNum result=(amount256*rate256)/rate0256;
    return  result.getuint64()-theAmount;
}


void initRateTable(){

    rateTable[0]=1;
    rateTable[0]=rateTable[0]<<60;
    bonusTable[0]=1;
    bonusTable[0]=bonusTable[0]<<52;
    postRateTable[0]=1;
    postRateTable[0]=postRateTable[0]<<60;

    for(int i=1;i<ONEYEARPLUS1;i++)
    {

        if(chainHeight < FORK1HEIGHT)
        {
             rateTable[i]=rateTable[i-1]+(rateTable[i-1]>>18);  
             bonusTable[i]=bonusTable[i-1]+(bonusTable[i-1]>>16);
        }
        else if(chainHeight >= FORK1HEIGHT)
        {
            postRateTable[i]=postRateTable[i-1]+(postRateTable[i-1]>>20); //10% APR
            bonusTable[i]=bonusTable[i-1]+(bonusTable[i-1]>>16);
	}
    }
}

CAmount GetInterest(CAmount nValue, int outputBlockHeight, int valuationHeight, int maturationBlock){

    //These conditions generally should not occur
    if(maturationBlock >= 500000000 || outputBlockHeight<0 || valuationHeight<0 || valuationHeight<outputBlockHeight){
        return nValue;
    }

    int blocks=valuationHeight-outputBlockHeight;
    CAmount standardInterest;

    if (chainHeight < FORK1HEIGHT) 
    {
        standardInterest=getRateForAmount(blocks, nValue);
    }
    else
    {
        standardInterest=getPostRateForAmount(blocks, nValue);
    }

    CAmount bonusAmount=0;

    if(outputBlockHeight<TWOYEARS && chainHeight < FORK1HEIGHT)
    {
        //Calculate bonus rate based on outputBlockHeight
        bonusAmount=getBonusForAmount(blocks, nValue);
        CBigNum am(bonusAmount);
        CBigNum fac(TWOYEARS-outputBlockHeight);
        CBigNum div(TWOYEARS);
        CBigNum result= am - ((am*fac*fac*fac*fac)/(div*div*div*div));
        bonusAmount=result.getuint64();
	printf("Pre_Fork: Principal: %li BonusAmount: %li\n", nValue,bonusAmount);
    }
    else if(outputBlockHeight<TWOYEARS && chainHeight >= FORK1HEIGHT)
    {
	//LogPrintf("Fork: Principle:%li outputBlockHeight:%d valuationHeight:%d maturationBlock:%d", nValue, outputBlockHeight, valuationHeight, maturationBlock);
        //Calculate bonus rate based on outputBlockHeight
        bonusAmount=getBonusForAmount(blocks, nValue);
        CBigNum am(bonusAmount);
        CBigNum fac(TWOYEARS);
        CBigNum div(TWOYEARS);
        CBigNum result= ((am*fac*fac*fac*fac)/(div*div*div*div))/20; //605% One year Term Deposit Rate
        bonusAmount=result.getuint64();
	printf("Post_Fork: Principal: %li  BonusAmount: %li\n", nValue, bonusAmount);
    }
    
    CAmount interestAmount=standardInterest+bonusAmount;
    CAmount termDepositAmount=0;

    //Reward term deposits more
    if(maturationBlock>0)
    {
        int term=std::min(ONEYEAR,maturationBlock-outputBlockHeight);

        //No advantage to term deposits of less than 2 days
        if(term>720*2 && chainHeight < FORK1HEIGHT)
	{
           CBigNum am(interestAmount);
           CBigNum fac(TWOYEARS-term);
           CBigNum div(TWOYEARS);
           CBigNum result= am - ((am*fac*fac*fac*fac*fac*fac)/(div*div*div*div*div*div));
           termDepositAmount=result.getuint64();
	   printf("Pre_Fork: principal: %li termDepositAmount: %li\n", nValue, termDepositAmount);
	}    
        else if(term>720*2 && chainHeight >= FORK1HEIGHT)
	{
          CBigNum am(interestAmount);
          CBigNum fac(TWOYEARS-term);
          CBigNum div(TWOYEARS);
          CBigNum result= ((am*fac*fac*fac*fac)/(div*div*div*div));
          termDepositAmount=result.getuint64();
	  printf("Post_Fork: principal: %li termDepositAmount: %li\n", nValue, termDepositAmount);
        }
    }
    return nValue+interestAmount+termDepositAmount;
}

int main(int argc, char* argv[])
{
  if ( argc < 3 )
  {
     printf("SYNTAX: test PRINCIPAL TERM_BLOCKS BLOCKHEIGHT \n");
     exit(1);
  }

  initRateTable();
  CAmount principal= atoi(argv[1]);
  CAmount termblocks = atoi(argv[2]);
  CAmount blockheight = atoi(argv[3]);
  chainHeight = blockheight;

  CAmount bonus = getBonusForAmount(termblocks, principal);
  CAmount rate  = getRateForAmount(termblocks , principal);
  CAmount postrate = getPostRateForAmount(termblocks, principal);
  CAmount interest = GetInterest(principal, blockheight , (blockheight+termblocks), (blockheight+termblocks));

  printf("result for %li RIO at %li term_blocks BONUS:%li RATE:%li POSTRATE:%li INTEREST:%li \n",principal,termblocks,bonus,rate,postrate,interest);
}

