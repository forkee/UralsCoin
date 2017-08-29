// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
// Copyright (c) 2014-2015 The Urals developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"

#include "assert.h"
#include "core.h"
#include "protocol.h"
#include "util.h"

#include <boost/assign/list_of.hpp>

using namespace boost::assign;

//
// Main network
//

unsigned int pnSeed[] =
{
0x05C4BE52, 0x68EEBC2C, 0x55D6444B, 0x5B996D95, 0x9B5E9244, 0x4FACD744, 0x2D20B813, 0x5D9ED890, 0x6DF82145, 0xD4186F38, 0xD4651074, 0x0219B535, 0x25028591, 0x55D62C10, 0xC62CBE25, 0x461B5D07,0x40E76915, 0x05E7EB35,
};
// //uralsdev 04-2015   New Seeds

class CMainParams : public CChainParams {
public:
    CMainParams() {
        // The message start string is designed to be unlikely to occur in normal data.
        // The characters are rarely used upper ASCII, not valid as UTF-8, and produce
        // a large 4-byte int at any alignment.
        pchMessageStart[0] = 0xa3;    //uralsdev 04-2015   mainc.pp 3072    ggf testnet wert = falshc
        pchMessageStart[1] = 0xd5;    ///uralsdev 04-2015
        pchMessageStart[2] = 0xc2;    //uralsdev 04-2015
        pchMessageStart[3] = 0xf9;    //uralsdev 04-2015

        
  //    name, networkVersion, privateKeyPrefix, WIF_Start, CWIF_Start, donate
  // ("Urals",  0x66, 0xcc, "7",    "X"    , "  ")
  
        vAlertPubKey = ParseHex("048240a8748a80a286b270ba126705ced4f2ce5a7847b3610ea3c06513150dade2a8512ed5ea86320824683fc0818f0ac019214973e677acd1244f6d0571fc5103"); //uralsdev 04-2015   ist bei allen gleich
//        vAlertPubKey = ParseHex("04ffff001d01041f51756f64206c69636574206a6f76692c206e6f6e206c6963657420626f7669"); //uralsdev 04-2015   ist bei allen gleich
////	vAlertPubKey = ParseHex("0x04678afdb0fe5548271967f1a67130b7105cd6a828e03909a67962e0ea1f61deb649f6bc3f4cef38c4f35504e51ec112de5c384df7ba0b8d578a4c702b6bf11d5f"); //Urals generated
        nDefaultPort = 7444;
        nRPCPort = 7443;
        bnProofOfWorkLimit = CBigNum(~uint256(0) >> 20);  // Urals starting difficulty is 1 / 2^12    //uralsdev 04-2015   main.cpp 36
        nSubsidyHalvingInterval = 210000; //uralsdev 04-2015   UNBEKANNT FUNKTION

        // Genesis block            Sprungmarke AAAAB
//        const char* pszTimestamp = "On September 1, 1581, the Cossack squad under the command of Ermak made a campaign for the Stone Belt (the Urals)";
        const char* pszTimestamp = "28/06/2017: Russia’s Parliament is Discussing the Legalization of Bitcoin.";
        CTransaction txNew;
        txNew.vin.resize(1);
        txNew.vout.resize(1);
        txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        txNew.vout[0].nValue = 100 * COIN; //uralsdev 04-2015
        //txNew.vout[0].nValue = 50 * COIN; //uralsdev 04-2015
        txNew.vout[0].scriptPubKey = CScript() << ParseHex("04678afdb0fe5548271967f1a67130b7105cd6a828e03909a67962e0ea1f61deb649f6bc3f4cef38c4f35504e51ec112de5c384df7ba0b8d578a4c702b6bf11d5f") << OP_CHECKSIG;   //urals
        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock = 0;
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nVersion = 1;
        //genesis.nTime    = 1501545600;   //uralsdev 07-2017
        //genesis.nTime    = 1504224000;   //uralsdev 07-2017
        genesis.nTime    = 1503878400;   //uralsdev 07-2017
        //genesis.nBits    = 0x1e0ffff0;  //number of coins
        genesis.nBits    = 0x1e0fffff;  //number of coins
        genesis.nNonce   = 2373747;  //uralsdev 06-2017

        hashGenesisBlock = genesis.GetHash();
	
LogPrintf("Mainnet: nonce %08u: hash = %s \n", genesis.nNonce, hashGenesisBlock.ToString().c_str());
LogPrintf("Mainnet: nonce %08u: Merkle = %s \n", genesis.nNonce, genesis.hashMerkleRoot.ToString().c_str());
LogPrintf("Mainnet: nonce %08u: min nBit =  %08x\n",genesis.nNonce, bnProofOfWorkLimit.GetCompact());
//LogPrintf("Mainnet: nonce %08u: "hash 2 = %s\n", genesis.nNonce, hash.ToString().c_str());

        assert(hashGenesisBlock == uint256("0xe453fa9b1dac51b440076dc8bdbf2b82d6c006232fcff620bcd82bfbc24a2b69"));
        assert(genesis.hashMerkleRoot == uint256("0x5bab319403ecce5eccd4715162fd72d35313064211ff16fccde441a12b2b93b8")); 
/*
Mainnet: nonce 02373747: hash = 356175917440e0acbcd7802b294c2c0b9545697a1a36bb14f5cc780f57aad58b 
Mainnet: nonce 02373747: Merkle = 03bb497b4a6a17984b048a0b3351019e8a68a345b8e2b817310b7f9afffd92c
*/

/*	//// debug print
        printf("genesis.GetHash() = %s\n", genesis.GetHash().ToString().c_str());
        printf("genesis.hashMerkleRoot = %s\n", genesis.hashMerkleRoot.ToString().c_str());

        // If genesis block hash does not match, then generate new genesis hash.
        //if (true && block.GetHash() != (!fTestNet ? hashGenesisBlock : hashGenesisBlockTestNet))
        if (true && genesis.GetHash() != hashGenesisBlock )
        {
            printf("Searching for genesis block...\n");
            // This will figure out a valid hash and Nonce if you're
            // creating a different genesis block:
            uint256 hashTarget = CBigNum().SetCompact(genesis.nBits).getuint256();
            uint256 thash;
            char scratchpad[SCRYPT_SCRATCHPAD_SIZE];

            while(true)
            {
                scrypt_1024_1_1_256_sp(BEGIN(genesis.nVersion), BEGIN(thash), scratchpad);
                if (thash <= hashTarget)
                    break;
                if ((genesis.nNonce & 0xFFF) == 0)
                {
                    printf("nonce %08X: hash = %s (target = %s)\n", genesis.nNonce, thash.ToString().c_str(), hashTarget.ToString().c_str());
                }
               ++genesis.nNonce;
                if (genesis.nNonce == 0)
                {
                    printf("NONCE WRAPPED, incrementing time\n");
                    ++genesis.nTime;
                }
            }

                        printf("genesis.GetHash = %s\n", genesis.GetHash().ToString().c_str());
        }

        printf("After IF\n");
        genesis.print();
//        assert(genesis.hashMerkleRoot == uint256("0xd006aa2081c4d917877d5cc6ee942903374978568688d6fd76fc3b0d5fbf7287"));
//      assert(hashGenesisBlock == uint256("0x831ba083feeef4998e3f12e7143a96660d3889d532c186a98c8e45d78fda68e1"));

        assert(genesis.hashMerkleRoot == uint256("0xd006aa2081c4d917877d5cc6ee942903374978568688d6fd76fc3b0d5fbf7287"));
        assert(hashGenesisBlock == uint256("0xeb7df35c2dc2ee458a6d6e6f80fb834ad709e6410b6dd6de857f2c50e72850aa"));

 */
  
		vSeeds.push_back(CDNSSeedData("195.54.3.213", "195.54.3.213"));
//		vSeeds.push_back(CDNSSeedData("37.120.186.85", "37.120.186.85"));
//		vSeeds.push_back(CDNSSeedData("37.120.190.76", "37.120.190.76"));
//		vSeeds.push_back(CDNSSeedData("213.136.80.93", "213.136.80.93"));
//		vSeeds.push_back(CDNSSeedData("213.136.86.202", "213.136.86.202"));
//		vSeeds.push_back(CDNSSeedData("213.136.86.205", "213.136.86.205"));
//		vSeeds.push_back(CDNSSeedData("213.136.86.207", "213.136.86.207"));
		


        base58Prefixes[PUBKEY_ADDRESS] = list_of( 102);                    //uralsdev 04-2015   addresses start with 'i'
        base58Prefixes[SCRIPT_ADDRESS] = list_of( 5);                    //uralsdev 04-2015   script addresses start with '7'
        base58Prefixes[SECRET_KEY] =     list_of(204);                    //uralsdev 04-2015  Pubkey +128 uralsdev
        base58Prefixes[EXT_PUBLIC_KEY] = list_of(0x04)(0x88)(0xB2)(0x1E); //uralsdev 04-2015  'xpub '
        base58Prefixes[EXT_SECRET_KEY] = list_of(0x04)(0x88)(0xAD)(0xE4); //uralsdev 04-2015  'xpriv'
        base58Prefixes[EXT_COIN_TYPE]  = list_of(0x80000005);             // Urals BIP44 coin type is '5'

        // Convert the pnSeeds array into usable address objects.
        for (unsigned int i = 0; i < ARRAYLEN(pnSeed); i++)
        {
            // It'll only connect to one or two seed nodes because once it connects,
            // it'll get a pile of addresses with newer timestamps.
            // Seed nodes are given a random 'last seen time' of between one and two
            // weeks ago.
            const int64_t nOneWeek = 7*24*60*60;
            struct in_addr ip;
            memcpy(&ip, &pnSeed[i], sizeof(ip));
            CAddress addr(CService(ip, GetDefaultPort()));
            addr.nTime = GetTime() - GetRand(nOneWeek) - nOneWeek;
            vFixedSeeds.push_back(addr);
        }
    }

