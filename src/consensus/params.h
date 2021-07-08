// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef ROICOIN_CONSENSUS_PARAMS_H
#define ROICOIN_CONSENSUS_PARAMS_H

#include "uint256.h"


namespace Consensus {
/**
 * Parameters that influence chain consensus.
 */
struct Params {
    uint256 hashGenesisBlock;
    int nSubsidyHalvingInterval;
    /** Used to check majorities for block version upgrade */
    int nMajorityEnforceBlockUpgrade;
    int nMajorityRejectBlockOutdated;
    int nMajorityWindow;
    /** Block height and hash at which BIP34 becomes active */
    int BIP34Height;
    uint256 BIP34Hash;
    /** Proof of work parameters */
    // HFP0 DIF begin: new names for pre-/post-fork POW limits
    uint256 powLimitHistoric;
    uint256 powLimitResetAtFork;
    // HFP0 DIF end
    bool fPowAllowMinDifficultyBlocks;
    bool fPowNoRetargeting;
    int64_t nPowTargetSpacing;
    int64_t nPowTargetTimespan;
    // HFP0 DIF begin: add new MIDAS-specific parameters
    int64_t nPowAdjustmentInterval;
    uint32_t nPowMidasTimeStart;
    int nPowMidasBlockStart;
    // HFP0 DIF end
    int64_t DifficultyAdjustmentInterval() const { return nPowTargetTimespan / nPowTargetSpacing; }

    // HFP0 CLN removed Classic 2MB fork parameters and access functions

    // HFP0 FRK begin
    int nHFP0ActivateSizeForkHeight;     // block height trigger

    int HFP0ActivateSizeForkHeight() const { return nHFP0ActivateSizeForkHeight; };
    // HFP0 FRK end
};
} // namespace Consensus

#endif // ROICOIN_CONSENSUS_PARAMS_H
