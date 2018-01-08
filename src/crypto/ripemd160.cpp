// Copyright (c) 2014 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "crypto/ripemd160.h"

#include "crypto/common.h"

#include <string.h>

////// RIPEMD160

CRIPEMD160::CRIPEMD160()
{
   this->context = EVP_MD_CTX_new();
    this->initContext();
}

CRIPEMD160::~CRIPEMD160() {
   EVP_MD_CTX_free(this->context);
}

CRIPEMD160& CRIPEMD160::Write(const unsigned char* data, size_t len)
{

	if (unlikely(!EVP_DigestUpdate(this->context, data, len))) {
		printf("RIPEMD160: failed to update\n");
		abort();
	}

    return *this;
}

void CRIPEMD160::Finalize(unsigned char hash[OUTPUT_SIZE])
{
	
	unsigned int length;

	if (unlikely(!EVP_DigestFinal_ex(this->context,hash,&length))) {
		printf("RIPEMD160: failed to finalize\n");
		abort();
	}

}

CRIPEMD160& CRIPEMD160::Reset()
{
	EVP_MD_CTX_reset(this->context);
	this->initContext();

    return *this;
}

void CRIPEMD160::initContext() {
	if (unlikely(!EVP_DigestInit_ex(this->context, EVP_ripemd160(), NULL))) {
		printf("RIPEMD160: failed to finalize\n");
		abort();
	}
}
