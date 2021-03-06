// BlockchainImpl.cpp : Defines the entry point for the console application.
#include "stdafx.h"
#include "Block.h"
#include "BlockChain.h"
#include "Wallet.h"
#include "Transaction.h"
#include "TransactionInput.h"
#include "TransactionOutput.h"
#include "utility.hpp"
#include <iostream>
#include <string>
#include <vector>
using namespace std;

//void printBlockInfo(Block*);
bool isChainValid();

BlockChain blockchain;
Transaction genesisTransaction;

int difficulty = blockchain.difficulty;

int main()
{
	Wallet walletA = Wallet();
	Wallet walletB = Wallet();
	Wallet coinbase = Wallet();

	genesisTransaction = Transaction(coinbase.getPublicKey(), walletA.getPublicKey(), 100.0f, vector<TransactionInput>());
	// manually sign the genesis transaction
	genesisTransaction.generateSignature(coinbase.getPrivateKey()); 
	genesisTransaction.setTransactionId("0");
	// manually add the Transaction Output
	genesisTransaction.getOutputs().push_back(TransactionOutput(genesisTransaction.getRecipient(), genesisTransaction.getValue(), genesisTransaction.getTransactionId()));

	BlockChain::UTXOs.insert(pair<string, TransactionOutput>(genesisTransaction.getOutputs().at(0).getId(), genesisTransaction.getOutputs().at(0)));

	cout << "Creating and mining genesis block..." << endl;
	Block genesis = Block("0");
	genesis.addTransaction(genesisTransaction);
	blockchain.addBlock(genesis);

	// testing
	Block block1 = Block(genesis.getHash());
	cout << "WalletA's balance is: " << walletA.getBalance() << endl;
	cout << "WalletA is attempting to send funds (40) to WalletB..." << endl;
	block1.addTransaction(walletA.sendFunds(walletB.getPublicKey(), 40.0f));

	blockchain.addBlock(block1);

	cout << "WalletA's balance is: " << walletA.getBalance() << endl;
	cout << "WalletB's balance is: " << walletB.getBalance() << endl;

	Block block2 = Block(block1.getHash());
	cout << "WalletA is attempting to send more funds (1000) than it has..." << endl;
	block2.addTransaction(walletA.sendFunds(walletB.getPublicKey(), 1000.0f));

	blockchain.addBlock(block2);

	cout << "WalletA's balance is: " << walletA.getBalance() << endl;
	cout << "WalletB's balance is: " << walletB.getBalance() << endl;

	Block block3 = Block(block2.getHash());
	cout << "WalletB is attempting to send funds (20) to WalletA..." << endl;
	block3.addTransaction(walletB.sendFunds(walletA.getPublicKey(), 20.0f));

	cout << "WalletA's balance is: " << walletA.getBalance() << endl;
	cout << "WalletB's balance is: " << walletB.getBalance() << endl;

	cout << "Validating blockchain" << endl;
	isChainValid();


	/*cout << "Test keys" << endl;
	cout << "Public key: " << utility::getStringFromPublicKey(walletA.getPublicKey()) << endl;
	cout << "Private key: " << utility::getStringFromPrivateKey(walletA.getPrivateKey()) << endl;
	cout << endl;
	
	Transaction transaction(walletA.getPublicKey(), walletB.getPublicKey(), 5, vector<TransactionInput>());
	transaction.generateSignature(walletA.getPrivateKey());

	cout << "Is signature verified" << endl;
	cout << transaction.verifySignature() << endl;*/

	// Testing blocks alone
	/*blockchain.push_back(Block("First block", "0"));
	blockchain.at(0).mineBlock(difficulty);

	blockchain.push_back(Block("Second block", blockchain.back().getHash()));
	blockchain.at(1).mineBlock(difficulty);

	blockchain.push_back(Block("Third block", blockchain.back().getHash()));
	blockchain.at(2).mineBlock(difficulty);


	cout << "First block: " << endl;
	printBlockInfo(&blockchain.at(0));

	cout << "Second block: " << endl;
	printBlockInfo(&blockchain.at(1));

	cout << "Third block: " << endl;
	printBlockInfo(&blockchain.at(2));

	cout << "Is chain valid: " << isChainValid() << endl;*/

    return 0;
}

bool isChainValid()
{
	map<string, TransactionOutput> tempUTXOs;
	tempUTXOs.insert(pair<string, TransactionOutput>(genesisTransaction.getOutputs().at(0).getId(), genesisTransaction.getOutputs().at(0)));

	Block* currentBlock;
	Block* previousBlock;

	char* cstr = new char[difficulty + 1];
	for (int i = 0; i < difficulty; i++)
	{
		cstr[i] = '0';
	}
	cstr[difficulty] = '\0';
	string hashTarget(cstr);
	delete[] cstr;

	// Loop through blockchain to check hashes
	for (unsigned int i = 1; i < blockchain.size(); i++)
	{
		currentBlock = &blockchain.at(i);
		previousBlock = &blockchain.at(i - 1);

		// Compare registered hash and calculated hash
		if (currentBlock->getHash().compare(currentBlock->calculateHash()) != 0)
		{
			cout << "Registed and calculated hashes don't match at: " << i << endl;
			return false;
		}

		// Compare previous hash and registered previous hash
		if (previousBlock->getHash().compare(currentBlock->getPreviousHash()) != 0)
		{
			cout << "Previous hashes don't match at: " << i << endl;
			return false;
		}

		if (currentBlock->getHash().substr(0, difficulty).compare(hashTarget) != 0)
		{
			cout << "Block hasn't been mined yet - at: " << i << endl;
			return false;
		}
		
		cout << "Validating transactions for block " << i << endl;
		// Loop through block transactions
		for (unsigned int t = 0; t < currentBlock->getTransactions().size(); t++)
		{
			Transaction currentTransaction = currentBlock->getTransactions().at(t);

			if (!currentTransaction.verifySignature())
			{
				cout << "Signature on transaction " << t << " is invalid" << endl;
				return false;
			}

			if (currentTransaction.getInputsValue() != currentTransaction.getOutputsValue())
			{
				cout << "Inputs are not equal to outputs on transaction " << t << endl;
				return false;
			}

			for (TransactionInput input : currentTransaction.getInputs())
			{
				TransactionOutput tempOutput = tempUTXOs.at(input.getTransactionOutputId());

				if (input.getUTXO().getValue() != tempOutput.getValue())
				{
					cout << "Referenced input transaction " << t << " value is invalid" << endl;
					return false;
				}

				tempUTXOs.erase(input.getTransactionOutputId());
			}

			for (TransactionOutput output : currentTransaction.getOutputs())
			{
				tempUTXOs.insert(pair<string, TransactionOutput>(output.getId(), output));
			}

			if (!(currentTransaction.getOutputs().at(0).getRecipient() == currentTransaction.getRecipient()))
			{
				cout << "Transaction " << t << " output recipient is not who it should be" << endl;
				return false;
			}

			if (!(currentTransaction.getOutputs().at(1).getRecipient() == currentTransaction.getSender()))
			{
				cout << "Transaction " << t << " output 'change' is not sender" << endl;
				return false;
			}
		}
	}
	cout << "Blockchain is valid" << endl;
	return true;
}

//void printBlockInfo(Block* block)
//{
//	cout << "Block message: " << block->getData() << endl;
//	cout << "Block hash: " << block->getHash() << endl;
//	cout << "Previous block hash: " << block->getPreviousHash() << endl;
//	cout << endl;
//}