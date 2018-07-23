#include "stdafx.h"
#include "BlockChain.h"
#include "Block.h"
#include "TransactionOutput.h"
#include <vector>
#include <map>
#include <string>

using namespace std;

const float BlockChain::minimumTransaction = 0.1f;
map<string, TransactionOutput> BlockChain::UTXOs;

BlockChain::BlockChain()
{
}


BlockChain::~BlockChain()
{
}

void BlockChain::addBlock(Block & block)
{
	cout << "Mining block..." << endl;
	block.mineBlock(difficulty);
	blockchain.push_back(block);
	cout << "Block mined!" << endl;
}

const size_t BlockChain::size()
{
	return blockchain.size();
}

Block& BlockChain::at(int pos)
{
	return blockchain.at(pos);
}
