#include "stdafx.h"
#include "Transaction.h"
#include "utility.hpp"
#include <string>
#include "cryptopp/eccrypto.h"
#include "cryptopp/ecp.h"
#include <cstddef>
#include "BlockChain.h"

static int sequence = 0;

using namespace std;
using namespace CryptoPP;

Transaction::Transaction(ECDSA<ECP, SHA256>::PublicKey from, 
						ECDSA<ECP, SHA256>::PublicKey to, 
						float value, 
						vector<TransactionInput> inputs)
	: sender(from), recipient(to), value(value), inputs(inputs)
{
	valid = true;
}

Transaction::Transaction()
{
	valid = false;
}

Transaction::~Transaction()
{
}

std::string Transaction::calculateHash()
{
	sequence++;
	return utility::applySHA256(utility::getStringFromPublicKey(sender) 
		+ utility::getStringFromPublicKey(recipient) 
		+ to_string(value) 
		+ to_string(sequence));
}

void Transaction::generateSignature(ECDSA<ECP, SHA256>::PrivateKey const& privateKey)
{
	string data = utility::getStringFromPublicKey(sender) + utility::getStringFromPublicKey(recipient) + to_string(value);
	signature = utility::applyECDSASig(privateKey, data);
}

bool Transaction::verifySignature()
{
	string data = utility::getStringFromPublicKey(sender) + utility::getStringFromPublicKey(recipient) + to_string(value);
	return utility::verifyECDSASig(sender, data, signature);
}

bool Transaction::processTransaction()
{
	if (!verifySignature())
	{
		cout << "Transaction Signature failed to verify" << endl;
		return false;
	}

	// Gather transaction inputs (make sure they are unspent)
	for (TransactionInput& i : inputs)
	{
		i.setUTXO(BlockChain::UTXOs.at(i.getTransactionOutputId()));
	}

	float inputVals = getInputsValue();

	// Check if transaction is valid
	if (inputVals < BlockChain::minimumTransaction)
	{
		cout << "Transaction Inputs too small: " << to_string(inputVals) << endl;
		return false;
	}

	// Generate transaction outputs
	float leftOver = inputVals - value;
	transactionId = calculateHash();

	// Send value to the recipient
	outputs.push_back(TransactionOutput(recipient, value, transactionId));

	// Send the left over 'change' back to sender
	outputs.push_back(TransactionOutput(sender, leftOver, transactionId));

	// Add outputs to Unspent list
	for (TransactionOutput o : outputs)
	{
		BlockChain::UTXOs.insert(pair<string, TransactionOutput>(o.getId(), o));
	}

	// Remove transaction inputs from UTXO lists as spent
	for (TransactionInput i : inputs)
	{
		BlockChain::UTXOs.erase(i.getUTXO().getId());
	}

	return true;
}

// Returns sum of inputs (UTXOs) values
const float Transaction::getInputsValue()
{
	float total = 0;
	for (TransactionInput i : inputs)
	{
		total += i.getUTXO().getValue();
	}

	return total;
}

// Returns sum of outputs values
const float Transaction::getOutputsValue()
{
	float total = 0;
	for (TransactionOutput o : outputs)
	{
		total += o.getValue();
	}

	return total;
}

const string& Transaction::getTransactionId()
{
	return transactionId;
}


std::vector<TransactionOutput>& Transaction::getOutputs()
{
	return outputs;
}

std::vector<TransactionInput>& Transaction::getInputs()
{
	return inputs;
}

const ECDSA<ECP, SHA256>::PublicKey& Transaction::getRecipient()
{
	return recipient;
}

const ECDSA<ECP, SHA256>::PublicKey& Transaction::getSender()
{
	return sender;
}

const float& Transaction::getValue()
{
	return value;
}

const bool Transaction::isValid()
{
	return valid;
}
	
void Transaction::setTransactionId(const string transactionId)
{
	this->transactionId = transactionId;
}