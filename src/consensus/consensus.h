// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef ROICOIN_CONSENSUS_CONSENSUS_H
#define ROICOIN_CONSENSUS_CONSENSUS_H

#include "uint256.h"

#define HFP0_DEBUG_DIF 0     // difficulty
#define HFP0_DEBUG_POW 0     // proof-of-work


/** The maximum allowed size for a serialized block, in bytes (network rule) */
static const unsigned int MAX_BLOCK_SIZE = 1000000; //1 MB
/** The maximum allowed number of signature check operations in a block (network rule) */
static const unsigned int MAX_BLOCK_SIGOPS = MAX_BLOCK_SIZE/50;
/** Coinbase transaction outputs can only be spent after this number of new blocks (network rule) */
static const int COINBASE_MATURITY = 3;

static const uint256 POW_LIMIT_FORK_MAINNET = uint256S("00007fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");



/** Flags for LockTime() */
enum {

    /* Interpret sequence numbers as relative lock-time constraints. */
	LOCKTIME_VERIFY_SEQUENCE = (1 << 0),
    /* Use GetMedianTimePast() instead of nTime for end point timestamp. */
    	LOCKTIME_MEDIAN_TIME_PAST = (1 << 1),
};

/** Used as the flags parameter to CheckFinalTx() in non-consensus code */
static const unsigned int STANDARD_LOCKTIME_VERIFY_FLAGS = LOCKTIME_MEDIAN_TIME_PAST;

#endif // ROICOIN_CONSENSUS_CONSENSUS_H
