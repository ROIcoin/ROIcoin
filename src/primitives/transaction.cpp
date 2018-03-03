// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "primitives/transaction.h"
#include "amount.h"
#include "pow.h"
#include "arith_uint256.h"
#include "block.h"

#include "hash.h"
#include "tinyformat.h"
#include "utilstrencodings.h"
#include "math.h"

#ifndef WIN32
# include "util.h"
#endif

#include "test/bignum.h"

int nHeight;
int chainHeight = nHeight;

std::string COutPoint::ToString() const
{
    return strprintf("COutPoint(%s, %u)", hash.ToString().substr(0,10), n);
}

CTxIn::CTxIn(COutPoint prevoutIn, CScript scriptSigIn, uint32_t nSequenceIn)
{
    prevout = prevoutIn;
    scriptSig = scriptSigIn;
    nSequence = nSequenceIn;
}

CTxIn::CTxIn(uint256 hashPrevTx, uint32_t nOut, CScript scriptSigIn, uint32_t nSequenceIn)
{
    prevout = COutPoint(hashPrevTx, nOut);
    scriptSig = scriptSigIn;
    nSequence = nSequenceIn;
}

std::string CTxIn::ToString() const
{
    std::string str;
    str += "CTxIn(";
    str += prevout.ToString();
    if (prevout.IsNull())
        str += strprintf(", coinbase %s", HexStr(scriptSig));
    else
        str += strprintf(", scriptSig=%s", scriptSig.ToString().substr(0,24));
    if (nSequence != std::numeric_limits<unsigned int>::max())
        str += strprintf(", nSequence=%u", nSequence);
    str += ")";
    return str;
}

CTxOut::CTxOut(const CAmount& nValueIn, CScript scriptPubKeyIn)
{
    nValue = nValueIn;
    scriptPubKey = scriptPubKeyIn;
}

uint256 CTxOut::GetHash() const
{
    return SerializeHash(*this);
}

std::string CTxOut::ToString() const
{
    return strprintf("CTxOut(nValue=%d.%08d, scriptPubKey=%s)", nValue / COIN, nValue % COIN, scriptPubKey.ToString().substr(0,30));
}

CMutableTransaction::CMutableTransaction() : nVersion(CTransaction::CURRENT_VERSION), nLockTime(0) {}
CMutableTransaction::CMutableTransaction(const CTransaction& tx) : nVersion(tx.nVersion), vin(tx.vin), vout(tx.vout), nLockTime(tx.nLockTime) {}

uint256 CMutableTransaction::GetHash() const
{
    return SerializeHash(*this);
}

void CTransaction::UpdateHash() const
{
    *const_cast<uint256*>(&hash) = SerializeHash(*this);
}

CTransaction::CTransaction() : nVersion(CTransaction::CURRENT_VERSION), vin(), vout(), nLockTime(0) { }

CTransaction::CTransaction(const CMutableTransaction &tx) : nVersion(tx.nVersion), vin(tx.vin), vout(tx.vout), nLockTime(tx.nLockTime) {
    UpdateHash();
}

CTransaction& CTransaction::operator=(const CTransaction &tx) {
    *const_cast<int*>(&nVersion) = tx.nVersion;
    *const_cast<std::vector<CTxIn>*>(&vin) = tx.vin;
    *const_cast<std::vector<CTxOut>*>(&vout) = tx.vout;
    *const_cast<unsigned int*>(&nLockTime) = tx.nLockTime;
    *const_cast<uint256*>(&hash) = tx.hash;
    return *this;
}

CAmount CTransaction::GetValueOut() const
{
    CAmount nValueOut = 0;
    for (std::vector<CTxOut>::const_iterator it(vout.begin()); it != vout.end(); ++it)
    {
        nValueOut += it->nValue;
        if (!MoneyRange(it->nValue) || !MoneyRange(nValueOut))
            throw std::runtime_error("CTransaction::GetValueOut(): value out of range");
    }
    return nValueOut;
}

double CTransaction::ComputePriority(double dPriorityInputs, unsigned int nTxSize) const
{
    nTxSize = CalculateModifiedSize(nTxSize);
    if (nTxSize == 0) return 0.0;

    return dPriorityInputs / nTxSize;
}

unsigned int CTransaction::CalculateModifiedSize(unsigned int nTxSize) const
{
    // In order to avoid disincentivizing cleaning up the UTXO set we don't count
    // the constant overhead for each txin and up to 110 bytes of scriptSig (which
    // is enough to cover a compressed pubkey p2sh redemption) for priority.
    // Providing any more cleanup incentive than making additional inputs free would
    // risk encouraging people to create junk outputs to redeem later.
    if (nTxSize == 0)
        nTxSize = ::GetSerializeSize(*this, SER_NETWORK, PROTOCOL_VERSION);
    for (std::vector<CTxIn>::const_iterator it(vin.begin()); it != vin.end(); ++it)
    {
        unsigned int offset = 41U + std::min(110U, (unsigned int)it->scriptSig.size());
        if (nTxSize > offset)
            nTxSize -= offset;
    }
    return nTxSize;
}

std::string CTransaction::ToString() const
{
    std::string str;
    str += strprintf("CTransaction(hash=%s, ver=%d, vin.size=%u, vout.size=%u, nLockTime=%u)\n",
        GetHash().ToString().substr(0,10),
        nVersion,
        vin.size(),
        vout.size(),
        nLockTime);
    for (unsigned int i = 0; i < vin.size(); i++)
        str += "    " + vin[i].ToString() + "\n";
    for (unsigned int i = 0; i < vout.size(); i++)
        str += "    " + vout[i].ToString() + "\n";
    return str;
}

