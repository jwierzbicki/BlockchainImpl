// Block.h
#pragma once

#include <string>
#include <vector>
#include "Transaction.h"

class Block
{
private:
	std::vector<Transaction> transactions;
	std::string merkleRoot;
	std::string hash;
	std::string previousHash;
	std::string data;
	long timeStamp;
	int nonce;
public:
	Block(std::string previousHash);
	~Block();
	const std::string calculateHash();
	const std::string& getHash();
	const std::string& getPreviousHash();
	const std::string& getData();
	const long& getTimeStamp();
	void mineBlock(int difficulty);
	const bool addTransaction(Transaction transaction);
	std::vector<Transaction>& getTransactions();
};

