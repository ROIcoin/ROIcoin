// Copyright (c) 2014 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "crypto/sha1.h"
#include "crypto/common.h"
#include <string.h>

////// SHA1

CSHA1::CSHA1()
{
    this->context = EVP_MD_CTX_new();
    this->initContext();
}

CSHA1::~CSHA1() {
	EVP_MD_CTX_free(this->context);
}

CSHA1& CSHA1::Write(const unsigned char* data, size_t len)
{

	if (unlikely(!EVP_DigestUpdate(this->context, data, len))) {
	printf("SHA1: failed to update\n");
	abort();
	}

    return *this;
}

void CSHA1::Finalize(unsigned char hash[OUTPUT_SIZE])
{
	unsigned int length;
	if (unlikely(!EVP_DigestFinal_ex(this->context,hash,&length))) {
		printf("SHA1: failed to finalize\n");
		abort();
	}

}

CSHA1& CSHA1::Reset()
{
	EVP_MD_CTX_reset(this->context);
	this->initContext();
    return *this;
}

void CSHA1::initContext() {
	if (unlikely(!EVP_DigestInit_ex(this->context, EVP_sha1(), NULL))) {
		printf("SHA1: failed to initialize\n");
		abort();
	}
}

