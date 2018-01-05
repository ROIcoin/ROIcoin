// Copyright (c) 2014 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "crypto/sha512.h"

#include "crypto/common.h"

#include <string.h>

////// SHA-512

CSHA512::CSHA512()
{
    this->context = EVP_MD_CTX_new();
    this->initContext();
}

CSHA512::~CSHA512() {
   EVP_MD_CTX_free(this->context);
}

CSHA512& CSHA512::Write(const unsigned char* data, size_t len)
{

    if (unlikely(!EVP_DigestUpdate(this->context,data,len))) {
	printf("SHA512: failed to update digest\n");
	abort();
    }

    return *this;
}

void CSHA512::Finalize(unsigned char hash[OUTPUT_SIZE])
{

    unsigned int lengthOfHash;

    if (unlikely(!EVP_DigestFinal_ex(this->context, hash, &lengthOfHash)))
    {
	printf("SHA512: failed to finalize\n");
	abort();
    }

}

CSHA512& CSHA512::Reset()
{
    EVP_MD_CTX_reset(this->context); 
    this->initContext();

    return *this;
}

void CSHA512::initContext() {

    if (unlikely(!EVP_DigestInit_ex(this->context, EVP_sha512(), NULL))) {
	printf("SHA512: failed to create digest\n");
	abort();
     }

}
