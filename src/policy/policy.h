// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2016 The Bitcoin Core developers
// Copyright (c) 2019 Bitcoin Association
// Distributed under the Open BSV software license, see the accompanying file LICENSE.

#ifndef BITCOIN_POLICY_POLICY_H
#define BITCOIN_POLICY_POLICY_H

#include "consensus/consensus.h"
#include "script/interpreter.h"
#include "script/standard.h"

#include <string>

class Config;
class CCoinsViewCache;

/** Defaults for -excessiveblocksize and -blockmaxsize. The changed when we reach blocksize activation time.
 *
 * DEFAULT_MAX_GENERATED_BLOCK_SIZE_* represents default for -blockmaxsize, 
 * which controls the maximum size of block the mining code will create 
 * 
 * DEFAULT_MAX_BLOCK_SIZE_* represents default for -excessiveblocksize, which specifies the 
 * maximum allowed size for a block, in bytes. This is actually a consenus rule - if a node sets
 * this to a value lower than  -blockmaxsize of another node, it will start rejecting 
 * big another node. 
 * 
 * Values bellow are also parsed by cdefs.py.
 */


/** Default max block size parameters before and after Genesis 
 */
static const uint64_t MAIN_DEFAULT_MAX_BLOCK_SIZE_BEFORE_GENESIS = 2 * ONE_GIGABYTE;
static const uint64_t MAIN_DEFAULT_MAX_BLOCK_SIZE_AFTER_GENESIS = 4 * ONE_GIGABYTE - ONE_MEGABYTE; 

static const uint64_t REGTEST_DEFAULT_MAX_BLOCK_SIZE_BEFORE_GENESIS = 2 * ONE_GIGABYTE;
static const uint64_t REGTEST_DEFAULT_MAX_BLOCK_SIZE_AFTER_GENESIS = 4 * ONE_GIGABYTE - ONE_MEGABYTE; 

static const uint64_t TESTNET_DEFAULT_MAX_BLOCK_SIZE_BEFORE_GENESIS = 2 * ONE_GIGABYTE;
static const uint64_t TESTNET_DEFAULT_MAX_BLOCK_SIZE_AFTER_GENESIS = 4 * ONE_GIGABYTE - ONE_MEGABYTE; 

static const uint64_t STN_DEFAULT_MAX_BLOCK_SIZE_BEFORE_GENESIS = 2 * ONE_GIGABYTE;
static const uint64_t STN_DEFAULT_MAX_BLOCK_SIZE_AFTER_GENESIS = 4 * ONE_GIGABYTE - ONE_MEGABYTE;


/** Default before and after max generated block size parameters and their activation times.
 */
static const uint64_t MAIN_NEW_BLOCKSIZE_ACTIVATION_TIME = 1563976800; // 2019-07-24T14:00:00
static const uint64_t MAIN_DEFAULT_MAX_GENERATED_BLOCK_SIZE_BEFORE = 32 * ONE_MEGABYTE;
static const uint64_t MAIN_DEFAULT_MAX_GENERATED_BLOCK_SIZE_AFTER = 128 * ONE_MEGABYTE;

static const uint64_t REGTEST_NEW_BLOCKSIZE_ACTIVATION_TIME = 1563976800; // 2019-07-24T14:00:00 
static const uint64_t REGTEST_DEFAULT_MAX_GENERATED_BLOCK_SIZE_BEFORE = 32 * ONE_MEGABYTE;
static const uint64_t REGTEST_DEFAULT_MAX_GENERATED_BLOCK_SIZE_AFTER = 128 * ONE_MEGABYTE;

static const uint64_t TESTNET_NEW_BLOCKSIZE_ACTIVATION_TIME = 1563976800; // 2019-07-24T14:00:00 
static const uint64_t TESTNET_DEFAULT_MAX_GENERATED_BLOCK_SIZE_BEFORE = 32 * ONE_MEGABYTE;
static const uint64_t TESTNET_DEFAULT_MAX_GENERATED_BLOCK_SIZE_AFTER = 128 * ONE_MEGABYTE;

static const uint64_t STN_NEW_BLOCKSIZE_ACTIVATION_TIME = 1558360800;   // 2019-05-20T14:00:00
static const uint64_t STN_DEFAULT_MAX_GENERATED_BLOCK_SIZE_BEFORE = 32 * ONE_MEGABYTE;
static const uint64_t STN_DEFAULT_MAX_GENERATED_BLOCK_SIZE_AFTER = 128 * ONE_MEGABYTE;


/** Default for -blockprioritypercentage, define the amount of block space
 * reserved to high priority transactions **/