    virtual const CBlock& GenesisBlock() const { return genesis; }
    virtual Network NetworkID() const { return CChainParams::MAIN; }

    virtual const vector<CAddress>& FixedSeeds() const {
        return vFixedSeeds;
    }
protected:
    CBlock genesis;
    vector<CAddress> vFixedSeeds;
};
static CMainParams mainParams;


//
// Testnet (v3)
//
class CTestNetParams : public CMainParams {
public:
    CTestNetParams() {
        // The message start string is designed to be unlikely to occur in normal data.
        // The characters are rarely used upper ASCII, not valid as UTF-8, and produce
        // a large 4-byte int at any alignment.
        pchMessageStart[0] = 0x4c;   //uralsdev 04-2015
        pchMessageStart[1] = 0x1a;   //uralsdev 04-2015
        pchMessageStart[2] = 0x2c;   //uralsdev 04-2015
        pchMessageStart[3] = 0xaf;   //uralsdev 04-2015   Testnet

        vAlertPubKey = ParseHex("04678afdb0fe5548271967f1a67130b7105cd6a828e03909a67962e0ea1f61deb649f6bc3f4cef38c4f35504e51ec112de5c384df7ba0b8d578a4c702b6bf11d5f");
//        vAlertPubKey = ParseHex("04517d8a699cb43d3938d7b24faaff7cda448ca4ea267723ba614784de661949bf632d6304316b244646dea079735b9a6fc4af804efb4752075b9fe2245e14e412");
        nDefaultPort = 17443;
        nRPCPort = 17444;
        strDataDir = "testnet3";

        // Modify the testnet genesis block so the timestamp is valid for a later start.
        genesis.nTime    = 1503878400;   //uralsdev 07-2017
        genesis.nNonce   = 2373747;  //uralsdev 06-2017
	//genesis.nBits    = 0x1e0ffff0;

        hashGenesisBlock = genesis.GetHash();
        assert(hashGenesisBlock == uint256("0xe453fa9b1dac51b440076dc8bdbf2b82d6c006232fcff620bcd82bfbc24a2b69"));  //uralsdev 

        vFixedSeeds.clear();
        vSeeds.clear();
        /*vSeeds.push_back(CDNSSeedData("5.56.209.255", "5.56.209.255"));   //uralsdev 04-2015
        vSeeds.push_back(CDNSSeedData("", ""));       //uralsdev 04-2015 vSeeds.push_back(CDNSSeedData("urals.qa", "testnet-seed.urals.qa"));
        *///legacy seeders
        vSeeds.push_back(CDNSSeedData("Demoserver1", "195.54.3.213"));      //uralsdev 04-2015
        vSeeds.push_back(CDNSSeedData("", ""));      //uralsdev 04-2015
        vSeeds.push_back(CDNSSeedData("", ""));       //uralsdev 04-2015

        base58Prefixes[PUBKEY_ADDRESS] = list_of(111);                    //uralsdev 04-2015   Testnet vorher 139 urals addresses start with 'x' or 'y'
        base58Prefixes[SCRIPT_ADDRESS] = list_of( 196);                    //uralsdev 04-2015   Testnet vorher 19 urals script addresses start with '8' or '9'
        base58Prefixes[SECRET_KEY]     = list_of(211);                    ///uralsdev 04-2015   Testnet vorher 239  private keys start with '9' or 'c' (Bitcoin defaults)
        base58Prefixes[EXT_PUBLIC_KEY] = list_of(0x04)(0x35)(0x87)(0xCF); //uralsdev 04-2015   tpub
        base58Prefixes[EXT_SECRET_KEY] = list_of(0x04)(0x35)(0x83)(0x94); //uralsdev 04-2015  tprv
        base58Prefixes[EXT_COIN_TYPE]  = list_of(0x80000001);             // Testnet urals BIP44 coin type is '5' (All coin's testnet default)
    }
    virtual Network NetworkID() const { return CChainParams::TESTNET; }
};
static CTestNetParams testNetParams;


