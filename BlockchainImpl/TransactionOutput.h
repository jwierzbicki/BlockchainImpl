#pragma once
#include <string>
#include "cryptopp/eccrypto.h"
class TransactionOutput
{
private:
	std::string id;
	CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PublicKey recipient;
	float value;
	std::string parentTransactionId;
public:
	TransactionOutput(CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PublicKey recipient, float value, std::string parentTransactionId);
	~TransactionOutput();
	bool isMine(CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PublicKey const& publicKey);
	const std::string& getId();
	const float getValue();
	const CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PublicKey& getRecipient();
};

