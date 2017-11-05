DIST_SUBDIRS = secp256k1
AM_LDFLAGS = $(PTHREAD_CFLAGS) $(LIBTOOL_LDFLAGS)


if EMBEDDED_LEVELDB
LEVELDB_CPPFLAGS += -I$(srcdir)/leveldb/include
LEVELDB_CPPFLAGS += -I$(srcdir)/leveldb/helpers/memenv
LIBLEVELDB += $(builddir)/leveldb/libleveldb.a
LIBMEMENV  += $(builddir)/leveldb/libmemenv.a

# NOTE: This dependency is not strictly necessary, but without it make may try to build both in parallel, which breaks the LevelDB build system in a race
$(LIBLEVELDB): $(LIBMEMENV)

$(LIBLEVELDB) $(LIBMEMENV):
	@echo "Building LevelDB ..." && $(MAKE) -C $(@D) $(@F) CXX="$(CXX)" \
	  CC="$(CC)" PLATFORM=$(TARGET_OS) AR="$(AR)" $(LEVELDB_TARGET_FLAGS) \
          OPT="$(CXXFLAGS) $(CPPFLAGS) -D__STDC_LIMIT_MACROS"
endif

ROICOIN_CONFIG_INCLUDES=-I$(builddir)/config
ROICOIN_INCLUDES=-I$(builddir) -I$(builddir)/obj $(BOOST_CPPFLAGS) $(LEVELDB_CPPFLAGS) $(CRYPTO_CFLAGS) $(SSL_CFLAGS)

ROICOIN_INCLUDES += -I$(srcdir)/secp256k1/include

LIBROICOIN_SERVER=libroicoin_server.a
LIBROICOIN_WALLET=libroicoin_wallet.a
LIBROICOIN_COMMON=libroicoin_common.a
LIBROICOIN_CLI=libroicoin_cli.a
LIBROICOIN_UTIL=libroicoin_util.a
LIBROICOIN_CRYPTO=crypto/libroicoin_crypto.a
LIBROICOIN_CRYPTO_X86=libroicoin_crypto_x86.a
LIBROICOIN_UNIVALUE=univalue/libroicoin_univalue.a
LIBROICOINQT=qt/libroicoinqt.a
LIBSECP256K1=secp256k1/libsecp256k1.la

