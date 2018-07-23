#include "stdafx.h"
#include "Wallet.h"
#include "cryptopp/eccrypto.h"
#include "cryptopp/ecp.h"
#include "cryptopp/osrng.h"
#include "cryptopp/oids.h"
#include <map>
#include "TransactionOutput.h"
#include "Transaction.h"
#include "BlockChain.h"

#define ECC_CURVE ASN1::secp256r1()

using namespace CryptoPP;
using namespace std;

Wallet::Wallet()
{
	generateKeyPair();
}


Wallet::~Wallet()
{
}

void Wallet::generateKeyPair()
{
	AutoSeededRandomPool prng;
	privateKey.Initialize(prng, ECC_CURVE); // exception
	privateKey.MakePublicKey(publicKey);
}

const ECDSA<ECP, SHA256>::PrivateKey& Wallet::getPrivateKey()
{
	return privateKey;
}

const ECDSA<ECP, SHA256>::PublicKey& Wallet::getPublicKey()
{
	return publicKey;
}

// Returns balance and stores the UTXOs owned by this wallet in UTXOs
const float Wallet::getBalance()
{
	float total = 0;

	for (map<string, TransactionOutput>::iterator it = BlockChain::UTXOs.begin(); it != BlockChain::UTXOs.end(); ++it)
	{
		TransactionOutput UTXO = it->second;
		if (UTXO.isMine(publicKey))
		{
			// Add it to list of unspent transactions
			UTXOs.insert(pair<string, TransactionOutput>(UTXO.getId(), UTXO));
			total += UTXO.getValue();
		}
	}

	return total;
}

// Generates and returns a new transaction from this wallet
Transaction Wallet::sendFunds(CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PublicKey const& recipient, const float value)
{
	if (getBalance() < value)
	{
		cout << "Not enough funds to send transaction. Transaction discarded." << endl;
		return Transaction();
	}

	// Create list of inputs
	vector<TransactionInput> inputs;

	float total = 0;

	for (map<string, TransactionOutput>::iterator it = UTXOs.begin(); it != UTXOs.end(); ++it)
	{
		TransactionOutput UTXO = it->second;
		total += UTXO.getValue();
		inputs.push_back(TransactionInput(UTXO.getId()));
		if (total > value)
			break;
	}

	Transaction newTransaction(publicKey, recipient, value, inputs);
	newTransaction.generateSignature(privateKey);

	for (TransactionInput input : inputs)
	{
		UTXOs.erase(input.getTransactionOutputId());
	}

	return newTransaction;
}
