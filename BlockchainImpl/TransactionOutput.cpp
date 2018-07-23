#include "stdafx.h"
#include "TransactionOutput.h"
#include "utility.hpp"
#include "cryptopp/eccrypto.h"
#include "cryptopp/ecp.h"
#include "cryptopp/sha.h"

using namespace std;
using namespace CryptoPP;

TransactionOutput::TransactionOutput(ECDSA<ECP, SHA256>::PublicKey recipient, float value, string parentTransactionId)
	: recipient(recipient), value(value), parentTransactionId(parentTransactionId)
{
	id = utility::applySHA256(utility::getStringFromPublicKey(recipient) + to_string(value) + parentTransactionId);
}


TransactionOutput::~TransactionOutput()
{
}

bool TransactionOutput::isMine(CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PublicKey const& publicKey)
{
	return publicKey == recipient;
}

const std::string& TransactionOutput::getId()
{
	return id;
}

const float TransactionOutput::getValue()
{
	return value;
}

const ECDSA<ECP, SHA256>::PublicKey& TransactionOutput::getRecipient()
{
	return recipient;
}