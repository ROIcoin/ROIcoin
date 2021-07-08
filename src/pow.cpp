// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2015 The Bitcoin Core developers
// MIDAS difficulty algorithm Copyright (c) 2015 Ray Dillinger (added by HFP0)
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "pow.h"

#include "arith_uint256.h"
#include "chain.h"
#include "primitives/block.h"
#include "uint256.h"
#include "util.h"

#include <math.h>
#include <stdint.h> 

void avgRecentTimestamps(const CBlockIndex* pindexLast, int64_t *avgOf5, int64_t *avgOf7, int64_t *avgOf9, int64_t *avgOf17, const Consensus::Params& params)
{
  int blockoffset = 0;
  int64_t oldblocktime;
  int64_t blocktime;

  *avgOf5 = *avgOf7 = *avgOf9 = *avgOf17 = 0;
  if (pindexLast)
    blocktime = pindexLast->GetBlockTime();
  else blocktime = 0;

  for (blockoffset = 0; blockoffset < 17; blockoffset++)
  {
    oldblocktime = blocktime;
    if (pindexLast)
    {
      pindexLast = pindexLast->pprev;
      blocktime = pindexLast->GetBlockTime();
    }
    else
    { // genesis block or previous
    blocktime -= params.nPowTargetSpacing;
    }
    // for each block, add interval.
    if (blockoffset < 5) *avgOf5 += (oldblocktime - blocktime);
    if (blockoffset < 7) *avgOf7 += (oldblocktime - blocktime);
    if (blockoffset < 9) *avgOf9 += (oldblocktime - blocktime);
    *avgOf17 += (oldblocktime - blocktime);
  }
  // now we have the sums of the block intervals. Division gets us the averages.
  *avgOf5 /= 5;
  *avgOf7 /= 7;
  *avgOf9 /= 9;
  *avgOf17 /= 17;
}

