#include "stdafx.h"
#include "TransactionInput.h"
#include "TransactionOutput.h"

using namespace std;

TransactionInput::TransactionInput(string transactionOutputId)
	: transactionOutputId(transactionOutputId)
{
}


TransactionInput::~TransactionInput()
{
}

TransactionOutput& TransactionInput::getUTXO()
{
	return *UTXO;
}

void TransactionInput::setUTXO(TransactionOutput& UTXO)
{
	this->UTXO = &UTXO;
}

const std::string& TransactionInput::getTransactionOutputId()
{
	return transactionOutputId;
}
