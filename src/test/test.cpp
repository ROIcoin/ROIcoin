#include "math.h"
#include "test/bignum.h"

// TO COMPILE DO: g++ -o test -I .. test.cpp -lssl -lcrypto

typedef int64_t CAmount;

static int THIRTYDAYS=720*30;
static int ONEYEAR=720*365;
static int ONEYEARPLUS1=ONEYEAR+1;
static int TWOYEARS=ONEYEAR*2;

static uint64_t rateTable[720*365+1];
static uint64_t bonusTable[720*365+1];

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

void initRateTable(){


    rateTable[0]=1;
    rateTable[0]=rateTable[0]<<60;
    bonusTable[0]=1;
    bonusTable[0]=bonusTable[0]<<52;

    //Interest rate on each block 1+(1/2^22)
    for(int i=1;i<ONEYEARPLUS1;i++){
        rateTable[i]=rateTable[i-1]+(rateTable[i-1]>>18);
        bonusTable[i]=bonusTable[i-1]+(bonusTable[i-1]>>16);
    }
}




CAmount GetInterest(CAmount nValue, int outputBlockHeight, int valuationHeight, int maturationBlock){

    //These conditions generally should not occur
    if(maturationBlock >= 500000000 || outputBlockHeight<0 || valuationHeight<0 || valuationHeight<outputBlockHeight){
        return nValue;
    }

    int blocks=valuationHeight-outputBlockHeight;
    CAmount standardInterest=getRateForAmount(blocks, nValue);

    CAmount bonusAmount=0;
    //Reward balances more in early stages
    if(outputBlockHeight<TWOYEARS){
        //Calculate bonus rate based on outputBlockHeight
        bonusAmount=getBonusForAmount(blocks, nValue);
        CBigNum am(bonusAmount);
        CBigNum fac(TWOYEARS-outputBlockHeight);
        CBigNum div(TWOYEARS);
        CBigNum result= am - ((am*fac*fac*fac*fac)/(div*div*div*div));
        bonusAmount=result.getuint64();
    }


    CAmount interestAmount=standardInterest+bonusAmount;
    CAmount termDepositAmount=0;

    //Reward term deposits more
    if(maturationBlock>0){
        int term=maturationBlock-outputBlockHeight;

        //No advantage to term deposits of less than 2 days
        if(term>720*2){
            CBigNum am(interestAmount);
            CBigNum fac(TWOYEARS-term);
            CBigNum div(TWOYEARS);
            CBigNum result= am - ((am*fac*fac*fac*fac*fac*fac)/(div*div*div*div*div*div));
            termDepositAmount=result.getuint64();
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
  CAmount bonus = getBonusForAmount(termblocks, principal);
  CAmount rate  = getRateForAmount(termblocks , principal);
  //EXAMPLE GetInterest(principal, i, i+(ONEDAY*364), i+(ONEDAY*364))-principal)*100.0)

  CAmount interest = GetInterest(principal, blockheight , (blockheight+termblocks), (blockheight+termblocks));

  printf("result for %li RIO at %li term_blocks BONUS:%li RATE:%li INTEREST:%li \n",principal,termblocks,bonus,rate,interest);
}
