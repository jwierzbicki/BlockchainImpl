#pragma once
#include "cryptopp/eccrypto.h"
#include "Transaction.h"

class Wallet
{
private:
	CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PrivateKey privateKey;
	CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PublicKey publicKey;
	void generateKeyPair();
	std::map<std::string, TransactionOutput> UTXOs;
public:
	Wallet();
	~Wallet();
	const CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PrivateKey& getPrivateKey();
	const CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PublicKey& getPublicKey();
	const float getBalance();
	Transaction sendFunds(CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PublicKey const& recipient, const float value);
};

