// Copyright (c) 2014 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "crypto/sha256.h"
#include "crypto/common.h"
#include <string.h>

////// SHA-256

CSHA256::CSHA256()
{
    this->context = EVP_MD_CTX_new();
    this->initContext();
}

CSHA256::~CSHA256() {
    EVP_MD_CTX_free(this->context);
}


CSHA256& CSHA256::Write(const unsigned char* data, size_t len)
{

	if (unlikely(!EVP_DigestUpdate(this->context, data, len)))
            {
		printf("SHA256: failed to update\n");
		abort();
	}

    return *this;
}

void CSHA256::Finalize(unsigned char hash[OUTPUT_SIZE])
{

	unsigned int lengthOfHash;

	if (unlikely(!EVP_DigestFinal_ex(context, hash, &lengthOfHash)))
                {
		printf("SHA256: failed to finalize\n");
		abort();
	}

}

CSHA256& CSHA256::Reset()
{
	EVP_MD_CTX_reset(this->context);
	this->initContext();
    return *this;

}

void CSHA256::initContext() {

	if (unlikely(!EVP_DigestInit_ex(context, EVP_sha256(), NULL)))
        {
		printf("SHA256: failed to init\n");
		abort();
	}

}

