// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"
#include "consensus/consensus.h"
#include "merkleblock.h"
#include "tinyformat.h"
#include "util.h"
#include "utilstrencodings.h"
#include "arith_uint256.h"
#include "uint256.h"
#include <assert.h>
#include "aligned_malloc.h"

#include <boost/assign/list_of.hpp>

using namespace std;

#include "chainparamsseeds.h"


class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "main";
        consensus.nSubsidyHalvingInterval 	= 7884000;
        consensus.nMajorityEnforceBlockUpgrade 	= 750;
        consensus.nMajorityRejectBlockOutdated 	= 950;
        consensus.nMajorityWindow 		= 1000;
	consensus.powLimitHistoric 		= uint256S("0fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
	consensus.powLimitResetAtFork    	= POW_LIMIT_FORK_MAINNET;    
	consensus.nPowTargetTimespan 		= 1200; // 20 minutes
        consensus.nPowTargetSpacing 		= 120; 	// 2 minutes
        consensus.fPowAllowMinDifficultyBlocks 	= false;
        consensus.fPowNoRetargeting 		= false;

        
	pchMessageStart[0]	= 0x10;
        pchMessageStart[1] 	= 0x27;
        pchMessageStart[2] 	= 0x11;
        pchMessageStart[3] 	= 0x29;
        vAlertPubKey 		= ParseHex("04e176d746abe3c2d176bf52efd5a174c266d44a3d8f64fb20894a4ea3259343b0fa506d8007323bd19d184d5040c5c7c45de768a54582bf36c028e9b9c22f9bd7");
        nDefaultPort 		= 3377;
        nMinerThreads 		= 0;
        nMaxTipAge 		= 24 * 60 * 60;
        nPruneAfterHeight 	= 262800;
/*
        const char* pszTimestamp = "But about that day or hour no one knows, not even the angels in heaven... Matthew 24:36";
        CMutableTransaction txNew;
        txNew.vin.resize(1);
        txNew.vout.resize(1);
        txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        txNew.vout[0].nValue = 120 * COIN;
        txNew.vout[0].scriptPubKey = CScript() << ParseHex("04ca7a877b6e62608927572de33f2fb1c09d5ad6d606edfcf412ecf2b753847f49fc3001bca8d880973e56ac1eee548b0a2d8c6bcc01f36d325f7774b8e57afd62") << OP_CHECKSIG;
        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock.SetNull();
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nVersion 		= 4;
        genesis.nTime    		= 1509853643;  //date +%s
        genesis.nBits    		= 0x21000FFF;  //0x21000FFF
	genesis.nNonce 			= 4294901760;  //0xffff0000
        genesis.nStartLocation 		= 36468;
        genesis.nFinalCalculation 	= 2023345552;


	consensus.nPowAdjustmentInterval = 20 * 60;   	//Twenty minutes
	consensus.nPowMidasBlockStart 	 = 1;		//Block Start MIDAS
	consensus.nPowMidasTimeStart  	 = 1509857120;	//date +%s


	consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == genesis.GetHash());

       // assert(consensus.hashGenesisBlock == uint256S("0x003852bfe1d708c4dfac4d48f5d678031c12f07fe478916c40351a84ab85747d"));
       // assert(genesis.hashMerkleRoot == uint256S("0xfd143730e1e498a3d76d09557044cfbacef449cb5513c94bef489a82b17e14be"));

        vSeeds.push_back(CDNSSeedData("seed1.roi-coin.com", "seed1.roi-coin.com"));  //New York
        vSeeds.push_back(CDNSSeedData("seed2.roi-coin.com", "seed2.roi-coin.com"));  //San Francisco
	vSeeds.push_back(CDNSSeedData("seed3.roi-coin.com", "seed3.roi-coin.com"));  //Toronto
	vSeeds.push_back(CDNSSeedData("seed4.roi-coin.com", "seed4.roi-coin.com"));  //Kansas City
	vSeeds.push_back(CDNSSeedData("seed5.roi-coin.com", "seed5.roi-coin.com"));  //Europe
        vSeeds.push_back(CDNSSeedData("seed6.roi-coin.com", "seed6.roi-coin.com"));  
        vSeeds.push_back(CDNSSeedData("seed7.roi-coin.com", "seed7.roi-coin.com"));  
	vSeeds.push_back(CDNSSeedData("seed8.roi-coin.com", "seed8.roi-coin.com"));  
	vSeeds.push_back(CDNSSeedData("seed9.roi-coin.com", "seed9.roi-coin.com"));  
	vSeeds.push_back(CDNSSeedData("seed10.roi-coin.com", "seed10.roi-coin.com"));  
        vSeeds.push_back(CDNSSeedData("seed11.roi-coin.com", "seed11.roi-coin.com")); 
        vSeeds.push_back(CDNSSeedData("seed12.roi-coin.com", "seed12.roi-coin.com"));  
	vSeeds.push_back(CDNSSeedData("seed13.roi-coin.com", "seed13.roi-coin.com"));  
	vSeeds.push_back(CDNSSeedData("seed14.roi-coin.com", "seed14.roi-coin.com"));  
	vSeeds.push_back(CDNSSeedData("seed15.roi-coin.com", "seed15.roi-coin.com"));  
*/
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,60);
	base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,122);
        base58Prefixes[SECRET_KEY]     = std::vector<unsigned char>(1,128);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x10)(0x27)(0x11)(0x10).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x10)(0x27)(0x29)(0x17).convert_to_container<std::vector<unsigned char> >();

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fRequireRPCPassword 		= true;
        fMiningRequiresPeers 		= true;
        fDefaultConsistencyChecks 	= false;
        fRequireStandard 		= true;
        fMineBlocksOnDemand 		= false;
        fTestnetToBeDeprecatedFieldRPC 	= false;

        checkpointData = (Checkpoints::CCheckpointData) {
            boost::assign::map_list_of
            ( 0, uint256S("0x003852bfe1d708c4dfac4d48f5d678031c12f07fe478916c40351a84ab85747d")),
            1509853643, // * UNIX timestamp of last checkpoint block
            1,   	// * total number of transactions between genesis and last checkpoint
                      //   (the tx=... number in the SetBestChain debug.log lines)
            2000.0     	// * estimated number of transactions per day after checkpoint
        };
    }

};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CMainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";
        consensus.nSubsidyHalvingInterval 	= 7884000;
        consensus.nMajorityEnforceBlockUpgrade 	= 750;
        consensus.nMajorityRejectBlockOutdated 	= 950;
        consensus.nMajorityWindow 		= 1000;
	consensus.powLimitHistoric 		= uint256S("0fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
	consensus.powLimitResetAtFork    	= POW_LIMIT_FORK_MAINNET;
	consensus.nPowTargetTimespan 		= 1200; // 20 minutes
        consensus.nPowTargetSpacing 		= 120; 	// 2 minutes
        consensus.fPowAllowMinDifficultyBlocks 	= true;
        consensus.fPowNoRetargeting 		= false;
        
	pchMessageStart[0]	= 0xab;
        pchMessageStart[1] 	= 0xcd;
        pchMessageStart[2] 	= 0xba;
        pchMessageStart[3] 	= 0xd2;
        vAlertPubKey 		= ParseHex("04c7d5fe6e3bff78ef8993909c85fba2ede6b77f4fd19abb167f18398ce8cdd8d894bb23e8d7b957a3806aa05e7328950a3d9e0dce18ef8749a858a1b75079fec4");
        nDefaultPort 		= 33770;
        nMinerThreads 		= 0;
        nMaxTipAge 		= 24 * 60 * 60;
        nPruneAfterHeight 	= 1000;

        const char* pszTimestamp = "This is a test of the TestNet system - Because everyone loves a fork.";

        CMutableTransaction txNew;
        txNew.vin.resize(1);
        txNew.vout.resize(1);
        txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        txNew.vout[0].nValue = 120 * COIN;
        txNew.vout[0].scriptPubKey = CScript() << ParseHex("0481ec3bf6739fb62598c2a9f31ac9bb702477567d6deaa9a72b822b44778f44cb794d41648700126381ef3c33398e2aa9d7f8c0c2dcd5cdedbf434ca23c12617c") << OP_CHECKSIG;
        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock.SetNull();
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nVersion 		 = 4;
     	genesis.nTime    		 = 1511385339;  //date +%s
        genesis.nBits    		 = 0x21000FFF;  //0x21000FFF
	genesis.nNonce 			 = 4294901760;  //0xffff0000
        genesis.nStartLocation 		 = 259177;
        genesis.nFinalCalculation 	 = 3482034946;
	consensus.nPowAdjustmentInterval = 20 * 60;   	//Twenty minutes
	consensus.nPowMidasBlockStart 	 = 1;		//Block Start MIDAS
	consensus.nPowMidasTimeStart  	 = 1511385339;	//date +%s

/*	
if(genesis.GetHash() != uint256S("008872e5582924544e5c707ee4b839bb82c28a9e94e917c94b40538d5658c04b") ){
            arith_uint256 hashTarget = arith_uint256().SetCompact(genesis.nBits);
            uint256 thash;
            char *scratchpad;
            scratchpad = (char*)aligned_malloc(1<<30);
            while(true){
                int collisions=0;
		int tmpflag=0;
		thash = genesis.FindBestPatternHash(collisions,scratchpad,8,&tmpflag);
		LogPrintf("nonce %08X: hash = %s (target = %s)\n", genesis.nNonce, thash.ToString().c_str(),
                hashTarget.ToString().c_str());
                if (UintToArith256(thash) <= hashTarget)
                    break;
                genesis.nNonce=genesis.nNonce+10000;
                if (genesis.nNonce == 0){
                    LogPrintf("NONCE WRAPPED, incrementing time\n");
                    ++genesis.nTime;
                }
            }
            aligned_free(scratchpad);
            LogPrintf("block.nTime = %u \n", genesis.nTime);
            LogPrintf("block.nNonce = %u \n", genesis.nNonce);
            LogPrintf("block.GetHash = %s\n", genesis.GetHash().ToString().c_str());
            LogPrintf("block.nBits = %u \n", genesis.nBits);
            LogPrintf("block.nStartLocation = %u \n", genesis.nStartLocation);
            LogPrintf("block.nFinalCalculation = %u \n", genesis.nFinalCalculation);

        }
*/	
	consensus.hashGenesisBlock = genesis.GetHash();
	assert(consensus.hashGenesisBlock == genesis.GetHash());

	assert(consensus.hashGenesisBlock == uint256S("0x005566d40af96f9f09b5bd8b499f769e571ab11dea9390b4e81135f8770c63a4"));
        assert(genesis.hashMerkleRoot == uint256S("0x92feaffad16699d7f7acb5bb53e6fab02d3eb5751cc32a4f787b1cced8e24016"));


	vSeeds.push_back(CDNSSeedData("TestNet.roi-coin.com", "TestNet.roi-coin.com"));  //TestNet - Toronto VPS


        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,60);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,123);
        base58Prefixes[SECRET_KEY]     = std::vector<unsigned char>(1,127);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x11)(0x27)(0xab)(0xcd).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x11)(0x27)(0x12)(0x34).convert_to_container<std::vector<unsigned char> >();
        
	//vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fRequireRPCPassword 		= false;
        fMiningRequiresPeers 		= false;
        fDefaultConsistencyChecks 	= false;
        fRequireStandard 		= false;
        fMineBlocksOnDemand 		= false;
        fTestnetToBeDeprecatedFieldRPC 	= true;



        checkpointData = (Checkpoints::CCheckpointData)
	{
            
	boost::assign::map_list_of
            ( 0, uint256S("001")),

            1511385339, // * UNIX timestamp of last checkpoint block
            1,   	// * total number of transactions between genesis and last checkpoint
                        //   (the tx=... number in the SetBestChain debug.log lines)
            500.0     	// * estimated number of transactions per day after checkpoint
        };
    }
};

