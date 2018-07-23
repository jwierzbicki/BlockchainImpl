#pragma once
#include <vector>
#include <string>
#include <map>
#include "TransactionOutput.h"
#include "Block.h"

class BlockChain
{
private:
	std::vector<Block> blockchain;
public:
	BlockChain();
	~BlockChain();
	void addBlock(Block & block);
	const size_t size();
	Block& at(int pos);
	static std::map<std::string, TransactionOutput> UTXOs;
	const static float minimumTransaction;
	const static int difficulty = 3;
};

