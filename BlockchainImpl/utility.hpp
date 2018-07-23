#pragma once
#include <string>
#include <vector>
#include "Transaction.h"
#include "cryptopp/eccrypto.h"

namespace utility
{
	const std::string applySHA256(std::string const& data);
	const std::string applyECDSASig(CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PrivateKey const& privateKey,
		std::string const& input);
	bool verifyECDSASig(CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PublicKey const& publicKey,
		std::string const& data, 
		std::string const& signature);
	const std::string getStringFromPublicKey(CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PublicKey const& publicKey);
	const std::string getStringFromPrivateKey(CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PrivateKey const& privateKey);
	const std::string getMerkleRoot(std::vector<Transaction> const& transactions);
}