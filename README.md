# **ROI Coin is _"Different"_** 

<img src="https://roi-coin.com/wp-content/uploads/2018/04/roi-coin-marketing-graphics-1.jpg?w1500=&h=500" width="1500">

> * __Website:__ [https://roi-coin.com](https://roi-coin.com)
> * __Facebook:__ [https://www.facebook.com/roicoin](https://www.facebook.com/roicoin)
> * __Twitter:__ [https://twitter.com/ROI_Coin](https://twitter.com/ROI_Coin)
> * __Discord:__ [https://discord.gg/ruhSp4e](https://discord.gg/ruhSp4e)
> * __Telegram:__ [https://t.me/joinchat/I0JIzQ72Fz6EWWAMwQURhQ](https://t.me/joinchat/I0JIzQ72Fz6EWWAMwQURhQ)
> * __YouTube:__ [https://www.youtube.com/channel/UCclukUCxqxHk-nrUSj3MM5g](https://www.youtube.com/channel/UCclukUCxqxHk-nrUSj3MM5g)

## ROI Coin Resources
> * __Block Explorer 1:__ [https://roi-coin-blockexplorer.roi-coin.com](https://roi-coin-blockexplorer.roi-coin.com)
> * __Block Explorer 2:__ [https://blockexplorer.roi-coin.com](https://blockexplorer.roi-coin.com)
> * __AltMarkets Exchange:__ [https://altmarkets.io/trading/roibtc](https://altmarkets.io/trading/roibtc)
> * __BitexLive Exchange:__ [https://bitexlive.com/exchange/USDT-ROI](https://bitexlive.com/exchange/USDT-ROI)
> * __SuprNova Mining Pool:__ [https://roi.suprnova.cc](https://roi.suprnova.cc)
> * __Rplant Mining Pool:__ [https://pool.rplant.xyz](https://pool.rplant.xyz)
> * __Coinspool Mining Pool:__ [http://www.coinspool.cc](http://www.coinspool.cc)


## What is ROI Coin?

> **ROI Coin** is a Cryptocurrency built on the ROI Coin core ver. 11.3, incorporating HOdl PoS to reward savers.  
> It pays interest on every balance as well as higher interest rates for locked term deposits and that is precisely why ROI Coin is different.

### Compound Interest on _all Balances_

> Interest is paid on all outputs (Balances) and compounded on each block. This is to discourage rolling outputs into new blocks simply to compound interest.

### Benefits:

> -  Exciting for Stakeholder to see their balances continually increase as each block is received.
> -  Interest discourages users from leaving large balances on exchanges - expensive to maintain large sell walls.
> -  Interest encourages users to keep their balances, reducing supply while increasing market demand.
> -  30 day limit will dilute abandoned balances, reducing supply.
> -  30 day limit encourages term deposits, reducing supply and increasing demand.
> -  Encourages exchanges to trade in the coin, as they can earn interest on customer's deposits.
> -  Unconfirmed transactions become more valuable over time, as their inputs continue to earn interest. 

### Deposit Interest

> - Paid on Term Deposits (aka Fixed deposit / time deposit)
> - Regular deposits can have a maximum of 1 year interest
> - Term deposits may have up to 2 years of interest
> - This allows users to lock up funds for a specified amount of time up to two years.

## Why?

> -  This encourages and reward Stakeholder.
> -  Term deposits also constrain supply, increasing demand - term deposit coins cannot be moved until term ends.
> - Interest is handled in the blockchain and protocol using CHECKLOCKTIMEVERIFY. There is no counterparty. 

### Fixed Parameters

> - The PoW Algorithm is considered a technical detail and is subject to change to favor CPU and consumer grade hardware with the intention of keeping mining participatory and distributed.

### Specifications:

> - 1 MB Blocksize
> - Max TX is 250 KB ( ~ 1500 TXs)
> - COINBASE_MATURITY = 360;
> - 120 second blocks
> - 120 ROI subsidy per block (no halving)
> - Total of 1,000,000,000 ROI will be mined over a 30 year period (7,884,000 blocks) or 86,400 coins per day.
> - Total coinbase potentially 4 Billion coins. Variables depending on behavior of coin owners, term depositing and period of time for locked funds.

## Term Deposits

| Time     | Start Coins | End Coins | Coin Gain | Interest |
| :--------: | :--------: | :--------: | :--------: | :--------: |
| 1 week  | 100 | 100.592 | .592 | .59% |
| 2 weeks  | 100 | 101.163 | 1.163 | 1.16% |
| 1 month  | 100 | 102.397 | 2.397 | 2.40% |
| 3 months  | 100 | 106.280 | 6.280 | 6.28% |
| 6 months   | 100 | 110.646 | 10.646 | 10.65% |
| 12 months | 100 | 118.065 | 18.065 | 18.07% |



> **Note:**
> 
> - When a Term Deposit matures, it stops earning interest - you need to move it to start earning interest again.
> -  Bonus rates are paid on regular balances as well.
> - The bonus rate is locked at the time of the transaction, the rate you can achieve reduces over time due to the multiplier, but once you're earning that bonus rate, it doesn't reduce.

## Proof of Work

### 1GB AES Pattern Search PoW (HOdl)

ASIC/GPU resistant. Pattern Search involves filling up RAM with pseudo-random data, and then conducting a search for the start location of an AES encrypted data pattern in that data. Pattern Search is an evolution of the ProtoShares Momentum PoW, first used in MemoryCoin and later modified for use in CryptoNote (Monero,Bytecoin), Ethash(Ethereum), HOdl Coin, EuropeCoin and now ROI Coin.

## ROI Coin Ubuntu Compile Dependencies

### Compiling on Ubuntu (16.04 tested)):

### Preparing your environment

As per the requirements and dependencies specifications in doc/build-unix.md, it may be necessary to install the required dependencies prior to downloading and compiling the source. Following the steps in this section will leave you with a fully prepared build environment.

### For basic building

sudo apt-get install build-essential libtool autotools-dev autoconf pkg-config libssl-dev libboost-all-dev git

### For Berkeley DB 4.8

The wallet.dat file relies on BerkeleyDB (BDB), specifically version 4.8 to maintain binary compatibility. Ubuntu 14.04+ doesn't provide this version however, so you'll need to install it from the Bitcoin PPA.

sudo apt-get install software-properties-common python-software-properties

sudo add-apt-repository ppa:bitcoin/bitcoin

sudo apt-get update

sudo apt-get install libdb4.8-dev libdb4.8++-dev

### For QT5 GUI client building

If you are only intending to compile the CLI daemon, skip this step.

sudo apt-get install libqt5gui5 libqt5core5a libqt5dbus5 qttools5-dev qttools5-dev-tools libprotobuf-dev protobuf-compiler

### For QR codes capabilities

If you are only intending to compile the CLI daemon, skip this step. 

sudo apt-get install libqrencode-dev

### For Miniupnpc capabilities

sudo apt-get install libminiupnpc-dev

### Github Source Code: (note: do not "git" under sudo)

git clone https://github.com/ROIcoin/ROI-Coin.git ROIcoin

cd ROIcoin/

"sudo su":

./autogen.sh

./configure

make install

### Configuration options

The configure script has options that can change the feature set and what gets compiled. Below are some common 'flags' that can or should be used depending on your dependencies choices or your intended compile result

--with-gui=no           (disables compiling the gui wallet even if qt is available)
--without-miniupnpc     (disables miniupnpc support entierly (miniupnpc not required))
--enable-upnp-default   (forces miniupnpc to be used by default (miniupnpc required)

For example, if you only want to compile the CLI daemon:

./configure --with-gui=no

### Installing to a better location

If you want the binaries copied to /usr/local/bin

make install

This may require root privs. Run sudo make install if the above gives an error.

Or you can copy thse binaries to the folder of your choice manually.

### Running the compiled binaries

If you DID NOT run make install, then after compilation finishes, and from the same directory that the compilation steps were done in, use either of the following:

./src/ROIcoind (starts the CLI daemon)

./src/qt/ROIcoin-qt (starts the GUI wallet)

### Setting up the ROIcoin.conf file

You can access/create/edit the conf file from the terminal using the following commands:

cd ~/.ROIcoin/

nano ROIcoin.conf

__The minimum recommended entries for the conf file are as follows:__

rpcuser=xxxx  (Replace the strings of 'xxxx' with preferred data.)<br />
rpcpassword=xxxx<br />
rpcport=3377<br />
daemon=1<br />
server=1<br />
listen=1<br />



