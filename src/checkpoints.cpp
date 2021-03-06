// Copyright (c) 2009-2014 The Bitcoin developers
// Copyright (c) 2014-2015 The BitSend developers
// Copyright (c) 2017 The Urals developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "checkpoints.h"

#include "main.h"
#include "uint256.h"

#include <stdint.h>

#include <boost/assign/list_of.hpp> // for 'map_list_of()'
#include <boost/foreach.hpp>

namespace Checkpoints
{
    typedef std::map<int, uint256> MapCheckpoints;

    // How many times we expect transactions after the last checkpoint to
    // be slower. This number is a compromise, as it can't be accurate for
    // every system. When reindexing from a fast disk with a slow CPU, it
    // can be up to 20, while when downloading from a slow network with a
    // fast multicore CPU, it won't be much higher than 1.
    static const double SIGCHECK_VERIFICATION_FACTOR = 5.0;

    struct CCheckpointData {
        const MapCheckpoints *mapCheckpoints;
        int64_t nTimeLastCheckpoint;
        int64_t nTransactionsLastCheckpoint;
        double fTransactionsPerDay;
    };

    bool fEnabled = true;

    // What makes a good checkpoint block?
    // + Is surrounded by blocks with reasonable timestamps
    //   (no blocks before with a timestamp after, none after with
    //    timestamp before)
    // + Contains no strange transactions
    // uralsdev urals 2015-04
    static MapCheckpoints mapCheckpoints =
        boost::assign::map_list_of
          (  0, uint256("0x00000bf56638a0f85009007e92cd848160d5211da779fd4e23f4bd22f0f5221a")) //uralsdev 09-2017
	( 100, uint256("0x00000d1135604f0f6391f3b16d05c348c8901cbace293ecfd84a5f535a4f8e18"))
	( 3000, uint256("0x000004d7d9610d957b43a160fb90c4f56961e91509bf17f18c42ba2060273c36"))
	( 5000, uint256("0x00000362aba9ec91f776f7d0d7e8e871ddb1af95b03dd62ce860fb15e0c14ba5"))
	( 10000, uint256("0x000000577880073a284ac6ebf21b7e380c4ae6ab6f9d040c8472d6f1fda107da"))
	( 20000, uint256("0x000000065f35161847c1ed0c2fee02596fb99931fa316522db330f782f5f3882"))
	( 25000, uint256("0x0000000002a801db19ecda061a2be1e2428ebd112d9ddbc2b6ed3305267594c7"))
	( 30000, uint256("0x00000000446eebe1b729b7d192d9b0f74b95e541eddee5419c6c8c47278f7e77"))
	( 35000, uint256("0x000000002e8369b82c543ffc6d2dcfedf36662e0850641e5ff3348b6b16b1c35"))
	( 41000, uint256("0x0000000005e76c8e5e676ae4da985eed849c0abb2a878c010dd76837a9aa7820"))
	( 45500, uint256("0x00000000060f5fafdaeb078fd339c8473e8081e4119a220be7fee4eadd44d8b8"))
	( 49010, uint256("0x00000000075e6171028f08883ebd88075576813e8d1b7bedd4b9eb76d2fa172e"))
//	( 140602, uint256("0x0000000000b86fa0891a7241c71a0969439896b61abaf07e856eb0f49115b741"))
//	( 186158, uint256("0x0000000002d477cc1fea0438f7d477c0c993ae3762bc60efdbb8873275b385c7"))
//	( 212573, uint256("0x0000000da60270f8183780773064d689762b6ad6749296e9fc274c3dcecfc6f6"))
//	( 279122, uint256("0x00000002e14806c9d2aa7afeeb43df69b90ab8c6083b33b34fd9d62c57dac559"))
        ;
    static const CCheckpointData data = {
        &mapCheckpoints,
        1505121347, // * UNIX timestamp of last checkpoint block
	5000,    // * total number of transactions between genesis and last checkpoint //
                    //   (the tx=... number in the SetBestChain debug.log lines)
        350     // * estimated number of transactions per day after checkpoint //800
    };