unsigned int GetNextWorkRequiredMIDAS(const CBlockIndex *pindexLast, const CBlockHeader *pblock, const Consensus::Params& params)
{
    int64_t avgOf5;
    int64_t avgOf9;
    int64_t avgOf7;
    int64_t avgOf17;
    int64_t toofast;
    int64_t tooslow;
    int64_t difficultyfactor = 10000;
    int64_t now;
    int64_t BlockHeightTime;

    int64_t nFastInterval = (params.nPowTargetSpacing * 9 ) / 10; // seconds per block desired when far behind schedule
    int64_t nSlowInterval = (params.nPowTargetSpacing * 11) / 10; // seconds per block desired when far ahead of schedule
    int64_t nIntervalDesired;

    const arith_uint256 bnPowLimit = UintToArith256(params.powLimitResetAtFork);
    unsigned int nProofOfWorkLimit = UintToArith256(params.powLimitResetAtFork).GetCompact();

#if HFP0_DEBUG_DIF
    // HFP0 DBG begin
    LogPrintf("HFP0 DIF: MIDAS init: nProofOfWorkLimit: %08x  %s\n", nProofOfWorkLimit, bnPowLimit.ToString());
    // HFP0 DBG end
#endif

    if (pindexLast == NULL) {
        // Genesis Block
#if HFP0_DEBUG_DIF
        // HFP0 DBG begin
        LogPrintf("HFP0 DIF: MIDAS at genesis returning POW limit\n");
        // HFP0 DBG end
#endif
        return nProofOfWorkLimit;
    }

    // special testnet rules
    /* disabled for testing MIDAS on testnet */
    if (params.fPowAllowMinDifficultyBlocks)
    {
#if HFP0_DEBUG_DIF
        // HFP0 DBG begin
        LogPrintf("HFP0 DIF: MIDAS in testnet (fPowAllowMinDifficultyBlocks)\n");
        // HFP0 DBG end
#endif
        // Special difficulty rule for testnet: If the new block's timestamp is more than 2* nPowTargetSpacing then allow
        // mining of a min-difficulty block.
        if (pblock->nTime > pindexLast->nTime + params.nPowTargetSpacing * 2) {
#if HFP0_DEBUG_DIF
            // HFP0 DBG begin
            LogPrintf("HFP0 DIF: MIDAS: testnet: new block timestamp > 2*nPowTargetSpacing: returning POW limit\n");
            // HFP0 DBG end
#endif
            return nProofOfWorkLimit;
        }
        else {
            // Return the last non-special-min-difficulty-rules-block after the fork triggered
            const CBlockIndex* pindex = pindexLast;
            while (pindex->pprev && pindex->nBits == nProofOfWorkLimit && pindex->nHeight > params.nPowMidasBlockStart)
                pindex = pindex->pprev;
            // HFP0 DBG begin
            arith_uint256 dbgBits;
            dbgBits.SetCompact(pindex->nBits);
#if HFP0_DEBUG_DIF
            // HFP0 DBG begin
            LogPrintf("HFP0 DIF: MIDAS: testnet: regular timestamp, return last non-special min-dif block\n");
            if (!pindex->pprev) {
                LogPrintf("HFP0 DIF: MIDAS: testnet: hit genesis (nIntervalDesired=%d)\n", nIntervalDesired);
            }
            else if (pindex->nHeight % nIntervalDesired == 0) {
                LogPrintf("HFP0 DIF: MIDAS: testnet: hit interval at height %d (nIntervalDesired=%d)\n", pindex->nHeight, nIntervalDesired);
            }
            else {
                LogPrintf("HFP0 DIF: MIDAS: testnet: hit min-dif-block at height %d (nIntervalDesired=%d)\n", pindex->nHeight, nIntervalDesired);
            }
            LogPrintf("HFP0 DIF: MIDAS: testnet: return pindex->nBits: %08x  %s\n", pindex->nBits, dbgBits.ToString());
            // HFP0 DBG end
#endif
            return pindex->nBits;
        }
    }

    // Regulate block times so as to remain synchronized in the long run with the actual time.  The first step is to
    // calculate what interval we want to use as our regulatory goal.  It depends on how far ahead of (or behind)
    // schedule we are.  If we're more than an adjustment period ahead or behind, we use the maximum (nSlowInterval) or minimum
    // (nFastInterval) values; otherwise we calculate a weighted average somewhere in between them.  The closer we are
    // to being exactly on schedule the closer our selected interval will be to our nominal interval (nPowTargetSpacing).

    now = pindexLast->GetBlockTime();
    // original MIDAS:
    //   BlockHeightTime = params.nPowTimeStart + pindexLast->nHeight * params.nPowTargetSpacing;
    // Since a large discrepancy (nearly 1/2 year) between actual and expected block height time
    // has been built up over the first seven years of Bitcoin, we do not use the Genesis block
    // as indicative, but take a more recent block time (let's say 180 days prior to the fork)
    // on which to base the expected block height time.
    BlockHeightTime = params.nPowMidasTimeStart + (pindexLast->nHeight - params.nPowMidasBlockStart) * params.nPowTargetSpacing;

    if (now < BlockHeightTime + params.nPowAdjustmentInterval && now > BlockHeightTime )
    // ahead of schedule by less than one interval.
    nIntervalDesired = ((params.nPowAdjustmentInterval - (now - BlockHeightTime)) * params.nPowTargetSpacing +
                (now - BlockHeightTime) * nFastInterval) / params.nPowAdjustmentInterval;
    else if (now + params.nPowAdjustmentInterval > BlockHeightTime && now < BlockHeightTime)
    // behind schedule by less than one interval.
    nIntervalDesired = ((params.nPowAdjustmentInterval - (BlockHeightTime - now)) * params.nPowTargetSpacing +
                (BlockHeightTime - now) * nSlowInterval) / params.nPowAdjustmentInterval;

    // ahead by more than one interval;
    else if (now < BlockHeightTime) nIntervalDesired = nSlowInterval;

    // behind by more than an interval.
    else  nIntervalDesired = nFastInterval;

    // find out what average intervals over last 5, 7, 9, and 17 blocks have been.
    avgRecentTimestamps(pindexLast, &avgOf5, &avgOf7, &avgOf9, &avgOf17, params);

    // check for emergency adjustments. These are to bring the diff up or down FAST when a burst miner or multipool
    // jumps on or off.  Once they kick in they can adjust difficulty very rapidly, and they can kick in very rapidly
    // after massive hash power jumps on or off.

    // Important note: This is a self-damping adjustment because 8/5 and 5/8 are closer to 1 than 3/2 and 2/3.  Do not
    // screw with the constants in a way that breaks this relationship.  Even though self-damping, it will usually
    // overshoot slightly. But normal adjustment will handle damping without getting back to emergency.
    toofast = (nIntervalDesired * 2) / 3;
    tooslow = (nIntervalDesired * 3) / 2;

    // both of these check the shortest interval to quickly stop when overshot.  Otherwise first is longer and second shorter.
    if (avgOf5 < toofast && avgOf9 < toofast && avgOf17 < toofast)
    {  //emergency adjustment, slow down (longer intervals because shorter blocks)
#if HFP0_DEBUG_DIF
        // HFP0 DBG begin
        LogPrintf("HFP0 DIF:GetNextWorkRequired EMERGENCY RETARGET\n");
        // HFP0 DBG end
#endif
        difficultyfactor *= 8;
        difficultyfactor /= 5;
    }
    else if (avgOf5 > tooslow && avgOf7 > tooslow && avgOf9 > tooslow)
    {  //emergency adjustment, speed up (shorter intervals because longer blocks)
#if HFP0_DEBUG_DIF
        // HFP0 DBG begin
        LogPrintf("HFP0 DIF:GetNextWorkRequired EMERGENCY RETARGET\n");
        // HFP0 DBG end
#endif
        difficultyfactor *= 5;
        difficultyfactor /= 8;
    }

    // If no emergency adjustment, check for normal adjustment.
    else if (((avgOf5 > nIntervalDesired || avgOf7 > nIntervalDesired) && avgOf9 > nIntervalDesired && avgOf17 > nIntervalDesired) ||
         ((avgOf5 < nIntervalDesired || avgOf7 < nIntervalDesired) && avgOf9 < nIntervalDesired && avgOf17 < nIntervalDesired))
    {   // At least 3 averages too high or at least 3 too low, including the two longest. This will be executed 3/16 of
        // the time on the basis of random variation, even if the settings are perfect. It regulates one-sixth of the way
        // to the calculated point.
#if HFP0_DEBUG_DIF
        // HFP0 DBG begin
        LogPrintf("HFP0 DIF:GetNextWorkRequired RETARGET\n");
        // HFP0 DBG end
#endif
        difficultyfactor *= (6 * nIntervalDesired);
        difficultyfactor /= (avgOf17 + (5 * nIntervalDesired));
    }

    // limit to doubling or halving.  There are no conditions where this will make a difference unless there is an
    // unsuspected bug in the above code.
    if (difficultyfactor > 20000) difficultyfactor = 20000;
    if (difficultyfactor < 5000) difficultyfactor = 5000;

    arith_uint256 bnNew;
    arith_uint256 bnOld;

    bnOld.SetCompact(pindexLast->nBits);

    if (difficultyfactor == 10000) // no adjustment.
      return(bnOld.GetCompact());

    bnNew = bnOld / difficultyfactor;
    bnNew *= 10000;

    if (bnNew > bnPowLimit)
      bnNew = bnPowLimit;

#if HFP0_DEBUG_DIF
    // HFP0 DBG begin
    LogPrintf("HFP0 DIF:actual time %d, Scheduled time for this block height = %d\n", now, BlockHeightTime );
    LogPrintf("HFP0 DIF:nominal block interval = %d, regulating on interval %d to get back to schedule.\n",
          params.nPowTargetSpacing, nIntervalDesired );
    LogPrintf("HFP0 DIF:intervals of last 5/7/9/17 blocks = %d / %d / %d / %d.\n",
          avgOf5, avgOf7, avgOf9, avgOf17);
    LogPrintf("HFP0 DIF:difficulty Before Adjustment: %08x  %s\n", pindexLast->nBits, bnOld.ToString());
    LogPrintf("HFP0 DIF:difficulty After Adjustment:  %08x  %s\n", bnNew.GetCompact(), bnNew.ToString());
    // HFP0 DBG end
#endif

    return bnNew.GetCompact();
}
// HFP0 DIF end

