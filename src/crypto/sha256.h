// Copyright (c) 2014 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_CRYPTO_SHA256_H
#define BITCOIN_CRYPTO_SHA256_H

#include <stdint.h>
#include <stdlib.h>
#include <openssl/sha.h>
#include <openssl/evp.h>

/** A hasher class for SHA-256. */
class CSHA256
{
private:

    EVP_MD_CTX* context;

    void initContext();

public:
    static const size_t OUTPUT_SIZE = SHA256_DIGEST_LENGTH;

    CSHA256();
    ~CSHA256();

    CSHA256& Write(const unsigned char* data, size_t len);
    void Finalize(unsigned char hash[OUTPUT_SIZE]);
    CSHA256& Reset();
};

#endif // BITCOIN_CRYPTO_SHA256_H
