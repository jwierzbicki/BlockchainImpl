#pragma once
#include <string>
#include "cryptopp/eccrypto.h"
#include "TransactionInput.h"
#include "TransactionOutput.h"

class Transaction
{
private:
	std::string transactionId;
	CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PublicKey sender;
	CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PublicKey recipient;
	float value;
	std::string signature;
	std::vector<TransactionInput> inputs;
	std::vector<TransactionOutput> outputs;
	bool valid;
public:
	Transaction(CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PublicKey from,
		CryptoPP::ECDSA <CryptoPP::ECP, CryptoPP::SHA256>::PublicKey to,
		float value,
		std::vector<TransactionInput> inputs);
	Transaction();
	~Transaction();
	std::string calculateHash();
	void generateSignature(CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PrivateKey const& privateKey);
	bool verifySignature();
	bool processTransaction();
	const float getInputsValue();
	const float getOutputsValue();
	const std::string& getTransactionId();
	std::vector<TransactionOutput>& getOutputs();
	std::vector<TransactionInput>& getInputs();
	const CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PublicKey& getRecipient();
	const CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PublicKey& getSender();
	const float& getValue();
	const bool isValid();
	void setTransactionId(const std::string transactionId);
};