unsigned int GetNextWorkRequired(const CBlockIndex* pindexLast, const CBlockHeader *pblock, const Consensus::Params& params)
{
    unsigned int nProofOfWorkLimit = UintToArith256(params.powLimitHistoric).GetCompact();
    // HFP0 FRK begin
    // historic difficulty algorithm is the Bitcoin's 2016 blocks, used prior to HFP0 hard fork
    unsigned int retarget = DIFF_BTC_MIDAS;  //DIFF_BTC_2016;
    // HFP0 FRK end

    // HFP0 FRK, DIF begin
    // if we are at or past the fork height, use new difficulty algorithm and powLimit
#if HFP0_DEBUG_DIF
    // HFP0 DBG begin
    if (pindexLast->nHeight + 1 == params.nPowMidasBlockStart) {
        LogPrintf("HFP0 DIF: GetNextWorkRequired detected fork height %d\n", params.nPowMidasBlockStart);
    }
    // HFP0 DBG end
#endif
    if (pindexLast->nHeight + 1 >= params.nPowMidasBlockStart) {
        retarget = DIFF_BTC_MIDAS;
        nProofOfWorkLimit = UintToArith256(params.powLimitResetAtFork).GetCompact();
    }
    // HFP0 FRK, DIF end

    // HFP0 FRK, DIF begin
    // reset the difficulty at the fork
    if ((pindexLast->nHeight + 1) == params.nPowMidasBlockStart) {
#if HFP0_DEBUG_DIF
        // HFP0 DBG begin
        LogPrintf("HFP0 DIF: CalculateNextWorkRequired reset to min difficulty - next block is fork\n");
        // HFP0 DBG end
#endif
        return nProofOfWorkLimit;
    }
    // HFP0 FRK, DIF end

    // HFP0 FRK, DIF begin
    if (retarget == DIFF_BTC_MIDAS) {
#if HFP0_DEBUG_DIF
        // HFP0 DBG begin
        LogPrintf("HFP0 DIF: using regular diff algo for block %d\n", pindexLast->nHeight + 1);
        // HFP0 DBG end
#endif
        // Genesis block
        if (pindexLast == NULL)
            return nProofOfWorkLimit;

        // Only change once per difficulty adjustment interval
        if ((pindexLast->nHeight+1) % params.DifficultyAdjustmentInterval() != 0)
        {
            if (params.fPowAllowMinDifficultyBlocks)
            {
                // Special difficulty rule for testnet:
                // If the new block's timestamp is more than 2* 10 minutes
                // then allow mining of a min-difficulty block.
                if (pblock->GetBlockTime() > pindexLast->GetBlockTime() + params.nPowTargetSpacing*2)
                    return nProofOfWorkLimit;
                else
                {
                    // Return the last non-special-min-difficulty-rules-block
                    const CBlockIndex* pindex = pindexLast;
                    while (pindex->pprev && pindex->nHeight % params.DifficultyAdjustmentInterval() != 0 && pindex->nBits == nProofOfWorkLimit)
                        pindex = pindex->pprev;
                    return pindex->nBits;
                }
            }
            return pindexLast->nBits;
        }

        // Go back by what we want to be 14 days worth of blocks
        int nHeightFirst = pindexLast->nHeight - (params.DifficultyAdjustmentInterval()-1);
        assert(nHeightFirst >= 0);
        const CBlockIndex* pindexFirst = pindexLast->GetAncestor(nHeightFirst);
        assert(pindexFirst);

        return CalculateNextWorkRequired(pindexLast, pindexFirst->GetBlockTime(), params);
    }

#if HFP0_DEBUG_DIF
    // HFP0 DBG begin
    LogPrintf("HFP0 DIF: using MIDAS for block %d\n", pindexLast->nHeight + 1);
    // HFP0 DBG end
#endif
    // post-fork: use MIDAS
    return GetNextWorkRequiredMIDAS(pindexLast, pblock, params);
    // HFP0 FRK, DIF end
}

