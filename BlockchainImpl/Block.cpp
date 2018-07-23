// Block.cpp
#include "stdafx.h"
#include "Block.h"
#include <string>
#include <time.h>
#include "cryptopp/sha.h"
#include "cryptopp/hex.h"
#include "cryptopp/filters.h"
#include <Windows.h>
#include "utility.hpp"
#include "Transaction.h"

using namespace std;

Block::Block(string previousHash)
{
	this->nonce = 0;
	this->previousHash = previousHash;
	this->timeStamp = (long) time(NULL);
	this->hash = calculateHash();
}

Block::~Block()
{
}

void Block::mineBlock(int difficulty)
{
	char* cstr = new char[difficulty + 1];
	for (int i = 0; i < difficulty; i++)
	{
		cstr[i] = '0';
	}
	cstr[difficulty] = '\0';
	string str(cstr);
	delete[] cstr;

	do
	{
		nonce++;
		hash = calculateHash();
	} while (hash.substr(0, difficulty) != str);
}

const string Block::calculateHash()
{
	stringstream ss;
	ss << this->previousHash << this->timeStamp << to_string(this->nonce) << this->merkleRoot;
	return utility::applySHA256(ss.str());
}

const bool Block::addTransaction(Transaction transaction)
{
	// Process transaction and check if valid, unless block is genesis block
	if (!transaction.isValid())
	{
		return false;
	}
	if (previousHash != "0")
	{
		if (!transaction.processTransaction())
		{
			cout << "Transaction failed to process. Discarded" << endl;
			return false;
		}
	}

	transactions.push_back(transaction);
	cout << "Transaction successfully added to block" << endl;
	return true;
}

std::vector<Transaction>& Block::getTransactions()
{
	return transactions;
}

const string& Block::getData()
{
	return this->data;
}

const string& Block::getHash()
{
	return this->hash;
}

const string& Block::getPreviousHash()
{
	return this->previousHash;
}

const long& Block::getTimeStamp()
{
	return this->timeStamp;
}