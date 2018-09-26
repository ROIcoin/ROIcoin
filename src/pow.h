// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef ROICOIN_POW_H
#define ROICOIN_POW_H

#include "consensus/params.h"

#include <stdint.h>

class CBlockHeader;
class CBlockIndex;
class uint256;
class arith_uint256;

unsigned int GetNextWorkRequired(const CBlockIndex* pindexLast, const CBlockHeader *pblock, const Consensus::Params&);
unsigned int CalculateNextWorkRequired(const CBlockIndex* pindexLast, int64_t nFirstBlockTime, const Consensus::Params&);
// HFP0 DIF begin
// define constants for difficulty retarget algorithms
enum DiffMode {
    DIFF_BTC_2016  = 0, // Retarget every 2016 blocks (historic Bitcoin style)
    DIFF_BTC_MIDAS = 1, // after initial HFP0 fork : retarget using MIDAS
};
// HFP0 DIF end

/** Check whether a block hash satisfies the proof-of-work requirement specified by nBits */
// HFP0 FRK, DIF begin
bool CheckProofOfWork(uint256 hash, unsigned int nBits, uint256 activePowLimit);
// HFP0 FRK, DIF end

arith_uint256 GetBlockProof(const CBlockIndex& block);

/** Return the time it would take to redo the work difference between from and to, assuming the current hashrate corresponds to the difficulty at tip, in seconds. */
int64_t GetBlockProofEquivalentTime(const CBlockIndex& to, const CBlockIndex& from, const CBlockIndex& tip, const Consensus::Params&);


#endif // ROICOIN_POW_H