unsigned int CalculateNextWorkRequired(const CBlockIndex* pindexLast, int64_t nFirstBlockTime, const Consensus::Params& params)
{
    if (params.fPowNoRetargeting)
        return pindexLast->nBits;

    // Limit adjustment step
    int64_t nActualTimespan = pindexLast->GetBlockTime() - nFirstBlockTime;
    LogPrintf("  nActualTimespan = %d  before bounds\n", nActualTimespan);
    if (nActualTimespan < params.nPowTargetTimespan/4)
        nActualTimespan = params.nPowTargetTimespan/4;
    if (nActualTimespan > params.nPowTargetTimespan*4)
        nActualTimespan = params.nPowTargetTimespan*4;

    // Retarget
    const arith_uint256 bnPowLimit = UintToArith256(params.powLimitHistoric);
    arith_uint256 bnNew;
    arith_uint256 bnOld;
    bnNew.SetCompact(pindexLast->nBits);
    bnOld = bnNew;
    bnNew *= nActualTimespan;
    bnNew /= params.nPowTargetTimespan;

    if (bnNew > bnPowLimit)
        bnNew = bnPowLimit;

#if HFP0_DEBUG_DIF
    // HFP0 DBG begin: correct function name
    LogPrintf("CalculateNextWorkRequired RETARGET\n");
    // HFP0 DBG end
#endif
    LogPrintf("params.nPowTargetTimespan = %d    nActualTimespan = %d\n", params.nPowTargetTimespan, nActualTimespan);
    LogPrintf("Before: %08x  %s\n", pindexLast->nBits, bnOld.ToString());
    LogPrintf("After:  %08x  %s\n", bnNew.GetCompact(), bnNew.ToString());

    return bnNew.GetCompact();
}