$(LIBSECP256K1): $(wildcard secp256k1/src/*) $(wildcard secp256k1/include/*)
	$(AM_V_at)$(MAKE) $(AM_MAKEFLAGS) -C $(@D) $(@F)

# Make is not made aware of per-object dependencies to avoid limiting building parallelization
# But to build the less dependent modules first, we manually select their order here:
EXTRA_LIBRARIES = \
  crypto/libroicoin_crypto.a \
  libroicoin_util.a \
  libroicoin_common.a \
  libroicoin_crypto_x86.a \
  univalue/libroicoin_univalue.a \
  libroicoin_server.a \
  libroicoin_cli.a
if ENABLE_WALLET
ROICOIN_INCLUDES += $(BDB_CPPFLAGS)
EXTRA_LIBRARIES += libroicoin_wallet.a
endif

if BUILD_ROICOIN_LIBS
lib_LTLIBRARIES = libroicoinconsensus.la
LIBROICOIN_CONSENSUS=libroicoinconsensus.la
else
LIBROICOIN_CONSENSUS=
endif

bin_PROGRAMS =
TESTS =

if BUILD_ROICOIND
  bin_PROGRAMS += ROIcoind
endif

if BUILD_ROICOIN_UTILS
  bin_PROGRAMS += ROIcoin-cli ROIcoin-tx
endif

.PHONY: FORCE
# ROIcoin core #
ROICOIN_CORE_H = \
  addrman.h \
  aligned_malloc.h \
  aes.h \
  alert.h \
  amount.h \
  arith_uint256.h \
  base58.h \
  bloom.h \
  chain.h \
  chainparams.h \
  chainparamsbase.h \
  chainparamsseeds.h \
  checkpoints.h \
  checkqueue.h \
  clientversion.h \
  coincontrol.h \
  coins.h \
  compat.h \
  compat/byteswap.h \
  compat/endian.h \
  compat/sanity.h \
  compressor.h \
  consensus/consensus.h \
  consensus/params.h \
  consensus/validation.h \
  core_io.h \
  eccryptoverify.h \
  ecwrapper.h \
  hash.h \
  init.h \
  key.h \
  keystore.h \
  leveldbwrapper.h \
  limitedmap.h \
  main.h \
  memusage.h \
  merkleblock.h \
  miner.h \
  mruset.h \
  net.h \
  netbase.h \
  noui.h \
  patternsearch.h \
  policy/fees.h \
  pow.h \
  primitives/block.h \
  primitives/transaction.h \
  protocol.h \
  pubkey.h \
  random.h \
  reverselock.h \
  rpcclient.h \
  rpcprotocol.h \
  rpcserver.h \
  scheduler.h \
  script/interpreter.h \
  script/script.h \
  script/script_error.h \
  script/sigcache.h \
  script/sign.h \
  script/standard.h \
  sha512.h \
  serialize.h \
  streams.h \
  support/allocators/secure.h \
  support/allocators/zeroafterfree.h \
  support/cleanse.h \
  support/pagelocker.h \
  sync.h \
  threadsafety.h \
  timedata.h \
  tinyformat.h \
  txdb.h \
  txmempool.h \
  ui_interface.h \
  uint256.h \
  undo.h \
  util.h \
  utilmoneystr.h \
  utilstrencodings.h \
  utiltime.h \
  validationinterface.h \
  version.h \
  wallet/crypter.h \
  wallet/db.h \
  wallet/wallet.h \
  wallet/wallet_ismine.h \
  wallet/walletdb.h

JSON_H = \
  json/json_spirit.h \
  json/json_spirit_error_position.h \
  json/json_spirit_reader.h \
  json/json_spirit_reader_template.h \
  json/json_spirit_stream_reader.h \
  json/json_spirit_utils.h \
  json/json_spirit_value.h \
  json/json_spirit_writer.h \
  json/json_spirit_writer_template.h

obj/build.h: FORCE
	@$(MKDIR_P) $(builddir)/obj
	@$(top_srcdir)/share/genbuild.sh $(abs_top_builddir)/src/obj/build.h \
	  $(abs_top_srcdir)
libroicoin_util_a-clientversion.$(OBJEXT): obj/build.h

# server: shared between ROIcoind and ROIcoin-qt
libroicoin_server_a_CPPFLAGS = $(ROICOIN_INCLUDES) $(MINIUPNPC_CPPFLAGS)
libroicoin_server_a_SOURCES = \
  addrman.cpp \
  alert.cpp \
  bloom.cpp \
  chain.cpp \
  checkpoints.cpp \
  init.cpp \
  leveldbwrapper.cpp \
  main.cpp \
  merkleblock.cpp \
  miner.cpp \
  net.cpp \
  noui.cpp \
  patternsearch.cpp \
  policy/fees.cpp \
  pow.cpp \
  rest.cpp \
  rpcblockchain.cpp \
  rpcmining.cpp \
  rpcmisc.cpp \
  rpcnet.cpp \
  rpcrawtransaction.cpp \
  rpcserver.cpp \
  script/sigcache.cpp \
  timedata.cpp \
  txdb.cpp \
  txmempool.cpp \
  validationinterface.cpp \
  $(JSON_H) \
  $(ROICOIN_CORE_H)

# wallet: shared between ROIcoind and ROIcoin-qt, but only linked
# when wallet enabled
libroicoin_wallet_a_CPPFLAGS = $(ROICOIN_INCLUDES)
libroicoin_wallet_a_SOURCES = \
  wallet/crypter.cpp \
  wallet/db.cpp \
  wallet/rpcdump.cpp \
  wallet/rpcwallet.cpp \
  wallet/wallet.cpp \
  wallet/wallet_ismine.cpp \
  wallet/walletdb.cpp \
  $(ROICOIN_CORE_H)

# crypto primitives library
crypto_libroicoin_crypto_a_CPPFLAGS = $(ROICOIN_CONFIG_INCLUDES)
crypto_libroicoin_crypto_a_SOURCES = \
  crypto/common.h \
  crypto/hmac_sha256.cpp \
  crypto/hmac_sha256.h \
  crypto/hmac_sha512.cpp \
  crypto/hmac_sha512.h \
  crypto/ripemd160.cpp \
  crypto/ripemd160.h \
  crypto/sha1.cpp \
  crypto/sha1.h \
  crypto/sha256.cpp \
  crypto/sha256.h \
  crypto/sha512.cpp \
  crypto/sha512.h

# univalue JSON library
univalue_libroicoin_univalue_a_SOURCES = \
  univalue/univalue.cpp \
  univalue/univalue.h \
  univalue/univalue_escapes.h \
  univalue/univalue_read.cpp \
  univalue/univalue_write.cpp

libroicoin_crypto_x86_a_CPPFLAGS = $(ROICOIN_INCLUDES) $(CRYPTO_X86_CXXFLAGS) $(CRYPTO_AES_CXXFLAGS) $(CRYPTO_AVX2_CXXFLAGS)
libroicoin_crypto_x86_a_SOURCES = \
  aes.cpp \
  sha512.cpp

# common: shared between ROIcoind, and ROIcoin-qt and non-server tools
libroicoin_common_a_CPPFLAGS = $(ROICOIN_INCLUDES)
libroicoin_common_a_SOURCES = \
  amount.cpp \
  arith_uint256.cpp \
  base58.cpp \
  chainparams.cpp \
  coins.cpp \
  compressor.cpp \
  core_read.cpp \
  core_write.cpp \
  eccryptoverify.cpp \
  ecwrapper.cpp \
  hash.cpp \
  key.cpp \
  keystore.cpp \
  netbase.cpp \
  patternsearch.cpp \
  primitives/block.cpp \
  primitives/transaction.cpp \
  protocol.cpp \
  pubkey.cpp \
  scheduler.cpp \
  script/interpreter.cpp \
  script/script.cpp \
  script/script_error.cpp \
  script/sign.cpp \
  script/standard.cpp \
  $(ROICOIN_CORE_H)

# util: shared between all executables.
# This library *must* be included to make sure that the glibc
# backward-compatibility objects and their sanity checks are linked.
libroicoin_util_a_CPPFLAGS = $(ROICOIN_INCLUDES)
libroicoin_util_a_SOURCES = \
  support/pagelocker.cpp \
  chainparamsbase.cpp \
  clientversion.cpp \
  compat/glibc_sanity.cpp \
  compat/glibcxx_sanity.cpp \
  compat/strnlen.cpp \
  random.cpp \
  rpcprotocol.cpp \
  support/cleanse.cpp \
  sync.cpp \
  uint256.cpp \
  util.cpp \
  utilmoneystr.cpp \
  utilstrencodings.cpp \
  utiltime.cpp \
  $(ROICOIN_CORE_H)

if GLIBC_BACK_COMPAT
libroicoin_util_a_SOURCES += compat/glibc_compat.cpp
endif

# cli: shared between ROIcoin-cli and ROIcoin-qt
libroicoin_cli_a_CPPFLAGS = $(ROICOIN_INCLUDES)
libroicoin_cli_a_SOURCES = \
  rpcclient.cpp \
  $(ROICOIN_CORE_H)

nodist_libroicoin_util_a_SOURCES = $(srcdir)/obj/build.h
#

# ROIcoind binary #
ROIcoind_SOURCES = roicoind.cpp
ROIcoind_CPPFLAGS = $(ROICOIN_INCLUDES)
ROIcoind_LDFLAGS = $(RELDFLAGS) $(AM_LDFLAGS) $(LIBTOOL_APP_LDFLAGS)

if TARGET_WINDOWS
ROIcoind_SOURCES += roicoind-res.rc
endif

ROIcoind_LDADD = \
  $(LIBROICOIN_SERVER) \
  $(LIBROICOIN_COMMON) \
  $(LIBROICOIN_UNIVALUE) \
  $(LIBROICOIN_UTIL) \
  $(LIBROICOIN_CRYPTO) \
  $(LIBROICOIN_CRYPTO_X86) \
  $(LIBLEVELDB) \
  $(LIBMEMENV) \
  $(LIBSECP256K1)

if ENABLE_WALLET
ROIcoind_LDADD += libroicoin_wallet.a
endif

ROIcoind_LDADD += $(BOOST_LIBS) $(BDB_LIBS) $(SSL_LIBS) $(CRYPTO_LIBS) $(MINIUPNPC_LIBS)
#

# ROIcoin-cli binary #
ROIcoin_cli_SOURCES = roicoin-cli.cpp
ROIcoin_cli_CPPFLAGS = $(ROICOIN_INCLUDES)
ROIcoin_cli_LDFLAGS = $(RELDFLAGS) $(AM_LDFLAGS) $(LIBTOOL_APP_LDFLAGS)

if TARGET_WINDOWS
ROIcoin_cli_SOURCES += roicoin-cli-res.rc
endif

ROIcoin_cli_LDADD = \
  $(LIBROICOIN_CLI) \
  $(LIBROICOIN_UTIL)

ROIcoin_cli_LDADD += $(BOOST_LIBS) $(SSL_LIBS) $(CRYPTO_LIBS)
#

# ROIcoin-tx binary #
ROIcoin_tx_SOURCES = roicoin-tx.cpp
ROIcoin_tx_CPPFLAGS = $(ROICOIN_INCLUDES)
ROIcoin_tx_LDFLAGS = $(RELDFLAGS) $(AM_LDFLAGS) $(LIBTOOL_APP_LDFLAGS)

if TARGET_WINDOWS
ROIcoin_tx_SOURCES += roicoin-tx-res.rc
endif

ROIcoin_tx_LDADD = \
  $(LIBROICOIN_UNIVALUE) \
  $(LIBROICOIN_COMMON) \
  $(LIBROICOIN_UTIL) \
  $(LIBROICOIN_CRYPTO) \
  $(LIBROICOIN_CRYPTO_X86) \
  $(LIBSECP256K1)

ROIcoin_tx_LDADD += $(BOOST_LIBS) $(CRYPTO_LIBS)
#

# roicoinconsensus library #
if BUILD_ROICOIN_LIBS
include_HEADERS = script/roicoinconsensus.h
libroicoinconsensus_la_SOURCES = \
  crypto/hmac_sha512.cpp \
  crypto/ripemd160.cpp \
  crypto/sha1.cpp \
  crypto/sha256.cpp \
  crypto/sha512.cpp \
  eccryptoverify.cpp \
  ecwrapper.cpp \
  hash.cpp \
  primitives/transaction.cpp \
  pubkey.cpp \
  script/roicoinconsensus.cpp \
  script/interpreter.cpp \
  script/script.cpp \
  uint256.cpp \
  utilstrencodings.cpp

if GLIBC_BACK_COMPAT
  libroicoinconsensus_la_SOURCES += compat/glibc_compat.cpp
endif

libroicoinconsensus_la_LDFLAGS = -no-undefined $(RELDFLAGS)
libroicoinconsensus_la_LIBADD = $(CRYPTO_LIBS)
libroicoinconsensus_la_CPPFLAGS = $(CRYPTO_CFLAGS) -I$(builddir)/obj -DBUILD_ROICOIN_INTERNAL

endif
#

CLEANFILES = leveldb/libleveldb.a leveldb/libmemenv.a *.gcda *.gcno

DISTCLEANFILES = obj/build.h

EXTRA_DIST = leveldb

clean-local:
	-$(MAKE) -C leveldb clean
	-$(MAKE) -C secp256k1 clean
	rm -f leveldb/*/*.gcno leveldb/helpers/memenv/*.gcno
	-rm -f config.h

.rc.o:
	@test -f $(WINDRES)
	$(AM_V_GEN) $(WINDRES) -DWINDRES_PREPROC -i $< -o $@

.mm.o:
	$(AM_V_CXX) $(OBJCXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) \
	  $(CPPFLAGS) $(AM_CXXFLAGS) $(QT_INCLUDES) $(CXXFLAGS)  -c -o $@ $<

%.pb.cc %.pb.h: %.proto
	@test -f $(PROTOC)
	$(AM_V_GEN) $(PROTOC) --cpp_out=$(@D) --proto_path=$(abspath $(<D) $<)

if ENABLE_TESTS
include Makefile.test.include
endif

if ENABLE_QT
include Makefile.qt.include
endif

if ENABLE_QT_TESTS
include Makefile.qttest.include
endif