static CTestNetParams testNetParams;



/**
 * Regression test
 */
class CRegTestParams : public CTestNetParams {
public:
    CRegTestParams() {
        strNetworkID = "regtest";
        consensus.nSubsidyHalvingInterval = 150;
        consensus.nMajorityEnforceBlockUpgrade = 750;
        consensus.nMajorityRejectBlockOutdated = 950;
        consensus.nMajorityWindow = 1000;
        consensus.powLimitHistoric = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        pchMessageStart[0] = 0xfa;
        pchMessageStart[1] = 0xbf;
        pchMessageStart[2] = 0xb5;
        pchMessageStart[3] = 0xda;
        nMinerThreads = 1;
        nMaxTipAge = 24 * 60 * 60;
        genesis.nTime = 1296688602;
        genesis.nBits = 0x207fffff;
        genesis.nNonce = 2;
        consensus.hashGenesisBlock = genesis.GetHash();
        nDefaultPort = 18444;
 
/*
if(genesis.GetHash() != uint256S("008872e5582924544e5c707ee4b839bb82c28a9e94e917c94b40538d5658c04b") ){
            arith_uint256 hashTarget = arith_uint256().SetCompact(genesis.nBits);
            uint256 thash;
            char *scratchpad;
            scratchpad = (char*)aligned_malloc(1<<30);
            while(true){
                int collisions=0;
		int tmpflag=0;
		thash = genesis.FindBestPatternHash(collisions,scratchpad,8,&tmpflag);
		LogPrintf("nonce %08X: hash = %s (target = %s)\n", genesis.nNonce, thash.ToString().c_str(),
                hashTarget.ToString().c_str());
                if (UintToArith256(thash) <= hashTarget)
                    break;
                genesis.nNonce=genesis.nNonce+10000;
                if (genesis.nNonce == 0){
                    LogPrintf("NONCE WRAPPED, incrementing time\n");
                    ++genesis.nTime;
                }
            }
            aligned_free(scratchpad);
            LogPrintf("block.nTime = %u \n", genesis.nTime);
            LogPrintf("block.nNonce = %u \n", genesis.nNonce);
            LogPrintf("block.GetHash = %s\n", genesis.GetHash().ToString().c_str());
            LogPrintf("block.nBits = %u \n", genesis.nBits);
            LogPrintf("block.nStartLocation = %u \n", genesis.nStartLocation);
            LogPrintf("block.nFinalCalculation = %u \n", genesis.nFinalCalculation);
            consensus.hashGenesisBlock=genesis.GetHash();
        }

*/


       //assert(consensus.hashGenesisBlock == uint256S("0x0f9188f13cb7b2c71f2a335e3a4fc328bf5beb436012afca590b1a11466e2206"));
        nPruneAfterHeight = 1000;

        vFixedSeeds.clear(); //! Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();  //! Regtest mode doesn't have any DNS seeds.

        fRequireRPCPassword = false;
        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = false;

        checkpointData = (Checkpoints::CCheckpointData){
            boost::assign::map_list_of
            ( 0, uint256S("0f9188f13cb7b2c71f2a335e3a4fc328bf5beb436012afca590b1a11466e2206")),
            0,
            0,
            0
        };
    }
};
static CRegTestParams regTestParams;

static CChainParams *pCurrentParams = 0;

const CChainParams &Params() {
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams &Params(CBaseChainParams::Network network) {
    switch (network) {
        case CBaseChainParams::MAIN:
            return mainParams;
        case CBaseChainParams::TESTNET:
            return testNetParams;
        case CBaseChainParams::REGTEST:
            return regTestParams;
        default:
            assert(false && "Unimplemented network");
            return mainParams;
    }
}

void SelectParams(CBaseChainParams::Network network) {
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}

bool SelectParamsFromCommandLine()
{
    CBaseChainParams::Network network = NetworkIdFromCommandLine();
    if (network == CBaseChainParams::MAX_NETWORK_TYPES)
        return false;

    SelectParams(network);
    return true;
}