// HFP0 FRK, DIF begin
// pass in active POW limit from caller, remove Params as it was only used for powLimit
bool CheckProofOfWork(uint256 hash, unsigned int nBits, uint256 activePowLimit)
// HFP0 FRK, DIF end
{
    bool fNegative;
    bool fOverflow;
    arith_uint256 bnTarget;

    bnTarget.SetCompact(nBits, &fNegative, &fOverflow);

    // Check range
    if (fNegative || bnTarget == 0 || fOverflow || bnTarget > UintToArith256(activePowLimit)) {
#if HFP0_DEBUG_DIF
        // HFP0 DBG begin
        LogPrintf("HFP0 DIF:CheckProofOfWork() error:\n");
        LogPrintf("HFP0 DIF:nBits:    %08x  %s\n", bnTarget.GetCompact(), bnTarget.ToString());
        LogPrintf("HFP0 DIF:powLimit: %08x  %s\n", UintToArith256(activePowLimit).GetCompact(), UintToArith256(activePowLimit).ToString());
        LogPrintf("HFP0 DIF:hash:     %08x  %s\n", UintToArith256(hash).GetCompact(), UintToArith256(hash).ToString());
        // HFP0 DBG end
#endif
        return error("CheckProofOfWork(): nBits below minimum work");
    }

    // Check proof of work matches claimed amount
    if (UintToArith256(hash) > bnTarget) {
        return error("CheckProofOfWork(): hash doesn't match nBits");
    }

    return true;
}

arith_uint256 GetBlockProof(const CBlockIndex& block)
{
    arith_uint256 bnTarget;
    bool fNegative;
    bool fOverflow;
    bnTarget.SetCompact(block.nBits, &fNegative, &fOverflow);
    if (fNegative || fOverflow || bnTarget == 0)
        return 0;
    // We need to compute 2**256 / (bnTarget+1), but we can't represent 2**256
    // as it's too large for a arith_uint256. However, as 2**256 is at least as large
    // as bnTarget+1, it is equal to ((2**256 - bnTarget - 1) / (bnTarget+1)) + 1,
    // or ~bnTarget / (nTarget+1) + 1.
    return (~bnTarget / (bnTarget + 1)) + 1;
}

int64_t GetBlockProofEquivalentTime(const CBlockIndex& to, const CBlockIndex& from, const CBlockIndex& tip, const Consensus::Params& params)
{
    arith_uint256 r;
    int sign = 1;
    if (to.nChainWork > from.nChainWork) {
        r = to.nChainWork - from.nChainWork;
    } else {
        r = from.nChainWork - to.nChainWork;
        sign = -1;
    }
    r = r * arith_uint256(params.nPowTargetSpacing) / GetBlockProof(tip);
    if (r.bits() > 63) {
        return sign * std::numeric_limits<int64_t>::max();
    }
    return sign * r.GetLow64();
}
