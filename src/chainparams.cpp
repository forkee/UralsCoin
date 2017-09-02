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
  
        vAlertPubKey = ParseHex("048240a8748a80a286b270ba126705ced4f2ce5a7847b3610ea3c06513150dade2a8512ed5ea86320824683fc0818f0ac019214973e677acd1244f6d0571fc5103"); 
        nDefaultPort = 7444;
        nRPCPort = 7443;
        bnProofOfWorkLimit = CBigNum(~uint256(0) >> 20);  // Urals starting difficulty is 1 / 2^12    
        nSubsidyHalvingInterval = 210000; 

        // Genesis block            
        const char* pszTimestamp = "28/06/2017: Russia’s Parliament is Discussing the Legalization of Bitcoin.";
        CTransaction txNew;
        txNew.vin.resize(1);
        txNew.vout.resize(1);
        txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        txNew.vout[0].nValue = 100 * COIN; 
        txNew.vout[0].scriptPubKey = CScript() << ParseHex("04678afdb0fe5548271967f1a67130b7105cd6a828e03909a67962e0ea1f61deb649f6bc3f4cef38c4f35504e51ec112de5c384df7ba0b8d578a4c702b6bf11d5f") << OP_CHECKSIG;   //urals
        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock = 0;
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nVersion = 1;
        genesis.nTime    = 1504224000;  
        genesis.nBits    = 0x1e0ffff0;  
        genesis.nNonce   = 25912842;

        hashGenesisBlock = genesis.GetHash();
	

        assert(hashGenesisBlock == uint256("0x00000bf56638a0f85009007e92cd848160d5211da779fd4e23f4bd22f0f5221a"));
        assert(genesis.hashMerkleRoot == uint256("0x5bab319403ecce5eccd4715162fd72d35313064211ff16fccde441a12b2b93b8"));

  
		vSeeds.push_back(CDNSSeedData("195.54.3.213", "195.54.3.213"));
		vSeeds.push_back(CDNSSeedData("151.248.118.2", "151.248.118.2"));
		

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,130);                    //uralsdev 09-2017   addresses start with 'u'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,5);                    //script addresses start with '7'
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,204);                    //Pubkey +128 uralsdev
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x88)(0xB2)(0x1E).convert_to_container<std::vector<unsigned char> >(); // 'xpub '
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x88)(0xAD)(0xE4).convert_to_container<std::vector<unsigned char> >(); // 'xpriv'
        base58Prefixes[EXT_COIN_TYPE]  = std::vector<unsigned char>(1,0x80000005);             // Urals BIP44 coin type is '5'
//        base58Prefixes[EXT_COIN_TYPE]  = list_of(0x80000005);             //Old format uralsdev 

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
//TODO: Rebuild testnet and regtest genesis to complify difficulty check
class CTestNetParams : public CMainParams {
public:
    CTestNetParams() {
        // The message start string is designed to be unlikely to occur in normal data.
        // The characters are rarely used upper ASCII, not valid as UTF-8, and produce
        // a large 4-byte int at any alignment.
        pchMessageStart[0] = 0x4c;   
        pchMessageStart[1] = 0x1a;   
        pchMessageStart[2] = 0x2c;   
        pchMessageStart[3] = 0xaf;   

        vAlertPubKey = ParseHex("04678afdb0fe5548271967f1a67130b7105cd6a828e03909a67962e0ea1f61deb649f6bc3f4cef38c4f35504e51ec112de5c384df7ba0b8d578a4c702b6bf11d5f");
        nDefaultPort = 17444;
        nRPCPort = 17443;
        strDataDir = "testnet3";

        // Modify the testnet genesis block so the timestamp is valid for a later start.
        genesis.nTime    = 1503878400;   //uralsdev 07-2017
        genesis.nNonce   = 2373747;  //uralsdev 06-2017
	//genesis.nBits    = 0x1e0ffff0;

        hashGenesisBlock = genesis.GetHash();
        assert(hashGenesisBlock == uint256("0x559287affd03319ca1eb5febe7b8141a8ddfb151634371c2b6be708c023aa9a6"));  //uralsdev 

        vFixedSeeds.clear();
        vSeeds.clear();
        vSeeds.push_back(CDNSSeedData("", ""));       

    
       base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,141);                    // Testnet addresses start with 'y' or 'z'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);                    // urals script addresses start with '8' or '9'
        base58Prefixes[SECRET_KEY]     = std::vector<unsigned char>(1,211);                    /// Testnet  private keys start with '9' or 'c' (Bitcoin defaults)
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >(); //  tpub
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >(); //  tprv
        base58Prefixes[EXT_COIN_TYPE]  = std::vector<unsigned char>(1,0x80000001);             // Testnet urals BIP44 coin type is '5' (All coin's testnet default)

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
        assert(hashGenesisBlock == uint256("0x559287affd03319ca1eb5febe7b8141a8ddfb151634371c2b6be708c023aa9a6"));   // Uralsdev 

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