int setNumBlock(int activeHeight)
{
    chainHeight = activeHeight;
    return chainHeight;
}

CAmount CTxOut::GetValueWithInterest(int outputBlockHeight, int valuationHeight) const{

    return GetInterest(nValue, outputBlockHeight, valuationHeight, scriptPubKey.GetTermDepositReleaseBlock());
    //return nValue;
}

static int THIRTYDAYS=720*30;
static int ONEYEAR=720*365;
static int ONEYEARPLUS1=ONEYEAR+1;
static int TWOYEARS=ONEYEAR*2;

static uint64_t rateTable[720*365+1];
static uint64_t bonusTable[720*365+1];

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
    return validateCoins(result.getuint64()-theAmount);
}

CAmount getRateForAmount(int periods, CAmount theAmount){

    CBigNum amount256(theAmount);
    CBigNum rate256(rateTable[periods]);
    CBigNum rate0256(rateTable[0]);
    CBigNum result=(amount256*rate256)/rate0256;

    return validateCoins(result.getuint64()-theAmount);
}

CAmount getPostRateForAmount(int periods, CAmount theAmount){

 if (periods <= 0) {
   return CAmount(0);
 }  
 double multiplier = 0.0000005975;
 double result = theAmount * pow(1.0 + multiplier, periods);
 CAmount rate = CAmount(result)-theAmount;
  
 return validateCoins(rate);
}

std::string initRateTable()
{
    std::string str;
    rateTable[0]=1;
    rateTable[0]=rateTable[0]<<60;
    bonusTable[0]=1;
    bonusTable[0]=bonusTable[0]<<52;

    for(int i=1;i<ONEYEARPLUS1;i++)
    {
        rateTable[i]=rateTable[i-1]+(rateTable[i-1]>>18);  
        bonusTable[i]=bonusTable[i-1]+(bonusTable[i-1]>>16);
        str += strprintf("%d %x %x\n",i,rateTable[i], bonusTable[i]);
    }

    for(int i=0;i<ONEYEAR;i++)
    {
        str += strprintf("rate: %d %d %d\n",i,getRateForAmount(i,COIN*100),getBonusForAmount(i,COIN*100));
    }

    return str;
 }
	 
CAmount GetInterest(CAmount nValue, int outputBlockHeight, int valuationHeight, int maturationBlock)

{

    //These conditions generally should not occur
    if(maturationBlock >= 500000000 || outputBlockHeight<0 || valuationHeight<0 || 	valuationHeight<outputBlockHeight)
    {
        return nValue;
    }

    // Matured coins only earn up to 30 days of interest
    int blocks=std::min(THIRTYDAYS,valuationHeight-outputBlockHeight);

    // dont allow negative blocks
    blocks=std::max(blocks,0);

    //Term deposits may have up to 1 year of interest
    if(maturationBlock>0)
    {
        blocks=std::min(ONEYEAR,valuationHeight-outputBlockHeight);
    }

    CAmount standardInterest;
    if (chainHeight < FORK1HEIGHT || outputBlockHeight < FORK1HEIGHT) 
    {
        standardInterest=getRateForAmount(blocks, nValue);
    }
    else
    {
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
	//LogPrintf("Pre_Fork: chainHeight: %d lockHeight: %d Principal: %d BonusAmount: %li\n", chainHeight,outputBlockHeight,nValue,bonusAmount);
    }
    else if(outputBlockHeight<TWOYEARS && chainHeight >= FORK1HEIGHT)
    {
         //Calculate bonus rate based on outputBlockHeight
         bonusAmount=getBonusForAmount(blocks, nValue);
         CBigNum am(bonusAmount);
         CBigNum fac(TWOYEARS);
         CBigNum div(TWOYEARS);
         CBigNum result= ((am*fac*fac*fac*fac)/(div*div*div*div))/10; //605% One year Term Deposit Rate
         bonusAmount=result.getuint64();
	 //LogPrintf("Post_Fork: chainHeight: %d lockHeight: %d Principal: %d  BonusAmount: %li\n", chainHeight, outputBlockHeight, nValue, bonusAmount);
    }

    CAmount interestAmount=standardInterest+bonusAmount;
    CAmount termDepositAmount=0;

    //Reward term deposits more
    if(maturationBlock>0)
    {
        int term=std::min(ONEYEAR,maturationBlock-outputBlockHeight);

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
	     //LogPrintf("Pre_Fork: chainHeight: %d lockHeight: %d principal: %d termDepositAmount: %li\n", chainHeight, outputBlockHeight, nValue, termDepositAmount);
	  }    
          else
	  {
             CBigNum am(interestAmount);
             CBigNum fac(TWOYEARS-term);
             CBigNum div(TWOYEARS);
             CBigNum result= ((am*fac*fac*fac*fac)/(div*div*div*div));
             termDepositAmount=result.getuint64();
	     //LogPrintf("Post_Fork: chainHeight: %d lockHeight: %d principal: %d termDepositAmount: %li\n", chainHeight, outputBlockHeight, nValue, termDepositAmount);
          }
        }
    }
#ifndef WIN32
    //LogPrintf("GetInterest: chainHeight: %d lockHeight: %d principal: %d interest: %li pos: %li deposit: %li matureblock: %d\n", chainHeight, outputBlockHeight, nValue, interestAmount, standardInterest, termDepositAmount,maturationBlock);
#endif
    return validateCoins(nValue+interestAmount+termDepositAmount);
  }
