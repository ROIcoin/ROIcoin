# **ROI coin is _"Return on Investment"_** 

ROIcoin <img src="https://d25lcipzij17d.cloudfront.net/badge.svg?id=gh&type=6&v=1.1.0.0&x2=0">

<img src="https://roi-coin.com/ow_userfiles/themes/theme_image_17.png?w1720=&h=521" width="1720">

> * __Website:__ [https://roi-coin.com](https://roi-coin.com)
> * __Facebook:__ [https://www.facebook.com/roicoin](https://www.facebook.com/roicoin)
> * __Twitter:__ [https://twitter.com/ROI_Coin](https://twitter.com/ROI_Coin)
> * __Google Plus:__ [https://plus.google.com/b/109809615316952270396/109809615316952270396](https://plus.google.com/b/109809615316952270396/109809615316952270396)
> * __YouTube:__ [https://www.youtube.com/channel/UCclukUCxqxHk-nrUSj3MM5g](https://www.youtube.com/channel/UCclukUCxqxHk-nrUSj3MM5g)

## What is ROI coin?

> **ROI coin** is a Cryptocurrency built on the ROI coin core ver. 11.3, incorporating HOdl PoS to reward savers.  
> It pays interest on every balance as well as higher interest rates for locked term deposits and that is precisely why ROI coin is Return on Investment.

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
> - Total coinbase potentially 7 Billion +/- coins. Variables depend on behavior of coin owners, term depositing and period of time for locked funds.

## Term Deposits

| Time     | Start Coins | End Coins | Coin Gain | Interest |
| :--------: | :--------: | :--------: | :--------: | :--------: |
| 1 week | 1000 | 1021 | 21 | 2.10% |
| 2 weeks | 1000 | 1043 | 43 | 4.30% |
| 1 month | 1000 | 1096 | 96 | 9.60% |
| 3 months | 1000 | 1331 | 331 | 33.10% |
| 6 months | 1000 | 1929 | 929 | 92.90% |
| 12 months | 1000 | 6933 | 5933 | 593.30% |

> **Note:**
> 
> - When a Term Deposit matures, it stops earning interest - you need to move it to start earning interest again.
> - Bonus rates are paid on regular balances as well. 15% APR

## Proof of Work

### 1GB AES Pattern Search PoW (HOdl)

ASIC/GPU resistant. Pattern Search involves filling up RAM with pseudo-random data, and then conducting a search for the start location of an AES encrypted data pattern in that data. Pattern Search is an evolution of the ProtoShares Momentum PoW, first used in MemoryCoin and later modified for use in CryptoNote (Monero,Bytecoin), Ethash(Ethereum), HOdl Coin, EuropeCoin and now ROI Coin.

## ROI Coin Ubuntu Compile Dependencies

### Compiling on Ubuntu (16.04 tested)):

Note
---------------------
Always use absolute paths to configure and compile bitcoin and the dependencies,
for example, when specifying the path of the dependency:

	../dist/configure --enable-cxx --disable-shared --with-pic --prefix=$BDB_PREFIX

Here BDB_PREFIX must be an absolute path - it is defined using $(pwd) which ensures
the usage of the absolute path.

To Build
---------------------

```bash
./autogen.sh
./configure --enable-tests=no
make
make install # optional
```

This will build bitcoin-qt as well if the dependencies are met.
It wont build the tests which take a long time.

Dependencies
---------------------

These dependencies are required:

 Library     | Purpose          | Description
 ------------|------------------|----------------------
 libssl      | Crypto           | Random Number Generation, Elliptic Curve Cryptography
 libboost    | Utility          | Library for threading, data structures, etc
 libevent    | Networking       | OS independent asynchronous networking

Optional dependencies:

 Library     | Purpose          | Description
 ------------|------------------|----------------------
 miniupnpc   | UPnP Support     | Firewall-jumping support
 libdb4.8    | Berkeley DB      | Wallet storage (only needed when wallet enabled)
 qt          | GUI              | GUI toolkit (only needed when GUI enabled)
 protobuf    | Payments in GUI  | Data interchange format used for payment protocol (only needed when GUI enabled)
 libqrencode | QR codes in GUI  | Optional for generating QR codes (only needed when GUI enabled)
 univalue    | Utility          | JSON parsing and encoding (bundled version will be used unless --with-system-univalue passed to configure)
 libzmq3     | ZMQ notification | Optional, allows generating ZMQ notifications (requires ZMQ version >= 4.x)

For the versions used, see [dependencies.md](dependencies.md)

Memory Requirements
--------------------

C++ compilers are memory-hungry. It is recommended to have at least 1.5 GB of
memory available when compiling Bitcoin Core. On systems with less, gcc can be
tuned to conserve memory with additional CXXFLAGS:


    ./configure CXXFLAGS="--param ggc-min-expand=1 --param ggc-min-heapsize=32768"

Dependency Build Instructions: Ubuntu & Debian
----------------------------------------------
Build requirements:

    sudo apt-get install build-essential libtool autotools-dev automake pkg-config libssl-dev libevent-dev bsdmainutils python3

Options when installing required Boost library files:

1. On at least Ubuntu 14.04+ and Debian 7+ there are generic names for the
individual boost development packages, so the following can be used to only
install necessary parts of boost:

        sudo apt-get install libboost-system-dev libboost-filesystem-dev libboost-chrono-dev libboost-program-options-dev libboost-test-dev libboost-thread-dev

2. If that doesn't work, you can install all boost development packages with:

        sudo apt-get install libboost-all-dev

BerkeleyDB is required for the wallet.

**For Ubuntu only:** db4.8 packages are available [here](https://launchpad.net/~bitcoin/+archive/bitcoin).
You can add the repository and install using the following commands:

    sudo apt-get install software-properties-common
    sudo add-apt-repository ppa:bitcoin/bitcoin
    sudo apt-get update
    sudo apt-get install libdb4.8-dev libdb4.8++-dev

Ubuntu and Debian have their own libdb-dev and libdb++-dev packages, but these will install
BerkeleyDB 5.1 or later, which break binary wallet compatibility with the distributed executables which
are based on BerkeleyDB 4.8. If you do not care about wallet compatibility,
pass `--with-incompatible-bdb` to configure.

See the section "Disable-wallet mode" to build Bitcoin Core without wallet.

Optional (see --with-miniupnpc and --enable-upnp-default):

    sudo apt-get install libminiupnpc-dev

ZMQ dependencies (provides ZMQ API 4.x):

    sudo apt-get install libzmq3-dev

Dependencies for the GUI: Ubuntu & Debian
-----------------------------------------

If you want to build Bitcoin-Qt, make sure that the required packages for Qt development
are installed. Either Qt 5 or Qt 4 are necessary to build the GUI.
If both Qt 4 and Qt 5 are installed, Qt 5 will be used. Pass `--with-gui=qt4` to configure to choose Qt4.
To build without GUI pass `--without-gui`.

To build with Qt 5 (recommended) you need the following:

    sudo apt-get install libqt5gui5 libqt5core5a libqt5dbus5 qttools5-dev qttools5-dev-tools libprotobuf-dev protobuf-compiler

Alternatively, to build with Qt 4 you need the following:

    sudo apt-get install libqt4-dev libprotobuf-dev protobuf-compiler

libqrencode (optional) can be installed with:

    sudo apt-get install libqrencode-dev

Once these are installed, they will be found by configure and a bitcoin-qt executable will be
built by default.

Dependency Build Instructions: Fedora
-------------------------------------
Build requirements:

    sudo dnf install gcc-c++ libtool make autoconf automake openssl-devel libevent-devel boost-devel libdb4-devel libdb4-cxx-devel python3

Optional:

    sudo dnf install miniupnpc-devel

To build with Qt 5 (recommended) you need the following:

    sudo dnf install qt5-qttools-devel qt5-qtbase-devel protobuf-devel

libqrencode (optional) can be installed with:

    sudo dnf install qrencode-devel

Notes
-----
The release is built with GCC and then "strip bitcoind" to strip the debug
symbols, which reduces the executable size by about 90%.


miniupnpc
---------

[miniupnpc](http://miniupnp.free.fr/) may be used for UPnP port mapping.  It can be downloaded from [here](
http://miniupnp.tuxfamily.org/files/).  UPnP support is compiled in and
turned off by default.  See the configure options for upnp behavior desired:

	--without-miniupnpc      No UPnP support miniupnp not required
	--disable-upnp-default   (the default) UPnP support turned off by default at runtime
	--enable-upnp-default    UPnP support turned on by default at runtime


Berkeley DB
-----------
It is recommended to use Berkeley DB 4.8. If you have to build it yourself:

```bash
BITCOIN_ROOT=$(pwd)

# Pick some path to install BDB to, here we create a directory within the bitcoin directory
BDB_PREFIX="${BITCOIN_ROOT}/db4"
mkdir -p $BDB_PREFIX

# Fetch the source and verify that it is not tampered with
wget 'http://download.oracle.com/berkeley-db/db-4.8.30.NC.tar.gz'
echo '12edc0df75bf9abd7f82f821795bcee50f42cb2e5f76a6a281b85732798364ef  db-4.8.30.NC.tar.gz' | sha256sum -c
# -> db-4.8.30.NC.tar.gz: OK
tar -xzvf db-4.8.30.NC.tar.gz

# Build the library and install to our prefix
cd db-4.8.30.NC/build_unix/
#  Note: Do a static build so that it can be embedded into the executable, instead of having to find a .so at runtime
../dist/configure --enable-cxx --disable-shared --with-pic --prefix=$BDB_PREFIX
make install

# Configure Bitcoin Core to use our own-built instance of BDB
cd $BITCOIN_ROOT
./autogen.sh
./configure LDFLAGS="-L${BDB_PREFIX}/lib/" CPPFLAGS="-I${BDB_PREFIX}/include/" # (other args...)
```

**Note**: You only need Berkeley DB if the wallet is enabled (see the section *Disable-Wallet mode* below).

Boost
-----
If you need to build Boost yourself:

	sudo su
	./bootstrap.sh
	./bjam install


Security
--------
To help make your bitcoin installation more secure by making certain attacks impossible to
exploit even if a vulnerability is found, binaries are hardened by default.
This can be disabled with:

Hardening Flags:

	./configure --enable-hardening
	./configure --disable-hardening


Hardening enables the following features:

* Position Independent Executable
    Build position independent code to take advantage of Address Space Layout Randomization
    offered by some kernels. Attackers who can cause execution of code at an arbitrary memory
    location are thwarted if they don't know where anything useful is located.
    The stack and heap are randomly located by default but this allows the code section to be
    randomly located as well.

    On an AMD64 processor where a library was not compiled with -fPIC, this will cause an error
    such as: "relocation R_X86_64_32 against `......' can not be used when making a shared object;"

    To test that you have built PIE executable, install scanelf, part of paxutils, and use:

    	scanelf -e ./bitcoin

    The output should contain:

     TYPE
    ET_DYN

* Non-executable Stack
    If the stack is executable then trivial stack based buffer overflow exploits are possible if
    vulnerable buffers are found. By default, bitcoin should be built with a non-executable stack
    but if one of the libraries it uses asks for an executable stack or someone makes a mistake
    and uses a compiler extension which requires an executable stack, it will silently build an
    executable without the non-executable stack protection.

    To verify that the stack is non-executable after compiling use:
    `scanelf -e ./bitcoin`

    the output should contain:
	STK/REL/PTL
	RW- R-- RW-

    The STK RW- means that the stack is readable and writeable but not executable.

Disable-wallet mode
--------------------
When the intention is to run only a P2P node without a wallet, bitcoin may be compiled in
disable-wallet mode with:

    ./configure --disable-wallet

In this case there is no dependency on Berkeley DB 4.8.

Mining is also possible in disable-wallet mode, but only using the `getblocktemplate` RPC
call not `getwork`.

Additional Configure Flags
--------------------------
A list of additional configure flags can be displayed with:

    ./configure --help

### Github Source Code: (note: do not "git" under sudo)

git clone https://github.com/ROIcoin/ROICoin.git ROIcoin

cd ROIcoin/

```bash
./autogen.sh
./configure --enable-tests=no
make
make install # optional
```

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

rpcuser=xxxx  (Replace the strings of 'xxxx' with preferred data.)
rpcpassword=xxxx
rpcport=3377