    static MapCheckpoints mapCheckpointsTestnet =
        boost::assign::map_list_of
	 ( 0, Params().HashGenesisBlock() )
//        ( 0 , uint256("0x000008030a1e9a647ecc6119e0782166552e49dadfa8353afa26f3a6c2179845"))   //uralsdev 04-2015
        ;
    static const CCheckpointData dataTestnet = {
        &mapCheckpointsTestnet,
        1402095180,
        3000,
        30
    };

    static MapCheckpoints mapCheckpointsRegtest =
        boost::assign::map_list_of
 ( 0, Params().HashGenesisBlock() )
//        ( 0, uint256("0x000008ca1832a4baf228eb1553c03d3a2c8e02399550dd6ea8d65cec3ef23d2e"))
        ;
    static const CCheckpointData dataRegtest = {
        &mapCheckpointsRegtest,
        0,
        0,
        0
    };

    const CCheckpointData &Checkpoints() {
        if (Params().NetworkID() == CChainParams::TESTNET)
            return dataTestnet;
        else if (Params().NetworkID() == CChainParams::MAIN)
            return data;
        else
            return dataRegtest;
    }

    bool CheckBlock(int nHeight, const uint256& hash)
    {
        if (!fEnabled)
            return true;

        const MapCheckpoints& checkpoints = *Checkpoints().mapCheckpoints;

        MapCheckpoints::const_iterator i = checkpoints.find(nHeight);
        if (i == checkpoints.end()) return true;
        return hash == i->second;
    }

    // Guess how far we are in the verification process at the given block index
    double GuessVerificationProgress(CBlockIndex *pindex, bool fSigchecks) {
        if (pindex==NULL)
            return 0.0;

        int64_t nNow = time(NULL);

        double fSigcheckVerificationFactor = fSigchecks ? SIGCHECK_VERIFICATION_FACTOR : 1.0;
        double fWorkBefore = 0.0; // Amount of work done before pindex
        double fWorkAfter = 0.0;  // Amount of work left after pindex (estimated)
        // Work is defined as: 1.0 per transaction before the last checkpoint, and
        // fSigcheckVerificationFactor per transaction after.

        const CCheckpointData &data = Checkpoints();

        if (pindex->nChainTx <= data.nTransactionsLastCheckpoint) {
            double nCheapBefore = pindex->nChainTx;
            double nCheapAfter = data.nTransactionsLastCheckpoint - pindex->nChainTx;
            double nExpensiveAfter = (nNow - data.nTimeLastCheckpoint)/86400.0*data.fTransactionsPerDay;
            fWorkBefore = nCheapBefore;
            fWorkAfter = nCheapAfter + nExpensiveAfter*fSigcheckVerificationFactor;
        } else {
            double nCheapBefore = data.nTransactionsLastCheckpoint;
            double nExpensiveBefore = pindex->nChainTx - data.nTransactionsLastCheckpoint;
            double nExpensiveAfter = (nNow - pindex->nTime)/86400.0*data.fTransactionsPerDay;
            fWorkBefore = nCheapBefore + nExpensiveBefore*fSigcheckVerificationFactor;
            fWorkAfter = nExpensiveAfter*fSigcheckVerificationFactor;
        }

        return fWorkBefore / (fWorkBefore + fWorkAfter);
    }

    int GetTotalBlocksEstimate()
    {
        if (!fEnabled)
            return 0;

        const MapCheckpoints& checkpoints = *Checkpoints().mapCheckpoints;

        return checkpoints.rbegin()->first;
    }

    CBlockIndex* GetLastCheckpoint(const std::map<uint256, CBlockIndex*>& mapBlockIndex)
    {
        if (!fEnabled)
            return NULL;

        const MapCheckpoints& checkpoints = *Checkpoints().mapCheckpoints;

        BOOST_REVERSE_FOREACH(const MapCheckpoints::value_type& i, checkpoints)
        {
            const uint256& hash = i.second;
            std::map<uint256, CBlockIndex*>::const_iterator t = mapBlockIndex.find(hash);
            if (t != mapBlockIndex.end())
                return t->second;
        }
        return NULL;
    }
}
