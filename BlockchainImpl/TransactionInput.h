#pragma once
#include <string>
#include "TransactionOutput.h"

class TransactionInput
{
private:
	TransactionOutput* UTXO;
	std::string transactionOutputId;
public:
	TransactionInput(std::string transactionOutputId);
	~TransactionInput();
	TransactionOutput& getUTXO();
	const std::string& getTransactionOutputId();
	void setUTXO(TransactionOutput & UTXO);
};

