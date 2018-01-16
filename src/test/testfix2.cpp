#include "math.h"
#include <string.h>
#include "test/bignum.h"
#define __STDC_FORMAT_MACROS
#include <inttypes.h>
//#include <stdint.h>
#include <sstream>
#include <iostream>

using namespace std;

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

CAmount getBonusForAmount(int periods, CAmount theAmount){

    CBigNum amount256(theAmount);
    CBigNum rate256(bonusTable[periods]);
    CBigNum rate0256(bonusTable[0]);
    CBigNum result=(amount256*rate256)/rate0256;
    //printf(" getBonusForAmount(): %" PRId64 " \n", result.getuint64());
    return result.getuint64()-theAmount;
}

CAmount getRateForAmount(int periods, CAmount theAmount){

    CBigNum amount256(theAmount);
    CBigNum rate256(rateTable[periods]);
    CBigNum rate0256(rateTable[0]);
    CBigNum result=(amount256*rate256)/rate0256;
    //printf(" getRateForAmount(): %" PRId64 " \n", result.getuint64());
    return  result.getuint64()-theAmount;
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
    
    // dont allow negative blocks
    blocks=std::max(blocks,0);

    CAmount standardInterest=getRateForAmount(blocks, nValue);
    CAmount bonusAmount=0; // NO BONUS
    
    CAmount interestAmount=standardInterest+bonusAmount;
    CAmount termDepositAmount=0;

    //Reward term deposits more
    if(maturationBlock>0)
    {
        int term=std::min(ONEYEAR,maturationBlock-outputBlockHeight);

        //No advantage to term deposits of less than 2 days
        if(term>720*2)
	{
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
     printf("SYNTAX: test PRINCIPAL VALUATIONHEIGHT BLOCKHEIGHT \n");
     exit(1);
  }

  CAmount principal= std::stoull(argv[1]);
  CAmount outputblock = atoi(argv[2]);
  CAmount blockheight = atoi(argv[3]);
  CAmount termblocks = blockheight-outputblock;

  chainHeight = blockheight;
  initRateTable();
  //printf("calculating rate for PRINCIPAL: %" PRId64 " TERM_BLOCKS: %li VALUATIONHEIGHT: %li BLOCKHEIGHT: %li \n",principal,termblocks,outputblock,blockheight);
 
  CAmount bonus = getBonusForAmount(termblocks, principal);
  CAmount rate  = getRateForAmount(termblocks , principal);
  
  CAmount interest = GetInterest(principal, outputblock , blockheight , termblocks );
  CAmount interestOnly = interest - principal;
  double interestRateForTime=(0.0+interestOnly)/(principal);
  double fractionOfaYear=262800.0/termblocks;
  double interestRatePerBlock=pow(1+interestRateForTime,1.0/termblocks)-1;

  std::ostringstream ss;
  
  ss << "\nResult for principal of " << (0.0+principal)/COIN <<" ROI ";
  ss << "for " << termblocks << " blocks.\n";
  ss << "This is approximately " << (0.0+termblocks)/(720) << " days.\n";
  ss << "Maturation amount: " << (0.0+interest)/COIN << " ROI.\n";
  ss << " IRPB = " << interestRatePerBlock;
  ss << "\n IRFT = " << interestRateForTime;
  ss << "\n APR = " << (pow(1+interestRateForTime,fractionOfaYear)-1)*100 <<"%\n";
  cout << ss.str();
 
}