static const uint64_t DEFAULT_BLOCK_PRIORITY_PERCENTAGE = 5;
/** Default for -blockmintxfee, which sets the minimum feerate for a transaction
 * in blocks created by mining code **/
static const Amount DEFAULT_BLOCK_MIN_TX_FEE(1000);
/** The maximum size for transactions we're willing to relay/mine */
static const unsigned int MAX_STANDARD_TX_SIZE = 100000;
/** Maximum number of signature check operations in an IsStandard() P2SH script
 */
static const unsigned int MAX_P2SH_SIGOPS = 15;
/** The maximum number of sigops we're willing to relay/mine in a single tx */
static const unsigned int MAX_STANDARD_TX_SIGOPS = MAX_TX_SIGOPS_COUNT / 5;
/** Default for -maxmempool, maximum megabytes of mempool memory usage */
static const unsigned int DEFAULT_MAX_MEMPOOL_SIZE = 300;
/** Default for -incrementalrelayfee, which sets the minimum feerate increase
 * for mempool limiting or BIP 125 replacement **/
static const CFeeRate MEMPOOL_FULL_FEE_INCREMENT(Amount(1000));
/** Default for -bytespersigop */
static const unsigned int DEFAULT_BYTES_PER_SIGOP = 20;
/**
 * Min feerate for defining dust. Historically this has been the same as the
 * minRelayTxFee, however changing the dust limit changes which transactions are
 * standard and should be done with care and ideally rarely. It makes sense to
 * only increase the dust limit after prior releases were already not creating
 * outputs below the new threshold.
 */
static const Amount DUST_RELAY_TX_FEE(1000);

/**
 * Standard script verification flags that standard transactions will comply
 * with. However scripts violating these flags may still be present in valid
 * blocks and we must accept those blocks.
 */
static const unsigned int STANDARD_SCRIPT_VERIFY_FLAGS =
    MANDATORY_SCRIPT_VERIFY_FLAGS | SCRIPT_VERIFY_DERSIG |
    SCRIPT_VERIFY_MINIMALDATA | SCRIPT_VERIFY_NULLDUMMY |
    SCRIPT_VERIFY_DISCOURAGE_UPGRADABLE_NOPS | SCRIPT_VERIFY_CLEANSTACK |
    SCRIPT_VERIFY_NULLFAIL | SCRIPT_VERIFY_CHECKLOCKTIMEVERIFY |
    SCRIPT_VERIFY_CHECKSEQUENCEVERIFY | SCRIPT_VERIFY_LOW_S;

/** For convenience, standard but not mandatory verify flags. */
static const unsigned int STANDARD_NOT_MANDATORY_VERIFY_FLAGS =
    STANDARD_SCRIPT_VERIFY_FLAGS & ~MANDATORY_SCRIPT_VERIFY_FLAGS;

/** returns flags for "standard" script*/
inline unsigned int StandardScriptVerifyFlags(bool genesisEnabled,
                                       bool utxoAfterGenesis) {
    unsigned int scriptFlags = STANDARD_SCRIPT_VERIFY_FLAGS;
    if (utxoAfterGenesis) {
        scriptFlags |= SCRIPT_UTXO_AFTER_GENESIS;
    }
    if (genesisEnabled) {
        scriptFlags |= SCRIPT_GENESIS;
    }
    return scriptFlags;
}


/** Used as the flags parameter to sequence and nLocktime checks in
 * non-consensus code. */
static const unsigned int STANDARD_LOCKTIME_VERIFY_FLAGS =
    LOCKTIME_VERIFY_SEQUENCE | LOCKTIME_MEDIAN_TIME_PAST;

bool IsStandard(const Config &config, const CScript &scriptPubKey, int nScriptPubKeyHeight, txnouttype &whichType);

/**
 * Check for standard transaction types
 * @param[in] nHeight represents the height that transactions was mined or the height that
 * we expect transcation will be mined in (in case transcation is being added to mempool)
 * @return True if all outputs (scriptPubKeys) use only standard transaction
 * forms
 */
bool IsStandardTx(const Config &config, const CTransaction &tx, int nHeight, std::string &reason);

/**
 * Check for standard transaction types
 * @param[in] mapInputs    Map of previous transactions that have outputs we're
 * spending
 * @return True if all inputs (scriptSigs) use only standard transaction forms
 */
bool AreInputsStandard(const Config& config,
                       const CTransaction &tx,
                       const CCoinsViewCache &mapInputs,
                       const int mempoolHeight);

extern CFeeRate incrementalRelayFee;
extern CFeeRate dustRelayFee;
extern unsigned int nBytesPerSigOp;

#endif // BITCOIN_POLICY_POLICY_H
