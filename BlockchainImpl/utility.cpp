#include "stdafx.h"
#include "utility.hpp"
#include <string>
#include <cstddef>
#include "cryptopp/eccrypto.h"
#include "cryptopp/sha.h"
#include "cryptopp/ecp.h"
#include "cryptopp/filters.h"
#include "cryptopp/hex.h"
#include "cryptopp/osrng.h"
#include "Transaction.h"

using namespace std;
using namespace CryptoPP;

namespace utility {
	const string applySHA256(string const& data)
	{
		SHA256 sha;
		string hashed;
		StringSource s(data, true, 
			new HashFilter(sha, new HexEncoder(new StringSink(hashed)))
		);
		return hashed;
	}

	const string applyECDSASig(ECDSA<ECP, SHA256>::PrivateKey const& privateKey, string const& input)
	{
		string signature;
		AutoSeededRandomPool prng;
		ECDSA<ECP, SHA256>::Signer signer(privateKey);
		StringSource s(input, true, 
			new SignerFilter(prng, signer, new StringSink(signature))
			//new SignerFilter(prng, signer, new HexEncoder(new StringSink(signature)))
		);
		return signature;
	}

	bool verifyECDSASig(ECDSA<ECP, SHA256>::PublicKey const& publicKey, string const& data, string const& signature)
	{
		ECDSA<ECP, SHA256>::Verifier verifier(publicKey);
		// Result of the verification process
		bool result = false;
		StringSource s(signature + data, true, 
			new SignatureVerificationFilter(verifier, new ArraySink((CryptoPP::byte*)&result, sizeof(result)))
		);
		return result;
	}

	const string getStringFromPublicKey(ECDSA<ECP, SHA256>::PublicKey const& publicKey)
	{
		string encoded;
		HexEncoder encoder;
		ByteQueue queue;

		publicKey.Save(queue);
		queue.CopyTo(encoder);
		encoder.MessageEnd();

		uint32_t size = encoder.MaxRetrievable();
		if (size)
		{
			encoded.resize(size);
			encoder.Get((CryptoPP::byte*)&encoded[0], encoded.size());
		}

		return encoded;
	}

	const string getStringFromPrivateKey(ECDSA<ECP, SHA256>::PrivateKey const& privateKey)
	{
		string encoded;
		HexEncoder encoder;
		ByteQueue queue;

		privateKey.Save(queue);
		queue.CopyTo(encoder);
		encoder.MessageEnd();

		uint32_t size = encoder.MaxRetrievable();
		if (size)
		{
			encoded.resize(size);
			encoder.Get((CryptoPP::byte*)&encoded[0], encoded.size());
		}

		return encoded;
	}

	const string getMerkleRoot(vector<Transaction> const& transactions)
	{
		int count = transactions.size();
		vector<string> previousTreeLayer;

		for (Transaction t : transactions)
		{
			previousTreeLayer.push_back(t.getTransactionId());
		}

		vector<string> treeLayer = previousTreeLayer;

		while (count > 1)
		{
			treeLayer.clear();

			for (unsigned int i = 1; i < previousTreeLayer.size(); i++)
			{
				treeLayer.push_back(previousTreeLayer.at(i - 1) + previousTreeLayer.at(i));
			}
			count = treeLayer.size();
			previousTreeLayer = treeLayer;
		}

		string merkleRoot = (treeLayer.size() == 1) ? treeLayer.at(0) : "";
		return merkleRoot;
	}
}