//
// Regression test
//
class CRegTestParams : public CTestNetParams {
public:
    CRegTestParams() {
      // Alle Parameter hier unverändert
        pchMessageStart[0] = 0xfc;          // Uralsdev 04-2015 old    0xfc
        pchMessageStart[1] = 0xc1;          // Uralsdev 04-2015 old    0xc1
        pchMessageStart[2] = 0xb7;         // Uralsdev 04-2015 old     0xb7
        pchMessageStart[3] = 0xdc;          // Uralsdev 04-2015 old    0xdc
        nSubsidyHalvingInterval = 150;
        bnProofOfWorkLimit = CBigNum(~uint256(0) >> 1);

        genesis.nTime    = 1503878400;   //uralsdev 07-2017
        genesis.nNonce   = 2373747;  //uralsdev 06-2017
        //genesis.nBits = 0x1e0ffff0;        // Uralsdev 
        nDefaultPort = 17445;             // Uralsdev 04-2015 old   19994
        strDataDir = "regtest";

        hashGenesisBlock = genesis.GetHash();
        assert(hashGenesisBlock == uint256("0xe453fa9b1dac51b440076dc8bdbf2b82d6c006232fcff620bcd82bfbc24a2b69"));   // Uralsdev 

        vSeeds.clear();  // Regtest mode doesn't have any DNS seeds.
    }

    virtual bool RequireRPCPassword() const { return false; }
    virtual Network NetworkID() const { return CChainParams::REGTEST; }
};
static CRegTestParams regTestParams;

static CChainParams *pCurrentParams = &mainParams;

const CChainParams &Params() {
    return *pCurrentParams;
}

void SelectParams(CChainParams::Network network) {
    switch (network) {
        case CChainParams::MAIN:
            pCurrentParams = &mainParams;
            break;
        case CChainParams::TESTNET:
            pCurrentParams = &testNetParams;
            break;
        case CChainParams::REGTEST:
            pCurrentParams = &regTestParams;
            break;
        default:
            assert(false && "Unimplemented network");
            return;
    }
}

bool SelectParamsFromCommandLine() {
    bool fRegTest = GetBoolArg("-regtest", false);
    bool fTestNet = GetBoolArg("-testnet", false);

    if (fTestNet && fRegTest) {
        return false;
    }

    if (fRegTest) {
        SelectParams(CChainParams::REGTEST);
    } else if (fTestNet) {
        SelectParams(CChainParams::TESTNET);
    } else {
        SelectParams(CChainParams::MAIN);
    }
    return true;
}
