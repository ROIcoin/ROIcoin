#include "math.h"
#include <string.h>
#include "test/bignum.h"
#define __STDC_FORMAT_MACROS
#include <inttypes.h>
//#include <stdint.h>
#include <sstream>
#include <iostream>

using namespace std;

// TO COMPILE DO: g++ -o ratecheck -I .. ratecheck.cpp  -lcrypto -std=c++11

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

int chainHeight;

CAmount getRateForAmount(int periods, CAmount theAmount){

    CBigNum amount256(theAmount);
    CBigNum rate256(rateTable[periods]);
    CBigNum rate0256(rateTable[0]);
    CBigNum result=(amount256*rate256)/rate0256;
    printf(" getRateForAmount(): %" PRId64 " \n", result.getuint64());
    return  result.getuint64()-theAmount;
}

void initRateTable(){

    rateTable[0]=1;
    rateTable[0]=(rateTable[0]<<60);

    for(int i=1;i<ONEYEARPLUS1;i++)
    {
        rateTable[i]=rateTable[i-1]+(rateTable[i-1]>>18)+(rateTable[i-1]>>19);  
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
    CAmount termDepositAmount=0;

    //Reward term deposits more
    if(maturationBlock>0)
    {
        int term=std::min(ONEYEAR,maturationBlock-outputBlockHeight);

        //No advantage to term deposits of less than 2 days
        if(term>720*2)
	{
            CBigNum am(standardInterest);
            CBigNum fac(TWOYEARS-term);
            CBigNum div(TWOYEARS);
            CBigNum result= am - ((am*fac*fac*fac*fac*fac*fac)/(div*div*div*div*div*div));
            termDepositAmount=result.getuint64();
        }
    }
    return nValue+standardInterest+termDepositAmount;
}

int main(int argc, char* argv[])
{
  if ( argc < 3 )
  {
     printf("SYNTAX: ratecheck  PRINCIPAL VALUATIONHEIGHT BLOCKHEIGHT TYPEFLAG[1=POS/0=TD] \n");
     exit(1);
  }

  CAmount principal= std::stoull(argv[1]);
  CAmount outputblock = atoi(argv[2]);
  CAmount blockheight = atoi(argv[3]);
  CAmount termblocks = blockheight-outputblock;
  int pos = atoi(argv[4]);
  if (pos == 1) {
    termblocks = -1;
  }

  chainHeight = blockheight;
  initRateTable();
  //printf("calculating rate for PRINCIPAL: %" PRId64 " TERM_BLOCKS: %li VALUATIONHEIGHT: %li BLOCKHEIGHT: %li \n",principal,termblocks,outputblock,blockheight);
 
  CAmount rate  = getRateForAmount(termblocks , principal);
  
  CAmount interest = GetInterest(principal, outputblock , blockheight , termblocks );
  CAmount interestOnly = interest - principal;
  
  double interestRateForTime=(0.0+interestOnly)/(principal);
  double fractionOfaYear=262800.0/termblocks;
  double interestRatePerBlock=pow(1+interestRateForTime,1.0/termblocks)-1;

  std::ostringstream ss;
  
  if ( termblocks != -1 ){ 
    ss << "\nResult for TD principal of " << (0.0+principal)/COIN <<" ROI ";
    ss << "for " << termblocks << " blocks.\n";
    ss << "This is approximately " << (0.0+termblocks)/(720) << " days.\n";
    ss << "Maturation amount: " << (0.0+interest)/COIN << " ROI.\n";
    ss << " IRPB = " << interestRatePerBlock;
    ss << "\n IRFT = " << interestRateForTime*100 << "%";
    ss << "\n APR = " << (pow(1+interestRateForTime,fractionOfaYear)-1)*100 <<"%\n";
  cout << ss.str();
  } else {
    ss << "\nResult for POS principal of " << (0.0+principal)/COIN <<" ROI ";
    ss << "for " << (blockheight-outputblock) << " blocks.\n";
    ss << "This is approximately " << (0.0+(blockheight-outputblock))/(720) << " days.\n";
    ss << "Maturation amount: " << (0.0+interest)/COIN << " ROI.\n";
    ss << "\n IRFT = " << interestRateForTime*100 << "% \n";
    cout << ss.str();
  }

 
}

