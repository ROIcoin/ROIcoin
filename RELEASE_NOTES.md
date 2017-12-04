# ** ROIcoin Release Notes for v1.1.0.0

This release contains one critical and some minor fixes.

The critical fix is for the term deposit interest rate algorithm bug that kept increasing on every block.
The new wallet will make sure that if you have pre-fork locked term deposits, the maturation interest will remain unchanged.
New term deposits created after the hardfork will obey the new term deposit rates:

| Time     | Start Coins | End Coins | Coin Gain | Interest |
| :--------: | :--------: | :--------: | :--------: | :--------: |
| 1 week | 1000 | 1021 | 21 | 2,10% |
| 2 weeks | 1000 | 1043 | 43 | 4,30% |
| 1 month | 1000 | 1096 | 96 | 9,60% |
| 3 months | 1000 | 1331 | 331 | 33,10% |
| 6 months | 1000 | 1929 | 929 | 92,90% |
| 12 months | 1000 | 6933 | 5933 | 593,30% |

The PoS calculation has been adjusted to 15% APR 
**Note:** the old HOdl per block 30 day interest still applies on top of ROI PoS rate.

We also applied cosmetic branding changes to the Wallet splashscreen and background.

## Hardfork at block 20000
### Notice to miners:
* If you are solo-mining you must upgrade to v1.1.0.0 before reaching block 20000 , any blocks found with the old wallet after the fork will be on the wrong chain !.
* If you are pool-mining make sure to check with the pool owner that their wallet has upgraded to v1.1.0.0 before block 20000
* Exchange owners please upgrade to v1.1.0.0 as soon as possible.

ROIcoin core v1.1.0.0 is now available at:
https://github.com/ROIcoin/ROIcoin/releases/

Please report bugs using the issue tracker at github:
https://github.com/ROIcoin/ROIcoin/issues

## Upgrading and downgrading
### How to Upgrade

If you are running an older version, shut it down. Wait until it has completely
shut down (which might take a few minutes for older versions).
Backup your wallet.dat file ( which you should always do regardless of an upgrade event ) 
then run the installer (on Windows) or just copy over ROIcoin-qt (on Linux).

### Downgrade warning

Because this release issues a hard-fork of the network, downgrading to prior versions
will not be possible once the network reaches a block height of 20000.

## Credits:
* PlainKoin
* DisasterFaster
* ghobson2013

